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

#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/Base.h>

namespace otl
{

////////////////////////////////////////////////////////////
PhysicalProperties::PhysicalProperties() :
m_mass(1.0),
m_radius(1.0),
m_safeRadius(1.1),
m_mu(ASTRO_GRAVITATIONAL_CONSTANT)
{

}

////////////////////////////////////////////////////////////
PhysicalProperties::PhysicalProperties(double mass, double radius) :
m_mass(mass),
m_radius(radius),
m_safeRadius(1.1 * radius),
m_mu(mass * ASTRO_GRAVITATIONAL_CONSTANT)
{

}

////////////////////////////////////////////////////////////
PhysicalProperties::PhysicalProperties(double mass, double radius, double safeRadius) :
m_mass(mass),
m_radius(radius),
m_safeRadius(safeRadius),
m_mu(mass * ASTRO_GRAVITATIONAL_CONSTANT)
{

}

////////////////////////////////////////////////////////////
double PhysicalProperties::GetMass() const
{
   return m_mass;
}

////////////////////////////////////////////////////////////
double PhysicalProperties::GetRadius() const
{
   return m_radius;
}

////////////////////////////////////////////////////////////
double PhysicalProperties::GetSafeRadius() const
{
   return m_safeRadius;
}

////////////////////////////////////////////////////////////
double PhysicalProperties::GetGravitationalParameter() const
{
   return m_mu;
}

////////////////////////////////////////////////////////////
std::string PhysicalProperties::ToString() const
{
   std::stringstream os;
   os << "m=" << m_mass << " r=" << m_radius << " sr=" << m_safeRadius << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
std::string PhysicalProperties::ToDetailedString(std::string prefix) const
{
   std::stringstream os;
   os << prefix << "Mass:                    " << m_mass       << std::endl;
   os << prefix << "Equatorial Radius:       " << m_radius     << std::endl;
   os << prefix << "Safe Radius:             " << m_safeRadius << std::endl;
   os << prefix << "Gravitational Parameter: " << m_mu         << std::endl;

   return os.str();
}

} // namespace otl