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
m_elapsedPropagationTime(),
m_propagator(new KeplerianPropagator()),
m_orbitRadius(0.0),
m_orbitType(Type::Invalid),
m_orbitalElements(),
m_referenceOrbitalElements(),
m_stateVector(),
m_referenceStateVector(),
m_orbitalElementsDirty(false),
m_referenceOrbitalElementsDirty(false),
m_stateVectorDirty(false),
m_referenceStateVectorDirty(false)
{
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu) :
Orbit()
{
   SetMu(mu);
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu, const StateVector& stateVector) :
Orbit(mu)
{
   SetStateVector(stateVector);
}

////////////////////////////////////////////////////////////
Orbit::Orbit(double mu, const OrbitalElements& orbitalElements) :
Orbit(mu)
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
void Orbit::SetStateVector(const StateVector& stateVector)
{
   m_stateVector = stateVector;
   m_referenceStateVector = stateVector;
   UpdateOrbitRadius();

   // Reset elapsed propagation time
   m_elapsedPropagationTime = Time();

   // State vectors are clean
   m_stateVectorDirty = false;
   m_referenceStateVectorDirty = false;

   // Orbital elements are dirty
   m_orbitalElementsDirty = true;
   m_referenceOrbitalElementsDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::SetOrbitalElements(const OrbitalElements& orbitalElements)
{
   m_orbitalElements = orbitalElements;
   m_referenceOrbitalElements = orbitalElements;  
   UpdateOrbitType();

   // Reset elapsed propagation time
   m_elapsedPropagationTime = Time();

   // Orbital elements are clean
   m_orbitalElementsDirty = false;
   m_referenceOrbitalElementsDirty = false;

   // State vectors are dirty
   m_stateVectorDirty = true;
   m_referenceOrbitalElementsDirty = true;   
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
void Orbit::Propagate(const Time& timeDelta, const PropagationType& propagationType)
{
   if (m_propagator)
   {
      switch (propagationType)
      {
      case PropagationType::OrbitalElements:
         PropagateOrbitalElements(timeDelta);
         break;

      case PropagationType::StateVector:
         PropagateStateVector(timeDelta);
         break;

      default:
         OTL_ERROR() << "Failed to propagate orbit. Invalid propagation type";
         break;
      }
   }
   else
   {
       OTL_ERROR() << "Failed to propagate orbit. Invalid propagator pointer";
   }
}

void Orbit::PropagateOrbitalElements(const Time& timeDelta)
{
   // Update the elapsed time since the reference orbital elements
   m_elapsedPropagationTime += timeDelta;

   // Update the reference orbital elements if they are out-of-date
   if (m_referenceOrbitalElementsDirty)
   {
      UpdateReferenceOrbitalElements();
   }

   // Propagate from the reference orbital elements and update orbit type
   m_orbitalElements = m_propagator->Propagate(m_referenceOrbitalElements, m_elapsedPropagationTime, m_mu);
   UpdateOrbitType();

   // Update flags
   m_orbitalElementsDirty = false;
   m_stateVectorDirty = true;
}

void Orbit::PropagateStateVector(const Time& timeDelta)
{
   // Update the elapsed time since the reference state vector
   m_elapsedPropagationTime += timeDelta;

   // Update the reference state vector if it is out-of-date
   if (m_referenceStateVectorDirty)
   {
      UpdateReferenceStateVector();
   }

   // Propagate from the reference state vector and update orbit radius
   m_stateVector = m_propagator->Propagate(m_referenceStateVector, m_elapsedPropagationTime, m_mu);
   UpdateOrbitRadius();

   // Update flags
   m_stateVectorDirty = false;
   m_orbitalElementsDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::PropagateToTrueAnomaly(double trueAnomaly)
{
   // Update the orbital elements if they are out-of-date
   if (m_orbitalElementsDirty)
   {
      UpdateOrbitalElements();
   }

   // Update the true anomaly
   OrbitalElements newOrbitalElements = m_orbitalElements;
   newOrbitalElements.trueAnomaly = trueAnomaly;

   // Set the final orbital elements
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
   os << prefix << "Gravitational Parameter: " << std::setprecision(6) << std::fixed << m_mu << std::endl;
   os << prefix << "Mean Orbital Radius:     " << std::setprecision(6) << std::fixed << m_orbitRadius << std::endl;
   os << prefix << "Orbital Elements:" << std::endl;
   os << GetOrbitalElements().ToDetailedString(prefix + "   ");
   os << prefix << "State Vector:" << std::endl;
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
void Orbit::UpdateReferenceStateVector() const
{
   m_referenceStateVector = ConvertOrbitalElements2StateVector(m_referenceOrbitalElements, m_mu);
   m_referenceStateVectorDirty = false;
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitalElements() const
{
   m_orbitalElements = ConvertStateVector2OrbitalElements(m_stateVector, m_mu);
   m_orbitalElementsDirty = false;
}

////////////////////////////////////////////////////////////
void Orbit::UpdateReferenceOrbitalElements() const
{
   m_referenceOrbitalElements = ConvertStateVector2OrbitalElements(m_referenceStateVector, m_mu);
   m_referenceOrbitalElementsDirty = false;
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