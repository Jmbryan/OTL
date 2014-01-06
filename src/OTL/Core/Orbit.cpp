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

#include <OTL/Core/Orbit.hpp>
#include <OTL/Core/PropagateAnalytical.hpp>

namespace otl
{

////////////////////////////////////////////////////////////
Orbit::Orbit() :
m_mu(0.0f),
m_radius(0.0f),
m_orbitType(Type::Invalid)
{
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu) :
m_mu(mu),
m_radius(0.0f),
m_orbitType(Type::Invalid)
{
}

////////////////////////////////////////////////////////////
Orbit::Orbit(const StateVector& stateVector, double mu) :
m_mu(mu),
m_radius(0.0f),
m_orbitType(Type::Invalid)
{
   SetStateVector(stateVector);
}

////////////////////////////////////////////////////////////
Orbit::Orbit(const OrbitalElements& orbitalElements, double mu) :
m_mu(mu),
m_radius(0.0f),
m_orbitType(Type::Invalid)
{
   SetOrbitalElements(orbitalElements);
}

////////////////////////////////////////////////////////////
Orbit::~Orbit()
{
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitType()
{
   double eccentricity = m_orbitalElements.eccentricity;
   
   if (eccentricity == ASTRO_ECC_CIRCULAR)
   {
      m_orbitType = Type::Circular;
   }
   else if (eccentricity > ASTRO_ECC_CIRCULAR &&
            eccentricity < ASTRO_ECC_PARABOLIC)
   {
      m_orbitType = Type::Elliptical;
   }
   else if (eccentricity == ASTRO_ECC_PARABOLIC)
   {
      m_orbitType = Type::Parabolic;
   }
   else if (eccentricity > ASTRO_ECC_PARABOLIC)
   {
      m_orbitType = Type::Hyperbolic;
   }
   else
   {
      m_orbitType = Type::Invalid;
   }
}

////////////////////////////////////////////////////////////
void Orbit::Propagate(double seconds)
{
   if (m_propagator)
   {
      m_propagator->Propagate(m_stateVector, m_mu, seconds);
      SetStateVector(m_stateVector);
   }
}

} // namespace otl