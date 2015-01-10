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

#include <OTL/Core/JplEphemeris.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>
#include <niek-ephem/DE405Ephemeris.h>

namespace otl
{

// Static globals
typedef std::unique_ptr<DE405Ephemeris> EphemerisDatabasePointer;
static EphemerisDatabasePointer g_ephemerisDatabase;

typedef std::map<std::string, DE405Ephemeris::AstroEntity> AstroEntityDictionary;
static AstroEntityDictionary g_entityDictionary;

////////////////////////////////////////////////////////////
JplEphemeris::JplEphemeris(const std::string& dataFilename) :
IEphemeris(),
m_dataFilename(dataFilename)
{
    
}

////////////////////////////////////////////////////////////
JplEphemeris::~JplEphemeris()
{

}

////////////////////////////////////////////////////////////
void JplEphemeris::SetDataFile(const std::string& dataFilename)
{
   m_dataFilename = dataFilename;
}

////////////////////////////////////////////////////////////
void JplEphemeris::LoadDataFile(const std::string& dataFilename)
{
   SetDataFile(dataFilename);
   VLoad();
}

////////////////////////////////////////////////////////////
void JplEphemeris::VLoad()
{
   try
   {
      g_ephemerisDatabase = std::make_unique<DE405Ephemeris>(m_dataFilename); // Loads file in constructor
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught while trying to load ephemeris datafile " <<
         Bracket(m_dataFilename) << ": " << Bracket(ex.what());
   } 
}

////////////////////////////////////////////////////////////
void JplEphemeris::VInitialize()
{
   g_entityDictionary["Mercury"] = DE405Ephemeris::Mercury;
   g_entityDictionary["Venus"] = DE405Ephemeris::Venus;
   g_entityDictionary["Earth"] = DE405Ephemeris::EarthMoonBarycenter;
   g_entityDictionary["Mars"] = DE405Ephemeris::Mars;
   g_entityDictionary["Jupiter"] = DE405Ephemeris::JupiterBarycenter;
   g_entityDictionary["Saturn"] = DE405Ephemeris::SaturnBarycenter;
   g_entityDictionary["Uranus"] = DE405Ephemeris::UranusBarycenter;
   g_entityDictionary["Neptune"] = DE405Ephemeris::NeptuneBarycenter;
   g_entityDictionary["Pluto"] = DE405Ephemeris::PlutoBarycenter;
   g_entityDictionary["Sun"] = DE405Ephemeris::Sun;
   g_entityDictionary["Moon"] = DE405Ephemeris::Moon;
}

////////////////////////////////////////////////////////////
bool JplEphemeris::VIsValidName(const std::string& name)
{
   AstroEntityDictionary::const_iterator it = g_entityDictionary.find(name);
   return (it != g_entityDictionary.end());
}

////////////////////////////////////////////////////////////
bool JplEphemeris::VIsValidEpoch(const Epoch& epoch)
{
    int year = epoch.GetGregorian().year;
    return (year >= 1800 && year <= 2200);
}

////////////////////////////////////////////////////////////
void JplEphemeris::VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   auto entity = g_entityDictionary[name];

   double pos[3];
   try
   {
      g_ephemerisDatabase->getPosition(epoch.GetJD(), entity, pos);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve position vector for " << Bracket(name) <<
         " at epoch " << Bracket(epoch) << ": " << Bracket(ex.what());
   }

   for (int i = 0; i < 3; ++i)
   {
      position[i] = pos[i];
   }
}

////////////////////////////////////////////////////////////
void JplEphemeris::VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   // Query the state vector at the given epoch
   StateVector stateVector;
   VGetStateVector(name, epoch, stateVector);

   // Return the velocity vector
   velocity = stateVector.velocity;
}

////////////////////////////////////////////////////////////
void JplEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   auto entity = g_entityDictionary[name];

   double pos[3], vel[3];
   try
   {
      g_ephemerisDatabase->getPosVel(epoch.GetJD(), entity, pos, vel);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve state vector for " << Bracket(name) <<
         " at epoch " << Bracket(epoch) << ": " << Bracket(ex.what());
   } 

   for (int i = 0; i < 3; ++i)
   {
      stateVector.position[i] = pos[i];
      stateVector.velocity[i] = vel[i] / MATH_DAY_TO_SEC;
   }
}

////////////////////////////////////////////////////////////
void JplEphemeris::VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
    // Query the state vector at the given epoch
    StateVector stateVector;
    VGetStateVector(name, epoch, stateVector);

    // Convert state vector to orbital elements
    ConvertStateVector2OrbitalElements(stateVector, orbitalElements, ASTRO_MU_SUN);
}

} // namespace otl