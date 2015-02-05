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

//const Planet::PlanetDictionary Planet::m_planetInfo = Planet::CreatePlanetInfo();

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
Planet::PlanetInfo::PlanetInfo() :
name(""),
radius(0.0),
mu(0.0)
{
}

////////////////////////////////////////////////////////////
Planet::PlanetInfo::PlanetInfo(const std::string& Name, double Radius, double Mu) :
name(Name),
radius(Radius),
mu(Mu)
{
}



////////////////////////////////////////////////////////////
Planet::Planet() :
//OrbitalBody("Earth", GetPlanetPhysicalProperties("Earth"), ASTRO_MU_SUN, OrbitalElements()),
OrbitalBody("Earth", PhysicalProperties(), 1.0, OrbitalElements()),
m_id(PlanetId::Earth)
{

}

////////////////////////////////////////////////////////////
Planet::Planet(Planet::PlanetId planetId, const Epoch& epoch) :
//OrbitalBody(ConvertPlanetIdentifier2Name(planetId), GetPlanetPhysicalProperties(planetId), ASTRO_MU_SUN, OrbitalElements(), epoch),
OrbitalBody(ConvertPlanetIdentifier2Name(planetId), PhysicalProperties(), 1.0, OrbitalElements(), epoch),
m_id(planetId)
{
   Initialize(m_id);
}

////////////////////////////////////////////////////////////
Planet::Planet(const std::string& name, const Epoch& epoch) :
//OrbitalBody(name, GetPlanetPhysicalProperties(name), ASTRO_MU_SUN, OrbitalElements(), epoch),
OrbitalBody(name, PhysicalProperties(), 1.0, OrbitalElements(), epoch),
m_id(ConvertPlanetName2Identifier(name))
{
   Initialize(m_id);
}

////////////////////////////////////////////////////////////
std::string Planet::ToString() const
{
   std::ostringstream os;
   os << "name=" << GetName() << " id=" << m_id << " epoch=" << GetEpoch();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string Planet::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Name: " << GetName() << std::endl;
   os << prefix << "Id:   " << m_id << std::endl;
   os << prefix << "Physical Properties:" << std::endl;
   os << GetPhysicalProperties().ToDetailedString(prefix + "   ");
   os << prefix << "Epoch:" << std::endl;
   os << GetEpoch().ToDetailedString(prefix + "   ");
   os << prefix << "Orbit:" << std::endl;
   os << GetOrbit().ToDetailedString(prefix + "   ");

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
void Planet::Initialize(Planet::PlanetId planetId)
{
   // Initialize ephemeris
   SetEphemeris(std::make_shared<JplApproximateEphemeris>());

   // Queue up an ephemeris query for the physical properties and orbital elements
   QueryPhysicalProperties();
   QueryCentralBodyMu();
   QueryOrbitalElements(GetEpoch());
}

////////////////////////////////////////////////////////////
//Planet::PlanetDictionary Planet::CreatePlanetInfo()
//{
//   PlanetDictionary planetInfo;
//
//   ////         Planet Id                         Name          Equatorial Radius      Gravitational Parameter    
//   //planetInfo[PlanetId::Mercury]   = PlanetInfo("Mercury",    ASTRO_RADIUS_MERCURY,  ASTRO_MU_MERCURY);
//   //planetInfo[PlanetId::Venus]     = PlanetInfo("Venus",      ASTRO_RADIUS_VENUS,    ASTRO_MU_VENUS);
//   //planetInfo[PlanetId::Earth]     = PlanetInfo("Earth",      ASTRO_RADIUS_EARTH,    ASTRO_MU_EARTH);
//   //planetInfo[PlanetId::Mars]      = PlanetInfo("Mars",       ASTRO_RADIUS_MARS,     ASTRO_MU_MARS);
//   //planetInfo[PlanetId::Jupiter]   = PlanetInfo("Jupiter",    ASTRO_RADIUS_JUPITER,  ASTRO_MU_JUPITER);
//   //planetInfo[PlanetId::Saturn]    = PlanetInfo("Saturn",     ASTRO_RADIUS_SATURN,   ASTRO_MU_SATURN);
//   //planetInfo[PlanetId::Uranus]    = PlanetInfo("Uranus",     ASTRO_RADIUS_URANUS,   ASTRO_MU_URANUS);
//   //planetInfo[PlanetId::Neptune]   = PlanetInfo("Neptune",    ASTRO_RADIUS_NEPTUNE,  ASTRO_MU_NEPTUNE);
//   //planetInfo[PlanetId::Pluto]     = PlanetInfo("Pluto",      ASTRO_RADIUS_PLUTO,    ASTRO_MU_PLUTO);
//
//   //         Planet Id                           Name                            Equatorial Radius     Gravitational Parameter
//   planetInfo[PlanetId::Mercury] = std::make_pair("Mercury",   PhysicalProperties(ASTRO_RADIUS_MERCURY, ASTRO_MU_MERCURY));
//   planetInfo[PlanetId::Venus]   = std::make_pair("Venus",     PhysicalProperties(ASTRO_RADIUS_VENUS,   ASTRO_MU_VENUS));
//   planetInfo[PlanetId::Earth]   = std::make_pair("Earth",     PhysicalProperties(ASTRO_RADIUS_EARTH,   ASTRO_MU_EARTH));
//   planetInfo[PlanetId::Mars]    = std::make_pair("Mars",      PhysicalProperties(ASTRO_RADIUS_MARS,    ASTRO_MU_MARS));
//   planetInfo[PlanetId::Jupiter] = std::make_pair("Jupiter",   PhysicalProperties(ASTRO_RADIUS_JUPITER, ASTRO_MU_JUPITER));
//   planetInfo[PlanetId::Saturn]  = std::make_pair("Saturn",    PhysicalProperties(ASTRO_RADIUS_SATURN,  ASTRO_MU_SATURN));
//   planetInfo[PlanetId::Uranus]  = std::make_pair("Uranus",    PhysicalProperties(ASTRO_RADIUS_URANUS,  ASTRO_MU_URANUS));
//   planetInfo[PlanetId::Neptune] = std::make_pair("Neptune",   PhysicalProperties(ASTRO_RADIUS_NEPTUNE, ASTRO_MU_NEPTUNE));
//   planetInfo[PlanetId::Pluto]   = std::make_pair("Pluto",     PhysicalProperties(ASTRO_RADIUS_PLUTO,   ASTRO_MU_PLUTO));
//
//   return planetInfo;
//}

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

PhysicalProperties GetPlanetPhysicalProperties(const std::string& planetName)
{
   const auto& planetId = ConvertPlanetName2Identifier(planetName);
   return GetPlanetPhysicalProperties(planetId);
}


} // namespace otl