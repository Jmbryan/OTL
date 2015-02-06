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
#include <OTL/Core/Logger.h>
extern "C"
{
#include  "cspice/SpiceUsr.h"
}

namespace otl
{

// Static globals
typedef std::map<std::string, std::string> BodyDictionary;
static BodyDictionary g_bodyDictionary;

////////////////////////////////////////////////////////////
SpiceEphemeris::SpiceEphemeris(const std::string& dataFilename) :
IEphemeris(),
m_dataFilename(dataFilename)
{

}

////////////////////////////////////////////////////////////
SpiceEphemeris::~SpiceEphemeris()
{

}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetDataFile(const std::string& dataFilename)
{
   m_dataFilename = dataFilename;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetReferenceFrame(const std::string& referenceFrame)
{
   m_referenceFrame = referenceFrame;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetAbberationCorrections(const std::string& abberationCorrections)
{
   m_abberationCorrections = abberationCorrections;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetObserverBody(const std::string& observerBody)
{
   m_observerBody = observerBody;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::LoadDataFile(const std::string& dataFilename)
{
   SetDataFile(dataFilename);
   VLoad();
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VLoad()
{
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
   g_bodyDictionary["Mercury"] = "MERCURY";
   g_bodyDictionary["Earth"] = "EARTH";

   m_referenceFrame = "J2000"; // "ECLIPJ2000" tudat
   m_abberationCorrections = "NONE";
   m_observerBody = "SUN";

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
   return true;
   BodyDictionary::const_iterator it = g_bodyDictionary.find(name);
   return (it != g_bodyDictionary.end());
}

////////////////////////////////////////////////////////////
bool SpiceEphemeris::VIsValidEpoch(const Epoch& epoch)
{
   return true;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   std::string targetBody = g_bodyDictionary[name];
   double ephemerisTime = CalculateEphemerisTime(epoch);
   double pos[3];
   double lightTime;

   spkpos_c(targetBody.c_str(),
            ephemerisTime,
            m_referenceFrame.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBody.c_str(),
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
   std::string targetBody = g_bodyDictionary[name];
   double ephemerisTime = CalculateEphemerisTime(epoch);
   double state[6];
   double lightTime;

   spkezr_c(targetBody.c_str(),
            ephemerisTime,
            m_referenceFrame.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBody.c_str(),
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
   orbitalElements = ConvertStateVector2OrbitalElements(stateVector, ASTRO_MU_SUN);
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector)
{
   std::string targetBody = g_bodyDictionary[name];
   double ephemerisTime = CalculateEphemerisTime(epoch);
   double state[6];
   double lightTime;

   spkezr_c(targetBody.c_str(),
            ephemerisTime,
            m_referenceFrame.c_str(),
            m_abberationCorrections.c_str(),
            m_observerBody.c_str(),
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