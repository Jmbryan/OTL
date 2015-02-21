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

#include <OTL/Core/SpiceEphemeris.h>
#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/System.h>
#include <OTL/Core/Logger.h>

extern "C"
{
#include  "cspice/SpiceUsr.h"
}
#include <iterator>
#include <vector>

namespace otl
{

////////////////////////////////////////////////////////////
SpiceEphemeris::SpiceEphemeris(const std::string& dataFilename,
                               const std::string& observerBodyName,
                               const std::string& referenceFrameName) :
IEphemeris(dataFilename),
m_observerBodyName(observerBodyName),
m_referenceFrameName(referenceFrameName)
{

}

////////////////////////////////////////////////////////////
SpiceEphemeris::~SpiceEphemeris()
{

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
OrbitalElements SpiceEphemeris::ConvertCartesianStateVectorToOrbitalElements(const CartesianStateVector& cartesianStateVector,
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

CartesianStateVector SpiceEphemeris::ConvertOrbitalElementsToCartesianStateVector(const OrbitalElements& orbitalElements,
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
 
   // Return the results
   return CartesianStateVector(
      state[0], state[1], state[2],
      state[3], state[4], state[5]);
}

////////////////////////////////////////////////////////////
int SpiceEphemeris::GetNumKernalsLoaded() const
{
   SpiceInt count;
   ktotal_c("ALL", &count);
   return count;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VLoad()
{
   const auto& dataFilename = GetDataFilename();

   if (dataFilename.empty())
   {
      OTL_ERROR() << "Failed to load ephemeris datafile. No file specified";
      return;
   }
   
   // SPICE doesn't throw exceptions...
   try
   {
      furnsh_c(dataFilename.c_str());
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught while trying to load ephemeris datafile " <<
         Bracket(dataFilename) << ": " << Bracket(ex.what());
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
   SpiceInt bodyNaifId;

   if (!name.empty())
   {     
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
Vector3d SpiceEphemeris::GetPosition(const std::string& name, const Epoch& epoch) const
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

   return Vector3d(pos[0], pos[1], pos[2]);
}

////////////////////////////////////////////////////////////
PhysicalProperties SpiceEphemeris::VGetPhysicalProperties(const std::string& name)
{
   try
   {
      // Retrieve mu and compute mass
      double mu = GetBodyProperty(name, "GM");
      double mass = mu / ASTRO_GRAVITATIONAL_CONSTANT;

      // Retrieve radius vector and compute mean equatorial radius
      auto radius = GetBodyProperties(name, "RADII", 3);
      double meanRadius = (radius[0] + radius[1] + radius[2]) / 3.0;

      return PhysicalProperties(mass, meanRadius);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve physical properties for "
         << Bracket(name) << ": " << Bracket(ex.what());
   }

   return PhysicalProperties();
}

////////////////////////////////////////////////////////////
double SpiceEphemeris::VGetGravitationalParameterCentralBody(const std::string& name)
{
   try
   {
      return GetBodyProperty(m_observerBodyName, "GM");
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve gravitational parameter for "
         << Bracket(m_observerBodyName) << ": " << Bracket(ex.what());
   }

   return 0.0;
}

////////////////////////////////////////////////////////////
StateVector SpiceEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch)
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

   return StateVector(state, StateVectorType::Cartesian);
}

////////////////////////////////////////////////////////////
double SpiceEphemeris::CalculateEphemerisTime(const Epoch& epoch) const
{
   return (epoch.GetJD() - j2000_c()) * spd_c();
}

} // namespace otl

#endif