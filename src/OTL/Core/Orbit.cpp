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

#include <OTL/Core/Orbit.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
Orbit::Orbit() :
m_useStateVectorForStringOutput(false),
m_mu(0.0),
m_orbitRadius(0.0),
m_orbitType(Type::Invalid),
m_stateVectorDirty(false),
m_orbitalElementsDirty(false),
m_propagator(new KeplerianPropagator())
{
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu) :
m_useStateVectorForStringOutput(false),
m_mu(mu),
m_orbitRadius(0.0),
m_orbitType(Type::Invalid),
m_stateVectorDirty(false),
m_orbitalElementsDirty(false),
m_propagator(new KeplerianPropagator())
{
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu, const StateVector& stateVector) :
m_useStateVectorForStringOutput(false),
m_mu(mu),
m_orbitRadius(0.0),
m_orbitType(Type::Invalid),
m_stateVectorDirty(false),
m_orbitalElementsDirty(false),
m_propagator(new KeplerianPropagator())
{
   SetStateVector(stateVector);
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu, const OrbitalElements& orbitalElements) :
m_useStateVectorForStringOutput(false),
m_mu(mu),
m_orbitRadius(0.0),
m_orbitType(Type::Invalid),
m_stateVectorDirty(false),
m_orbitalElementsDirty(false),
m_propagator(new KeplerianPropagator())
{
   SetOrbitalElements(orbitalElements);
}

////////////////////////////////////////////////////////////
Orbit::~Orbit()
{
}

////////////////////////////////////////////////////////////
void Orbit::SetMu(double mu)
{
   m_mu = mu;
}

////////////////////////////////////////////////////////////
void Orbit::SetPosition(const Vector3d& position)
{
    if (m_stateVectorDirty)
    {
        UpdateStateVector();
    }

    m_stateVector.position = position;
    UpdateOrbitRadius();

    m_stateVectorDirty = false;
    m_orbitalElementsDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::SetVelocity(const Vector3d& velocity)
{
    if (m_stateVectorDirty)
    {
        UpdateStateVector();
    }

    m_stateVector.velocity = velocity;
    UpdateOrbitRadius();

    m_stateVectorDirty = false;
    m_orbitalElementsDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::SetStateVector(const StateVector& stateVector)
{
   m_stateVector = stateVector;
   UpdateOrbitRadius();

   m_stateVectorDirty = false;
   m_orbitalElementsDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::SetOrbitalElements(const OrbitalElements& orbitalElements)
{
   m_orbitalElements = orbitalElements;
   UpdateOrbitType();

   m_stateVectorDirty = true;
   m_orbitalElementsDirty = false; 
}

////////////////////////////////////////////////////////////
void Orbit::SetPropagator(const PropagatorPointer& propagator)
{
   m_propagator = propagator;
}

////////////////////////////////////////////////////////////
double Orbit::GetMu() const
{
   return m_mu;
}

////////////////////////////////////////////////////////////
double Orbit::GetOrbitRadius() const
{
   if (m_stateVectorDirty)
   {
      UpdateStateVector();
      UpdateOrbitRadius();
   }
   return m_orbitRadius;
}

////////////////////////////////////////////////////////////
Orbit::Type Orbit::GetOrbitType() const
{
   if (m_orbitalElementsDirty)
   {
      UpdateOrbitalElements();
      UpdateOrbitType();
   }
   return m_orbitType;
}

////////////////////////////////////////////////////////////
const Vector3d& Orbit::GetPosition() const
{
   return GetStateVector().position;
}

////////////////////////////////////////////////////////////
const Vector3d& Orbit::GetVelocity() const
{
   return GetStateVector().velocity;
}

////////////////////////////////////////////////////////////
const StateVector& Orbit::GetStateVector() const
{
   if (m_stateVectorDirty)
   {
      UpdateStateVector();
      UpdateOrbitRadius();
   }
   return m_stateVector;
}

////////////////////////////////////////////////////////////
const OrbitalElements& Orbit::GetOrbitalElements() const
{
   if (m_orbitalElementsDirty)
   {
      UpdateOrbitalElements();
      UpdateOrbitType();
   }
   return m_orbitalElements;
}

////////////////////////////////////////////////////////////
bool Orbit::IsType(Type orbitType) const
{
   return (GetOrbitType() == orbitType);
}

////////////////////////////////////////////////////////////
void Orbit::Propagate(const Time& timeDelta)
{
   if (m_propagator)
   {
      auto newStateVector = m_propagator->Propagate(m_stateVector, timeDelta, m_mu);
      SetStateVector(newStateVector);
   }
   else
   {
       OTL_ERROR() << "Propagator pointer invalid";
   }
}

////////////////////////////////////////////////////////////
void Orbit::PropagateToTrueAnomaly(double trueAnomaly, const Orbit::Direction& direction)
{
   OrbitalElements newOrbitalElements = m_orbitalElements;
   newOrbitalElements.trueAnomaly = trueAnomaly;
   SetOrbitalElements(newOrbitalElements);
}

////////////////////////////////////////////////////////////
void Orbit::UseStateVectorForStringOutput(bool useStateVectorForStringOutput)
{
   m_useStateVectorForStringOutput = useStateVectorForStringOutput;
}

////////////////////////////////////////////////////////////
std::string Orbit::ToString() const
{
   std::ostringstream os;
   os << "mu=" << m_mu << " "
      << "r=" << m_orbitRadius << " ";

   if (m_useStateVectorForStringOutput)
   {
      os << GetStateVector();
   }
   else
   {
      os << GetOrbitalElements();
   }

   return os.str();
}

////////////////////////////////////////////////////////////
std::string Orbit::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbit:" << std::endl;
   os << prefix << "   Gravitational Parameter: " << std::setprecision(6) << std::fixed << m_mu << std::endl;
   os << prefix << "   Radius:                  " << std::setprecision(6) << std::fixed << m_orbitRadius << std::endl;
   os << GetOrbitalElements().ToDetailedString(prefix + "   ");
   os << GetStateVector().ToDetailedString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void Orbit::UpdateStateVector() const
{
   m_stateVector = ConvertOrbitalElements2StateVector(m_orbitalElements, m_mu);
   m_stateVectorDirty = false;
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitalElements() const
{
   m_orbitalElements = ConvertStateVector2OrbitalElements(m_stateVector, m_mu);
   m_orbitalElementsDirty = false;
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitRadius() const
{
   m_orbitRadius = m_stateVector.position.norm();
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitType() const
{
   double eccentricity = m_orbitalElements.eccentricity;  
   
   if (eccentricity > ASTRO_ECC_CIRCULAR &&
       eccentricity < ASTRO_ECC_PARABOLIC)
   {
      m_orbitType = Type::Elliptical;
   }
   else if (eccentricity > ASTRO_ECC_PARABOLIC)
   {
      m_orbitType = Type::Hyperbolic;
   }
   else if (IsApprox(eccentricity, ASTRO_ECC_PARABOLIC))
   {
      m_orbitType = Type::Parabolic;
   }
   else if (IsApprox(eccentricity, ASTRO_ECC_CIRCULAR))
   {
      m_orbitType = Type::Circular;
   }
   else
   {
      OTL_WARN() << "Invalid orbit type with eccentricity " << Bracket(eccentricity);
      m_orbitType = Type::Invalid;
   }
}

} // namespace keplerian

} // namespace otl