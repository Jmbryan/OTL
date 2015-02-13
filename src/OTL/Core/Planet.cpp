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

#include <OTL/Core/Planet.h>

namespace otl
{

typedef std::map<PlanetId, std::pair<std::string, PhysicalProperties>> PlanetDictionary;
static PlanetDictionary g_planetInfo =
{
   { PlanetId::Mercury,  std::make_pair("Mercury",  PhysicalProperties(ASTRO_MU_MERCURY / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_MERCURY)) },
   { PlanetId::Venus,    std::make_pair("Venus",    PhysicalProperties(ASTRO_MU_VENUS   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_VENUS))   },
   { PlanetId::Earth,    std::make_pair("Earth",    PhysicalProperties(ASTRO_MU_EARTH   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_EARTH))   },
   { PlanetId::Mars,     std::make_pair("Mars",     PhysicalProperties(ASTRO_MU_MARS    / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_MARS))    },
   { PlanetId::Jupiter,  std::make_pair("Jupiter",  PhysicalProperties(ASTRO_MU_JUPITER / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_JUPITER)) },
   { PlanetId::Saturn,   std::make_pair("Saturn",   PhysicalProperties(ASTRO_MU_SATURN  / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_SATURN))  },
   { PlanetId::Uranus,   std::make_pair("Uranus",   PhysicalProperties(ASTRO_MU_URANUS  / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_URANUS))  },
   { PlanetId::Neptune,  std::make_pair("Neptune",  PhysicalProperties(ASTRO_MU_NEPTUNE / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_NEPTUNE)) },
   { PlanetId::Pluto,    std::make_pair("Pluto",    PhysicalProperties(ASTRO_MU_PLUTO   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_PLUTO))   }
};


////////////////////////////////////////////////////////////
std::string ConvertPlanetIdentifier2Name(PlanetId planetId)
{
   PlanetDictionary::const_iterator it = g_planetInfo.find(planetId);
   if (it == g_planetInfo.end())
   {
      OTL_ERROR() << "Planet name not found for id " << Bracket(planetId);
      return std::string();
   }
   return it->second.first;
}

////////////////////////////////////////////////////////////
PlanetId ConvertPlanetName2Identifier(const std::string& name)
{
   PlanetId planetId = PlanetId::Invalid;
   for (auto it = g_planetInfo.begin(); it != g_planetInfo.end(); ++it)
   {
      if ((it->second).first == name)
      {
         planetId = it->first;
         break;
      }
   }
   OTL_ERROR_IF(planetId == PlanetId::Invalid, "Planet name " << Bracket(name) << " not found");

   return planetId;
}

////////////////////////////////////////////////////////////
PhysicalProperties GetPlanetPhysicalProperties(const PlanetId& planetId)
{
   const auto it = g_planetInfo.find(planetId);
   if (it == g_planetInfo.end())
   {
      OTL_ERROR() << "Invalid planet id " << Bracket(planetId);
      return PhysicalProperties();
   }
   return (it->second).second;
}

////////////////////////////////////////////////////////////
PhysicalProperties GetPlanetPhysicalProperties(const std::string& planetName)
{
   const auto& planetId = ConvertPlanetName2Identifier(planetName);
   return GetPlanetPhysicalProperties(planetId);
}

} // namespace otl