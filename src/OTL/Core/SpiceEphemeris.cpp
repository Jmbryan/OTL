////////////////////////////////////////////////////////////
//
// OTL - Orbital Trajectory Library
// Copyright (C) 2013-2018 Jason Bryan (Jmbryan10@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifdef OTL_SPICE
#define OTL_SPICE_EPHEMERIS
#ifdef OTL_SPICE_EPHEMERIS

#include <OTL/Core/SpiceEphemeris.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/System.h>
#include <OTL/Core/Logger.h>

#include <OTL/Core/OrbitalBody.h> // for PhysicalProperties

extern "C"
{
#include  "cspice/SpiceUsr.h"
}
#include <iterator>
#include <vector>

namespace otl
{

////////////////////////////////////////////////////////////
SpiceEphemeris::SpiceEphemeris(const std::string& dataFileName,
                               const std::string& observerBodyName,
                               const std::string& referenceFrameName) :
IEphemeris(),
m_dataFilename(dataFileName),
m_observerBodyName(observerBodyName),
m_referenceFrameName(referenceFrameName)
{

}

////////////////////////////////////////////////////////////
SpiceEphemeris::~SpiceEphemeris()
{

}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetDataFile(const std::string& dataFileName)
{
   m_dataFilename = dataFileName;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetReferenceFrame(const std::string& referenceFrameName)
{
   m_referenceFrameName = referenceFrameName;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetAbberationCorrections(const std::string& abberationCorrectionsName)
{
   m_abberationCorrections = abberationCorrectionsName;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetObserverBody(const std::string& observerBodyName)
{
   m_observerBodyName = observerBodyName;
}

////////////////////////////////////////////////////////////
double SpiceEphemeris::GetBodyProperty(const std::string& targetBodyName, const std::string& propertyName)
{
   // Retrieve the property
   SpiceInt dimension;
   SpiceDouble value[1];
   bodvrd_c(targetBodyName.c_str(), propertyName.c_str(), 1, &dimension, value);

   return value[0];
}

////////////////////////////////////////////////////////////
std::vector<double> SpiceEphemeris::GetBodyProperties(const std::string& targetBodyName,
                                                      const std::string& propertyName,
                                                      const int maxDimension)
{
   // Retrieve the properties
   SpiceInt dimension;
   SpiceDouble* rawValues = new SpiceDouble[maxDimension];
   bodvrd_c(targetBodyName.c_str(), propertyName.c_str(), maxDimension, &dimension, rawValues);
   
   std::vector<double> values(rawValues, rawValues + dimension);
   delete[] rawValues;
   return values;
}

////////////////////////////////////////////////////////////
OrbitalElements SpiceEphemeris::ConvertCartesianStateVectorToOrbitalElements(const StateVector& cartesianStateVector,
                                                                             double gravitationalParameterCentralBody,
                                                                             const Epoch& epoch)
{
   // Unpack the cartesian state vector in the input array
   SpiceDouble state[6];  
   for (int i = 0; i < 3; ++i)
   {
      state[i] = cartesianStateVector.position[i];
      state[i + 3] = cartesianStateVector.velocity[i + 3];
   }

   // Compuate the ephemeris time of epoch
   SpiceDouble et = CalculateEphemerisTime(epoch);

   // Convert to orbital elements
   SpiceDouble elts[8];
   oscelt_c(state, et, gravitationalParameterCentralBody, elts);

   // Unpack the output array
   double rp = elts[0];
   double e = elts[1];
   double i = elts[2];
   double l = elts[3];
   double w = elts[4];
   double M = elts[5];

   // Compute semi-major axis
   double a = rp * (1 + e) / (1 - SQR(e));

   // Compute true anomaly
   keplerian::KeplersEquationElliptical kepler;
   double TA = kepler.Evaluate(e, M);

   // Return the results
   return OrbitalElements(a, e, TA, i, w, l);
}

StateVector SpiceEphemeris::ConvertOrbitalElementsToCartesianStateVector(const OrbitalElements& orbitalElements,
                                                                         double gravitationalParameterCentralBody,
                                                                         const Epoch& epoch)
{
   // Compuate ephemeris time of epoch
   double et = CalculateEphemerisTime(epoch);

   // Unpack orbital elements
   double a = orbitalElements.semiMajorAxis;
   double e = orbitalElements.eccentricity;
   double TA = orbitalElements.trueAnomaly;

   // Compute radius of pericenter
   double rp = a * (1 - SQR(e)) / (1 + e);

   // Compute mean anomaly
   double M = ConvertTrueAnomaly2Anomaly(e, TA);

   // Build the spice orbital element array
   SpiceDouble elts[8];
   elts[0] = rp;
   elts[1] = e;
   elts[2] = orbitalElements.inclination;
   elts[3] = orbitalElements.lonOfAscendingNode;
   elts[4] = orbitalElements.argOfPericenter;
   elts[5] = M;
   elts[6] = et;
   elts[7] = gravitationalParameterCentralBody;

   // Convert to cartesian state vector
   SpiceDouble state[6];
   conics_c(elts, et, state);
 
   StateVector cartesianStateVector;
   for (int i = 0; i < 3; ++i)
   {
      cartesianStateVector.position[i] = state[i];
      cartesianStateVector.velocity[i] = state[i + 3];
   }

   // Return the results
   return cartesianStateVector;
}

////////////////////////////////////////////////////////////
int SpiceEphemeris::GetNumKernalsLoaded() const
{
   SpiceInt count;
   ktotal_c("ALL", &count);
   return count;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::LoadDataFile(const std::string& dataFileName)
{
   SetDataFile(dataFileName);
   VLoad();
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VLoad()
{
   if (m_dataFilename.empty())
   {
      OTL_ERROR() << "Failed to load ephemeris datafile. No file specified";
      return;
   }
   
   // SPICE doesn't throw exceptions...
   try
   {
      furnsh_c(m_dataFilename.c_str());
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught while trying to load ephemeris datafile " <<
         Bracket(m_dataFilename) << ": " << Bracket(ex.what());
   }  
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VInitialize()
{
   m_abberationCorrections = "NONE";

   // Example: ftp://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/info/mostused.html#D
   // SpiceInt n;
   // SpiceDouble gm;
   // SpiceDouble et;
   // SpiceDouble state[6];
   // SpiceDouble lt;
   // SpiceDouble elts[8];
   //
   // load kernels: LSK, Mex trajectory SPK, and gravity PCK
   // furnsh_c("naif0008.tls");
   // furnsh_c("ORMM__050901000000_00165.BSP");
   // furnsh_c("DE403-MASSES.TPC");
   // bodvrd_c("MARS", "GM", 1, &n, gm);                          // reterives GM for Mars
   // str2et_c("2005 SEP 02 04:50:45", &et);                      // convert UTC to ET
   // spkezr_c("MEX", et, "MARSIAU", "NONE", "MARS", state, &lt); // compute state of MEX at given UTC
   // oscelt_c(state, et, gm, etls);                              // converts state vector to orbital elements
}

////////////////////////////////////////////////////////////
bool SpiceEphemeris::VIsValidName(const std::string& name)
{
   SpiceBoolean found = false;

   if (!name.empty())
   {
      SpiceInt bodyNaifId;
      bods2c_c(name.c_str(), &bodyNaifId, &found);
   }
   
   return (found > 0);
}

////////////////////////////////////////////////////////////
bool SpiceEphemeris::VIsValidEpoch(const Epoch& epoch)
{
   return true;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   double ephemerisTime = CalculateEphemerisTime(epoch);
   double pos[3];
   double lightTime;

   spkpos_c(name.c_str(),
            ephemerisTime,
            m_referenceFrameName.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBodyName.c_str(),
            pos,
            &lightTime);

   for (int i = 0; i < 3; ++i)
   {
      position[i] = pos[i];
   }
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   // Query the state vector at the given epoch
   StateVector stateVector;
   VGetStateVector(name, epoch, stateVector);

   // Return the velocity vector
   velocity = stateVector.velocity;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   double ephemerisTime = CalculateEphemerisTime(epoch);
   double state[6];
   double lightTime;

   spkezr_c(name.c_str(),
            ephemerisTime,
            m_referenceFrameName.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBodyName.c_str(),
            state,
            &lightTime);

   for (int i = 0; i < 3; ++i)
   {
      stateVector.position[i] = state[i];
      stateVector.velocity[i] = state[i + 3];
   }
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   // Query the state vector at the given epoch
   StateVector stateVector;
   VGetStateVector(name, epoch, stateVector);

   // Convert state vector to orbital elements
   orbitalElements = ConvertCartesianStateVector2OrbitalElements(stateVector, ASTRO_MU_SUN);
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetPhysicalProperties(const std::string& name, PhysicalProperties& physicalProperties)
{
   try
   {
      // Retrieve mu and compute mass
      double mu = GetBodyProperty(name, "GM");
      double mass = mu / ASTRO_GRAVITATIONAL_CONSTANT;

      // Retrieve radius vector and compute mean equatorial radius
      auto radius = GetBodyProperties(name, "RADII", 3);
      double meanRadius = (radius[0] + radius[1] + radius[2]) / 3.0;

      physicalProperties = PhysicalProperties(mass, meanRadius);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve physical properties for "
         << Bracket(name) << ": " << Bracket(ex.what());
   }
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetGravitationalParameterCentralBody(const std::string& name, double& gravitationalParameterCentralBody)
{
   try
   {
      gravitationalParameterCentralBody = GetBodyProperty(m_observerBodyName, "GM");
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve gravitational parameter for "
         << Bracket(m_observerBodyName) << ": " << Bracket(ex.what());
   }
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector)
{
   SpiceDouble ephemerisTime = CalculateEphemerisTime(epoch);
   SpiceDouble state[6];
   SpiceDouble lightTime;

   spkezr_c(name.c_str(),
            ephemerisTime,
            m_referenceFrameName.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBodyName.c_str(),
            state,
            &lightTime);

   StateVector cartesianStateVector;
   for (int i = 0; i < 3; ++i)
   {
      cartesianStateVector.position[i] = state[i];
      cartesianStateVector.velocity[i] = state[i + 3];
   }

   stateVector = cartesianStateVector;
}

////////////////////////////////////////////////////////////
double SpiceEphemeris::CalculateEphemerisTime(const Epoch& epoch) const
{
   return (epoch.GetJD() - j2000_c()) * spd_c();
}

} // namespace otl

#endif
#endif