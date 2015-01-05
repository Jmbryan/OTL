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
#include <niek-ephem/DE405Ephemeris.h>

namespace otl
{

// Static globals
typedef DE405Ephemeris EphemerisDatabase;
typedef std::unique_ptr<EphemerisDatabase> EphemerisDatabasePointer;
static EphemerisDatabasePointer g_ephemerisDatabase;

typedef std::map<std::string, DE405Ephemeris::AstroEntity> AstroEntityDictionary;
static AstroEntityDictionary g_entityDictionary;

////////////////////////////////////////////////////////////
JplEphemeris::JplEphemeris() :
IEphemeris()
{
    
}

////////////////////////////////////////////////////////////
JplEphemeris::~JplEphemeris()
{

}

void JplEphemeris::VLoad()
{
    g_ephemerisDatabase.reset(new DE405Ephemeris("E:/Dev/OTL/data/jpl_eph/de405/de405.data"));
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
bool JplEphemeris::VIsNameValid(const std::string& name)
{
   std::map<std::string, DE405Ephemeris::AstroEntity>::const_iterator it = g_entityDictionary.find(name);
   return (it != g_entityDictionary.end());
}

////////////////////////////////////////////////////////////
bool JplEphemeris::VIsEpochValid(const Epoch& epoch)
{
    int year = epoch.GetGregorian().year;
    return (year >= 1800 && year <= 2200);
}

////////////////////////////////////////////////////////////
void JplEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   DE405Ephemeris::AstroEntity entity = g_entityDictionary[name];

   double pos[3], vel[3];
   g_ephemerisDatabase->getPosVel(epoch.GetJD(), entity, pos, vel);

   for (int i = 0; i < 3; ++i)
   {
      stateVector.position[i] = pos[i];
      stateVector.velocity[i] = vel[i] / MATH_DAY_TO_SEC;
   }
}

////////////////////////////////////////////////////////////
void JplEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
    // Query the state vector at the given epoch
    StateVector stateVector;
    VQueryDatabase(name, epoch, stateVector);

    // Convert state vector to orbital elements
    ConvertStateVector2OrbitalElements(stateVector, orbitalElements, ASTRO_MU_SUN);
}

} // namespace otl