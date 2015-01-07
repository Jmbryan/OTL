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
SpiceEphemeris::SpiceEphemeris(const std::string& dataFile) :
IEphemeris(),
m_dataFile(dataFile)
{

}

////////////////////////////////////////////////////////////
SpiceEphemeris::~SpiceEphemeris()
{

}

////////////////////////////////////////////////////////////
void SpiceEphemeris::SetDataFile(const std::string& dataFile)
{
   m_dataFile = dataFile;
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
void SpiceEphemeris::VLoad()
{
   try
   {
      furnsh_c(m_dataFile.c_str());
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught when trying to create ephemeris database using datafile [" << m_dataFile << "]: [" << ex.what() << "].";
   }  
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VInitialize()
{
   g_bodyDictionary["Mercury"] = "Mercury";
   g_bodyDictionary["Earth"] = "Earth";

   m_referenceFrame = "GSE";
   m_abberationCorrections = "NONE";
   m_observerBody = "Earth";
}

////////////////////////////////////////////////////////////
bool SpiceEphemeris::VIsNameValid(const std::string& name)
{
   return true;
   BodyDictionary::const_iterator it = g_bodyDictionary.find(name);
   return (it != g_bodyDictionary.end());
}

////////////////////////////////////////////////////////////
bool SpiceEphemeris::VIsEpochValid(const Epoch& epoch)
{
   return true;
}

////////////////////////////////////////////////////////////
void SpiceEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   std::string targetBody = g_bodyDictionary[name];
   double ephemerisTime = (epoch.GetJD() - j2000_c()) * spd_c();
   double state[6];
   double lightTime;

   spkezr_c(name.c_str(),
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
void SpiceEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   // Query the state vector at the given epoch
   StateVector stateVector;
   VQueryDatabase(name, epoch, stateVector);

   // Convert state vector to orbital elements
   ConvertStateVector2OrbitalElements(stateVector, orbitalElements, ASTRO_MU_SUN);
}

} // namespace otl

#endif