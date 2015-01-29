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

const Planet::PlanetDictionary Planet::m_planetInfo = Planet::CreatePlanetInfo();

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
NaturalBody(),
m_id(PlanetId::Invalid)
{
}

////////////////////////////////////////////////////////////
Planet::Planet(Planet::PlanetId planetId) :
NaturalBody(),
m_id(planetId)
{
   Initialize(planetId);
}

////////////////////////////////////////////////////////////
Planet::Planet(const std::string& name) :
NaturalBody(),
m_id(PlanetId::Invalid)
{
   m_id = ConvertName2Identifier(name);
   Initialize(m_id);
}

////////////////////////////////////////////////////////////
Planet::PlanetId Planet::ConvertName2Identifier(const std::string& name)
{
    PlanetId planetId = PlanetId::Invalid;
    for (auto it = m_planetInfo.begin(); it != m_planetInfo.end(); ++it)
    {
        if (it->second.name == name)
        {
            planetId = it->first;
            break;
        }
    }
    if (planetId == PlanetId::Invalid)
    {
        OTL_ERROR() << "Planet name [" << name << "] not found";
    }
    return planetId;
}

////////////////////////////////////////////////////////////
std::string Planet::ConvertIdentifier2Name(PlanetId planetId)
{
    PlanetDictionary::const_iterator it = m_planetInfo.find(planetId);
    if (it == m_planetInfo.end())
    {
        OTL_ERROR() << "Planet name not found for id [" << planetId << "]";
    }
    return it->second.name;
}

std::string Planet::ToString() const
{
   std::ostringstream os;
   os << "name=" << GetName() << " id=" << m_id << " epoch=" << GetEpoch();

   return os.str();
}

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
void Planet::Initialize(Planet::PlanetId planetId)
{
   PlanetDictionary::const_iterator it = m_planetInfo.find(planetId);
   OTL_ERROR_IF(it == m_planetInfo.end(), "Invalid planet ID " << Bracket(planetId));

   const PlanetInfo& planetInfo = it->second;

   SetName(planetInfo.name);
   SetMu(ASTRO_MU_SUN);
   SetPhysicalProperties(NaturalBody::PhysicalProperties(
      planetInfo.mu / ASTRO_GRAVITATIONAL_CONSTANT,
      planetInfo.mu,
      planetInfo.radius));


   // Initialize ephemeris
   UseEphemerisForPropagation(true);
   SetEphemeris(EphemerisPointer(new JplApproximateEphemeris()));
}

////////////////////////////////////////////////////////////
Planet::PlanetDictionary Planet::CreatePlanetInfo()
{
   PlanetDictionary planetInfo;

   //         Planet Id                         Name          Equatorial Radius      Gravitational Parameter    
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