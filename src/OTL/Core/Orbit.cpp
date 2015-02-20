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
#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
Orbit::Orbit() :
m_orbitType(Type::Invalid),
m_orbitRadius(0.0),
m_mu(0.0),
m_stateVector(),
m_referenceStateVector(),
m_cachedStateVector(),
m_propagator(std::make_shared<KeplerianPropagator>()),
m_elapsedPropagationTime(),
m_orbitPropertiesDirty(false)
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
   m_stateVector = m_referenceStateVector = m_cachedStateVector = stateVector;

   // Reset elapsed propagation time
   m_elapsedPropagationTime = Time();

   // Invalidate orbit properties
   m_orbitPropertiesDirty = true;
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
CartesianStateVector Orbit::GetCartesianStateVector() const
{
   if (m_stateVector.GetType() == StateVectorType::Cartesian)
   {
      return m_stateVector.GetCartesianStateVector();
   }
   else if (m_cachedStateVector.GetType() != StateVectorType::Cartesian)
   {
      m_cachedStateVector = m_stateVector.ToCartesianStateVector(m_mu);
   }

   return m_cachedStateVector.GetCartesianStateVector();
}

////////////////////////////////////////////////////////////
OrbitalElements Orbit::GetOrbitalElements() const
{
   if (m_stateVector.GetType() == StateVectorType::Orbital)
   {
      return m_stateVector.GetOrbitalElements();
   }
   else if (m_cachedStateVector.GetType() != StateVectorType::Orbital)
   {
      m_cachedStateVector = m_stateVector.ToOrbitalElements(m_mu);
   }

   return m_cachedStateVector.GetOrbitalElements();
}

////////////////////////////////////////////////////////////
const StateVector& Orbit::GetStateVector() const
{
   return m_stateVector;
}

////////////////////////////////////////////////////////////
double Orbit::GetOrbitRadius() const
{
   if (m_orbitPropertiesDirty)
   {
      UpdateOrbitProperties();
   }
   return m_orbitRadius;
}

////////////////////////////////////////////////////////////
Orbit::Type Orbit::GetOrbitType() const
{
   if (m_orbitPropertiesDirty)
   {
      UpdateOrbitProperties();
   }
   return m_orbitType;
}

////////////////////////////////////////////////////////////
const Time& Orbit::GetElapsedPropagationTime() const
{
   return m_elapsedPropagationTime;
}

////////////////////////////////////////////////////////////
bool Orbit::IsType(Type orbitType) const
{
   return (GetOrbitType() == orbitType);
}

////////////////////////////////////////////////////////////
void Orbit::Propagate(const Time& timeDelta)
{
   // Update the elapsed time since the reference state vector
   m_elapsedPropagationTime += timeDelta;

   // Propagate from the reference orbital elements and update orbit type
   m_stateVector = m_propagator->Propagate(m_referenceStateVector, m_elapsedPropagationTime, m_mu);

   // Reset the cached state vector
   m_cachedStateVector = m_stateVector;

   // Invalidate orbit properties
   m_orbitPropertiesDirty = true;
}

////////////////////////////////////////////////////////////
void Orbit::PropagateToTrueAnomaly(double trueAnomaly)
{
   // Retrieve the current orbital elements and update the true anomaly
   auto orbitalElements = GetOrbitalElements();
   orbitalElements.trueAnomaly = trueAnomaly;

   // Set the new state vector from the orbital elements
   SetStateVector(StateVector(orbitalElements));

   // Invalidate orbit properties
   m_orbitPropertiesDirty = true;
}

////////////////////////////////////////////////////////////
std::string Orbit::ToString() const
{
   std::ostringstream os;
   os << "t=";
   switch (m_orbitType)
   {
      case Orbit::Type::Elliptical:
         os << "elliptical";
         break;

      case Orbit::Type::Hyperbolic:
         os << "hyperbolic";
         break;

      case Orbit::Type::Parabolic:
         os << "parabolic";
         break;

      case Orbit::Type::Circular:
         os << "circular";
         break;

      default:
         os << "unknown";
         break;
   }
   os << "r=" << m_orbitRadius << " "
      << "mu=" << m_mu << " "; //<< m_stateVector.ToString();

   switch (m_stateVector.GetType())
   {
   case StateVectorType::Orbital:
      os << m_stateVector.GetOrbitalElements();
      break;

   case StateVectorType::Cartesian:
      os << m_stateVector.GetCartesianStateVector();
      break;
         
      default:
         break;
   }

   return os.str();
}

////////////////////////////////////////////////////////////
std::string Orbit::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbit Type:                           ";
   switch (m_orbitType)
   {
   case Orbit::Type::Elliptical:
      os << "Elliptical";
      break;

   case Orbit::Type::Hyperbolic:
      os << "Hyperbolic";
      break;

   case Orbit::Type::Parabolic:
      os << "Parabolic";
      break;

   case Orbit::Type::Circular:
      os << "Circular";
      break;

   default:
      os << "Unknown";
      break;
   }
   os << prefix << "Orbit Radius:                         " << std::setprecision(6) << std::fixed << m_orbitRadius << std::endl;
   os << prefix << "Central Body Gravitational Parameter: " << std::setprecision(6) << std::fixed << m_mu << std::endl;
   os << prefix << "State Vector:" << std::endl;
   //os << GetStateVector().ToDetailedString(prefix + "   ");
   switch (m_stateVector.GetType())
   {
   case StateVectorType::Orbital:
      os << m_stateVector.GetOrbitalElements().ToDetailedString(prefix + "   ") << std::endl;
      break;

   case StateVectorType::Cartesian:
      os << m_stateVector.GetCartesianStateVector().ToDetailedString(prefix + "   ") << std::endl;
      break;
         
   default:
      break;
   }

   return os.str();
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitProperties() const
{
   m_orbitRadius = ComputeOrbitRadius(m_stateVector);
   m_orbitType = ComputeOrbitType(m_stateVector, m_mu);
   m_orbitPropertiesDirty = false;
}

////////////////////////////////////////////////////////////
double ComputeOrbitRadius(const StateVector& stateVector)
{
   double orbitRadius = 0.0;

   const auto& stateVectorType = stateVector.GetType();
   if (stateVectorType == StateVectorType::Cartesian)
   {
      orbitRadius = stateVector.GetCartesianStateVector().position.norm();
   }
   else if (stateVectorType == StateVectorType::Orbital)
   {
      const auto& orbitalElements = stateVector.GetOrbitalElements();
      double a = orbitalElements.semiMajorAxis;
      double e = orbitalElements.eccentricity;
      double ta = orbitalElements.trueAnomaly;

      orbitRadius = a * (1.0 - SQR(e)) / (1.0 + e * cos(ta));
   }
   else
   {
      OTL_ERROR() << "Failed to compute orbit radius. Invalid state vector type";
   }

   return orbitRadius;
}

////////////////////////////////////////////////////////////
Orbit::Type ComputeOrbitType(const StateVector& stateVector, double mu)
{
   Orbit::Type orbitType = Orbit::Type::Invalid;

   const auto& stateVectorType = stateVector.GetType();
   double eccentricity = 0.0;
   if (stateVectorType == StateVectorType::Orbital)
   {
      const auto& orbitalElements = stateVector.GetOrbitalElements();
      eccentricity = orbitalElements.eccentricity;
   }
   else if (stateVectorType == StateVectorType::Cartesian)
   {
      const auto& cartesianStateVector = stateVector.GetCartesianStateVector();
      const auto& R = cartesianStateVector.position;
      const auto& V = cartesianStateVector.velocity;
      eccentricity = ((SQR(V.norm()) / mu - 1.0 / R.norm()) * R - (R.dot(V) / mu) * V).norm();
   }
   else
   {
      OTL_ERROR() << "Failed to compute orbit type. Invalid state vector type";
      return orbitType;
   }

   if (eccentricity > ASTRO_ECC_CIRCULAR && eccentricity < ASTRO_ECC_PARABOLIC)
   {
      orbitType = Orbit::Type::Elliptical;
   }
   else if (eccentricity > ASTRO_ECC_PARABOLIC)
   {
      orbitType = Orbit::Type::Hyperbolic;
   }
   else if (IsApprox(eccentricity, ASTRO_ECC_PARABOLIC))
   {
      orbitType = Orbit::Type::Parabolic;
   }
   else if (IsApprox(eccentricity, ASTRO_ECC_CIRCULAR))
   {
      orbitType = Orbit::Type::Circular;
   }
   else
   {
      OTL_ERROR() << "Invalid orbit type with eccentricity " << Bracket(eccentricity);
   }

   return orbitType;
}

} // namespace keplerian

} // namespace otl