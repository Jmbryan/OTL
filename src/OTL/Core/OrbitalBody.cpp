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

#include <OTL/Core/OrbitalBody.h>

namespace otl
{

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody() :
m_name(""),
m_orbit(1.0 * ASTRO_GRAVITATIONAL_CONSTANT)
{
}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name, double mass) :
m_name(name),
m_orbit(mass * ASTRO_GRAVITATIONAL_CONSTANT)
{
}

////////////////////////////////////////////////////////////
OrbitalBody::~OrbitalBody()
{

}

////////////////////////////////////////////////////////////
void OrbitalBody::SetName(const std::string& name)
{
   m_name = name;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetMass(double mass)
{
   m_orbit.SetMu(mass * ASTRO_GRAVITATIONAL_CONSTANT);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetMu(double mu)
{
   m_orbit.SetMu(mu);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetPosition(const Vector3d& position)
{
    m_orbit.SetPosition(position);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetVelocity(const Vector3d& velocity)
{
    m_orbit.SetVelocity(velocity);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetStateVector(const StateVector& stateVector)
{
    m_orbit.SetStateVector(stateVector);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetOrbitalElements(const OrbitalElements& orbitalElements)
{
    m_orbit.SetOrbitalElements(orbitalElements);
}

////////////////////////////////////////////////////////////
const std::string& OrbitalBody::GetName() const
{
   return m_name;
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetMass() const
{
   return m_orbit.GetMu() / ASTRO_GRAVITATIONAL_CONSTANT;
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetMu() const
{
   return m_orbit.GetMu();
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetOrbitRadius() const
{
   return m_orbit.GetOrbitRadius();
}

////////////////////////////////////////////////////////////
const Vector3d& OrbitalBody::GetPosition() const
{
   return m_orbit.GetStateVector().position;
}

////////////////////////////////////////////////////////////
const Vector3d& OrbitalBody::GetVelocity() const
{
   return m_orbit.GetStateVector().velocity;
}

////////////////////////////////////////////////////////////
const StateVector& OrbitalBody::GetStateVector() const
{
   return m_orbit.GetStateVector();
}

////////////////////////////////////////////////////////////
const OrbitalElements& OrbitalBody::GetOrbitalElements() const
{
   return m_orbit.GetOrbitalElements();
}

////////////////////////////////////////////////////////////
const keplerian::Orbit& OrbitalBody::GetOrbit() const
{
   return m_orbit;
}

////////////////////////////////////////////////////////////
void OrbitalBody::Propagate(const Time& timeDelta)
{
   m_orbit.Propagate(timeDelta);
}

////////////////////////////////////////////////////////////
std::string OrbitalBody::ToString() const
{
   std::ostringstream os;
   os << "name=" << m_name << " " << m_orbit;

   return os.str();
}

////////////////////////////////////////////////////////////
std::string OrbitalBody::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Name: " << GetName() << std::endl;
   os << prefix << "Orbit:" << std::endl;
   os << m_orbit.ToDetailedString(prefix + "   ");

   return os.str();
}

} // namespace otl