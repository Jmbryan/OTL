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

#include <OTL/Core/Planet.hpp>

namespace otl
{

const Planet::PlanetDictionary Planet::m_planetInfo = Planet::CreatePlanetInfo();

////////////////////////////////////////////////////////////
void Planet::Initialize(Planet::PlanetId planetId)
{
   assert(planetId > PlanetId::Invalid && planetId < PlanetId::Count);
   
   const PlanetInfo& planetInfo = m_planetInfo.at(planetId);

   SetName(planetInfo.name);
   SetRadius(planetInfo.radius);
   SetMu(planetInfo.mu);
}

////////////////////////////////////////////////////////////
Planet::PlanetId Planet::ConvertName2Identifier(const std::string& name)
{
   PlanetId planetId = PlanetId::Invalid;

   for (auto it = m_planetInfo.begin(); it != m_planetInfo.end(); ++it)
   {
      if (it->second.name.compare(name) == 0.0)
      {
         planetId = it->first;
         break;
      }
   }

   return planetId;
}

////////////////////////////////////////////////////////////
Planet::PlanetDictionary Planet::CreatePlanetInfo()
{
   PlanetDictionary planetInfo;

   //         Planet                            Name          Radius                 Gravitational Parameter    
   planetInfo[PlanetId::Mercury]   = PlanetInfo("Mercury",    ASTRO_RADIUS_MERCURY,  ASTRO_MU_MERCURY);
   planetInfo[PlanetId::Venus]     = PlanetInfo("Venus",      ASTRO_RADIUS_VENUS,    ASTRO_MU_VENUS);
   planetInfo[PlanetId::Earth]     = PlanetInfo("Earth",      ASTRO_RADIUS_EARTH,    ASTRO_MU_EARTH);
   planetInfo[PlanetId::Mars]      = PlanetInfo("Mars",       ASTRO_RADIUS_MARS,     ASTRO_MU_MARS);
   planetInfo[PlanetId::Jupiter]   = PlanetInfo("Jupiter",    ASTRO_RADIUS_JUPITER,  ASTRO_MU_JUPITER);
   planetInfo[PlanetId::Saturn]    = PlanetInfo("Saturn",     ASTRO_RADIUS_SATURN,   ASTRO_MU_SATURN);
   planetInfo[PlanetId::Uranus]    = PlanetInfo("Uranus",     ASTRO_RADIUS_URANUS,   ASTRO_MU_URANUS);
   planetInfo[PlanetId::Neptune]   = PlanetInfo("Neptune",    ASTRO_RADIUS_NEPTUNE,  ASTRO_MU_NEPTUNE);
   planetInfo[PlanetId::Pluto]     = PlanetInfo("Pluto",      ASTRO_RADIUS_PLUTO,    ASTRO_MU_PLUTO);

   return planetInfo;
}

} // namespace otl