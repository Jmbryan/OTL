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
#include <OTL/Core/JplApproximateEphemeris.h>
#include <OTL/Core/Logger.h>

namespace otl
{

typedef std::map<Planet::PlanetId, std::pair<std::string, PhysicalProperties>> PlanetDictionary;
static PlanetDictionary g_planetInfo =
{
   { Planet::PlanetId::Mercury,  std::make_pair("Mercury",  PhysicalProperties(ASTRO_MU_MERCURY / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_MERCURY)) },
   { Planet::PlanetId::Venus,    std::make_pair("Venus",    PhysicalProperties(ASTRO_MU_VENUS   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_VENUS))   },
   { Planet::PlanetId::Earth,    std::make_pair("Earth",    PhysicalProperties(ASTRO_MU_EARTH   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_EARTH))   },
   { Planet::PlanetId::Mars,     std::make_pair("Mars",     PhysicalProperties(ASTRO_MU_MARS    / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_MARS))    },
   { Planet::PlanetId::Jupiter,  std::make_pair("Jupiter",  PhysicalProperties(ASTRO_MU_JUPITER / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_JUPITER)) },
   { Planet::PlanetId::Saturn,   std::make_pair("Saturn",   PhysicalProperties(ASTRO_MU_SATURN  / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_SATURN))  },
   { Planet::PlanetId::Uranus,   std::make_pair("Uranus",   PhysicalProperties(ASTRO_MU_URANUS  / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_URANUS))  },
   { Planet::PlanetId::Neptune,  std::make_pair("Neptune",  PhysicalProperties(ASTRO_MU_NEPTUNE / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_NEPTUNE)) },
   { Planet::PlanetId::Pluto,    std::make_pair("Pluto",    PhysicalProperties(ASTRO_MU_PLUTO   / ASTRO_GRAVITATIONAL_CONSTANT, ASTRO_RADIUS_PLUTO))   }
};

////////////////////////////////////////////////////////////
Planet::Planet() :
OrbitalBody(),
m_id(PlanetId::Invalid)
{

}

////////////////////////////////////////////////////////////
Planet::Planet(Planet::PlanetId planetId, const Epoch& epoch) :
OrbitalBody(ConvertPlanetIdentifier2Name(planetId), PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_id(planetId)
{
   Initialize(m_id, std::make_shared<JplApproximateEphemeris>());
}

////////////////////////////////////////////////////////////
Planet::Planet(const std::string& name, const Epoch& epoch) :
OrbitalBody(name, PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_id(ConvertPlanetName2Identifier(name))
{
   Initialize(m_id, std::make_shared<JplApproximateEphemeris>());
}

////////////////////////////////////////////////////////////
Planet::Planet(PlanetId planetId, const EphemerisPointer& ephemeris, const Epoch& epoch) :
OrbitalBody(ConvertPlanetIdentifier2Name(planetId), PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_id(planetId)
{
   Initialize(m_id, ephemeris);
}

////////////////////////////////////////////////////////////
Planet::Planet(const std::string& name, const EphemerisPointer& ephemeris, const Epoch& epoch) :
OrbitalBody(name, PhysicalProperties(), 1.0, test::StateVector(OrbitalElements()), epoch),
m_id(ConvertPlanetName2Identifier(name))
{
   Initialize(m_id, ephemeris);
}

////////////////////////////////////////////////////////////
std::string Planet::ToString() const
{
   std::ostringstream os;
   os << "id=" << m_id << " name=" << GetName() << " epoch=" << GetEpoch();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string Planet::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Identifier: " << m_id << std::endl;
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToDetailedString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void Planet::VQueryPhysicalProperties()
{
   const auto& physicalProperties = GetPlanetPhysicalProperties(GetName());
   SetPhysicalProperties(physicalProperties);
}

////////////////////////////////////////////////////////////
void Planet::VQueryCentralBodyMu()
{
   SetGravitationalParameterCentralBody(ASTRO_MU_SUN);
}

////////////////////////////////////////////////////////////
void Planet::Initialize(Planet::PlanetId planetId, const EphemerisPointer& ephemeris)
{
   // Initialize ephemeris
   SetEphemeris(ephemeris);

   // Lazily query the planet data from the ephemeris database
   LazyQueryPhysicalProperties();
   LazyQueryCentralBodyMu();
   LazyQueryStateVector(GetEpoch());
}

////////////////////////////////////////////////////////////
std::string ConvertPlanetIdentifier2Name(Planet::PlanetId planetId)
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
Planet::PlanetId ConvertPlanetName2Identifier(const std::string& name)
{
   Planet::PlanetId planetId = Planet::PlanetId::Invalid;
   for (auto it = g_planetInfo.begin(); it != g_planetInfo.end(); ++it)
   {
      if ((it->second).first == name)
      {
         planetId = it->first;
         break;
      }
   }
   if (planetId == Planet::PlanetId::Invalid)
   {
      OTL_ERROR() << "Planet name " << Bracket(name) << " not found";
   }
   return planetId;
}

////////////////////////////////////////////////////////////
PhysicalProperties GetPlanetPhysicalProperties(const Planet::PlanetId& planetId)
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