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
m_direction(Direction::Invalid),
m_orbitPropertiesDirty(false),
m_orbitalElementsDirty(false),
m_cartesianStateVectorDirty(false),
m_referenceDirty(false)
//m_orbitType(Type::Invalid),
//m_orbitRadius(0.0),
//m_mu(0.0),
//m_propagator(std::make_shared<KeplerianPropagator>()),
//m_orbitPropertiesDirty(false),
//m_referenceStateVectorDirty(false),
//m_cachedStateVectorDirty(false)

{
}

////////////////////////////////////////////////////////////
//Orbit::Orbit(double mu) :
//Orbit()
//{
//   SetMu(mu);
//}

////////////////////////////////////////////////////////////
//Orbit::Orbit(double mu, const StateVector& stateVector) :
//Orbit(mu)
//{
//   SetStateVector(stateVector);
//}

Orbit::Orbit(const OrbitalElements& orbitalElements, double mu, Epoch epoch, Direction orbitDirection) :
m_direction(orbitDirection),
m_epoch(epoch),
m_orbitalElements(orbitalElements),
m_gravitationalParameterCentralBody(mu),
m_orbitPropertiesDirty(true),
m_orbitalElementsDirty(false),
m_cartesianStateVectorDirty(true),
m_referenceDirty(true)
{

}

Orbit::Orbit(const CartesianStateVector& cartesianStateVector, double mu, Epoch epoch, Direction orbitDirection) :
m_direction(orbitDirection),
m_epoch(epoch),
m_cartesianStateVector(cartesianStateVector),
m_gravitationalParameterCentralBody(mu),
m_orbitPropertiesDirty(true),
m_orbitalElementsDirty(true),
m_cartesianStateVectorDirty(false),
m_referenceDirty(true)
{

}

////////////////////////////////////////////////////////////
Orbit::~Orbit()
{
}

////////////////////////////////////////////////////////////
void Orbit::SetMu(double mu)
{
   m_gravitationalParameterCentralBody = mu;
}

void Orbit::SetOrbitalElements(const OrbitalElements& orbitalElements)
{
   m_orbitalElements = orbitalElements;
   m_orbitalElementsDirty = false;

   m_orbitPropertiesDirty = true;  
   m_cartesianStateVectorDirty = true;
   m_referenceDirty = true;
}

void Orbit::SetCartesianStateVector(const CartesianStateVector& cartesianStateVector)
{
   m_cartesianStateVector = cartesianStateVector;
   m_cartesianStateVectorDirty = true;

   m_orbitPropertiesDirty = true;
   m_orbitalElementsDirty = false;
   m_referenceDirty = true;
}

////////////////////////////////////////////////////////////
//void Orbit::SetStateVector(const StateVector& stateVector)
//{ 
//   m_stateVector = stateVector;
//
//   // Reset elapsed propagation time
//   m_elapsedPropagationTime = Time();
//
//   //// Invalidate orbit properties
//   m_orbitPropertiesDirty = true;
//   m_referenceStateVectorDirty = true;
//   m_cachedStateVectorDirty = true;
//}

////////////////////////////////////////////////////////////
//void Orbit::SetPropagator(const PropagatorPointer& propagator)
//{
//   m_propagator = propagator;
//}

////////////////////////////////////////////////////////////
double Orbit::GetMu() const
{
   return m_gravitationalParameterCentralBody;
}

////////////////////////////////////////////////////////////
const CartesianStateVector& Orbit::GetCartesianStateVector() const
{ 
   UpdateCartesianStateVector();
   return m_cartesianStateVector;
}
//CartesianStateVector Orbit::GetCartesianStateVector() const
//{
//   if (m_stateVector.IsCartesian())
//   {
//      return m_stateVector.GetCartesianStateVector();
//   }
//   else if (m_cachedStateVectorDirty)
//   {
//      m_cachedStateVector = m_stateVector.ToCartesianStateVector(m_mu);
//      m_cachedStateVectorDirty = false;
//   }
//
//   return m_cachedStateVector.GetCartesianStateVector();
//}

////////////////////////////////////////////////////////////
const OrbitalElements& Orbit::GetOrbitalElements() const
{
   UpdateOrbitalElements();
   return m_orbitalElements;
}
//OrbitalElements Orbit::GetOrbitalElements() const
//{
//   if (m_stateVector.IsOrbital())
//   {
//      return m_stateVector.GetOrbitalElements();
//   }
//   else if (m_cachedStateVectorDirty)
//   {
//      m_cachedStateVector = m_stateVector.ToOrbitalElements(m_mu);
//      m_cachedStateVectorDirty = false;
//   }
//
//   return m_cachedStateVector.GetOrbitalElements();
//}

////////////////////////////////////////////////////////////
//const StateVector& Orbit::GetStateVector() const
//{
//   return m_stateVector;
//}

////////////////////////////////////////////////////////////
//double Orbit::GetOrbitRadius() const
//{
//   if (m_orbitPropertiesDirty)
//   {
//      UpdateOrbitProperties();
//   }
//   return m_orbitRadius;
//}

////////////////////////////////////////////////////////////
Orbit::Type Orbit::GetOrbitType() const
{
   return GetOrbitProperties().type;
}

Orbit::Direction Orbit::GetOrbitDirection() const
{
   return m_direction;
}

const Epoch& Orbit::GetEpoch() const
{
   return m_epoch;
}

const Orbit::OrbitProperties& Orbit::GetOrbitProperties() const
{
   UpdateOrbitProperties();
   return m_properties;
}

////////////////////////////////////////////////////////////
//const Time& Orbit::GetElapsedPropagationTime() const
//{
//   return m_elapsedPropagationTime;
//}

////////////////////////////////////////////////////////////
bool Orbit::IsType(Type orbitType) const
{
   return (GetOrbitType() == orbitType);
}

bool Orbit::IsDirection(Direction orbitDirection) const
{
   return (GetOrbitDirection() == orbitDirection);
}

////////////////////////////////////////////////////////////
void Orbit::Propagate(const Time& timeDelta)
{
   UpdateReference();
   //m_orbitalElements = m_propagator.Propagate(m_referenceOrbitalElements, timeDelta, m_gravitationalParameterCentralBody);

   m_orbitalElementsDirty = false;
   m_orbitPropertiesDirty = true;
   m_cartesianStateVectorDirty = true;
}
void Orbit::PropagateTo(const Epoch& epoch)
{
   UpdateReference();
   return Propagate(epoch - m_referenceEpoch);
}
//void Orbit::Propagate(const Time& timeDelta)
//{
//   // Update the elapsed time since the reference state vector
//   m_elapsedPropagationTime += timeDelta;
//
//   // Update the reference state vector if it is dirty and
//   // make sure it is the same type as the propagator
//   if (m_referenceStateVectorDirty)
//   {
//      m_referenceStateVectorDirty = false;
//      m_referenceStateVector = m_stateVector;
//      m_referenceStateVector.ConvertTo(m_propagator->GetType(), m_mu);
//   }
//
//   // Propagate from the reference orbital elements and update orbit type
//   m_stateVector = m_propagator->Propagate(m_referenceStateVector, m_elapsedPropagationTime, m_mu);
//
//   // Invalidate orbit properties
//   m_orbitPropertiesDirty = true;
//   m_cachedStateVectorDirty = true;
//}

////////////////////////////////////////////////////////////
void Orbit::PropagateToMeanAnomaly(double meanAnomaly)
{
   UpdateReference();
   m_orbitalElements = m_referenceOrbitalElements;
   //m_orbitalElements.meanAnomaly = meanAnomaly;

   m_orbitalElementsDirty = false;
   m_orbitPropertiesDirty = true;
   m_cartesianStateVectorDirty = true;
}
void Orbit::PropagateToTrueAnomaly(double trueAnomaly)
{
   UpdateReference();
   m_orbitalElements = m_referenceOrbitalElements;  
   //m_orbitalElements.meanAnomaly = ConvertTrueAnomaly2MeanAnomaly(m_orbitalElements.eccentricity, trueAnomaly);

   m_orbitalElementsDirty = false;
   m_orbitPropertiesDirty = true;
   m_cartesianStateVectorDirty = true;
}

//void Orbit::PropagateToTrueAnomaly(double trueAnomaly)
//{
//   // Retrieve the current orbital elements and update the true anomaly
//   auto orbitalElements = GetOrbitalElements();
//   orbitalElements.trueAnomaly = trueAnomaly;
//
//   // Set the new state vector from the orbital elements
//   SetStateVector(StateVector(orbitalElements));
//
//   // Invalidate orbit properties
//   m_orbitPropertiesDirty = true;
//}

////////////////////////////////////////////////////////////
//std::string Orbit::ToString() const
//{
//   std::ostringstream os;
//   os << "t=";
//   switch (m_orbitType)
//   {
//      case Orbit::Type::Elliptical:
//         os << "elliptical";
//         break;
//
//      case Orbit::Type::Hyperbolic:
//         os << "hyperbolic";
//         break;
//
//      case Orbit::Type::Parabolic:
//         os << "parabolic";
//         break;
//
//      case Orbit::Type::Circular:
//         os << "circular";
//         break;
//
//      default:
//         os << "unknown";
//         break;
//   }
//   os << "r=" << m_orbitRadius << " "
//      << "mu=" << m_mu << " ";
//
//   switch (m_stateVector.GetType())
//   {
//   case StateVectorType::Orbital:
//      os << m_stateVector.GetOrbitalElements();
//      break;
//
//   case StateVectorType::Cartesian:
//      os << m_stateVector.GetCartesianStateVector();
//      break;
//         
//      default:
//         break;
//   }
//
//   return os.str();
//}

////////////////////////////////////////////////////////////
std::string Orbit::ToString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbit Type:                           ";
   switch (m_type)
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
   //os << prefix << "Orbit Radius:                         " << std::setprecision(6) << std::fixed << m_orbitRadius << std::endl;
   //os << prefix << "Central Body Gravitational Parameter: " << std::setprecision(6) << std::fixed << m_mu << std::endl;
   //os << prefix << "State Vector:" << std::endl;
   //switch (m_stateVector.GetType())
   //{
   //case StateVectorType::Orbital:
   //   os << m_stateVector.GetOrbitalElements().ToDetailedString(prefix + "   ") << std::endl;
   //   break;

   //case StateVectorType::Cartesian:
   //   os << m_stateVector.GetCartesianStateVector().ToDetailedString(prefix + "   ") << std::endl;
   //   break;
   //      
   //default:
   //   break;
   //}

   return os.str();
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitProperties() const
{
   //m_orbitRadius = ComputeOrbitRadius(m_stateVector);
   //m_orbitType = ComputeOrbitType(m_stateVector, m_mu);

   UpdateOrbitalElements();
   const double a = m_orbitalElements.semiMajorAxis;
   const double e = m_orbitalElements.eccentricity;
   const double M = 0.0;// m_orbitalElements.meanAnomaly;
   const double mu = m_gravitationalParameterCentralBody;

   // Compute anomaly (eccentric, hyperbolic, parabolic)
   //m_properties.anomaly = ConvertMeanAnomaly2Anomaly(e, M);

   // Compute true anomaly
   m_properties.trueAnomaly = ConvertAnomaly2TrueAnomaly(e, m_properties.anomaly);

   // Compute radius
   m_properties.radius = a * (1.0 - SQR(e)) / (1.0 + e * cos(m_properties.trueAnomaly));

   // Compute mean motion
   m_properties.meanMotion = sqrt(mu / pow(a, 3.0));

   // Compute period

   // Compute specific angular momentum (h)
   //m_properties.specificAngularMomentum = m_cartesianStateVector.position.cross(m_cartesianStateVector.velocity).norm(); // use COES

   // Compute semiperimeter

   // Compute time since periapsis

   

   m_orbitPropertiesDirty = false;
}

void Orbit::UpdateOrbitalElements() const
{
   if (m_orbitalElementsDirty)
   {
      m_orbitalElements = ConvertCartesianStateVector2OrbitalElements(m_cartesianStateVector, m_gravitationalParameterCentralBody);
      m_orbitalElementsDirty = false;
   }
}

void Orbit::UpdateCartesianStateVector() const
{
   if (m_cartesianStateVectorDirty)
   {
      m_cartesianStateVector = ConvertOrbitalElements2CartesianStateVector(m_orbitalElements, m_gravitationalParameterCentralBody);
      m_cartesianStateVectorDirty = false;
   }
}

void Orbit::UpdateReference() const
{
   if (m_referenceDirty)
   {
      m_referenceEpoch = m_epoch;
      m_referenceOrbitalElements = GetOrbitalElements();
      m_referenceDirty = false;
   }
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
      OTL_ERROR() << "Invalid state vector type " << Bracket(static_cast<int>(stateVectorType));
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
      OTL_ERROR() << "Invalid state vector type " << Bracket(static_cast<int>(stateVectorType));
      return orbitType;
   }

   if (IsElliptical(eccentricity))
   {
      orbitType = Orbit::Type::Elliptical;
   }
   else if (IsHyperbolic(eccentricity))
   {
      orbitType = Orbit::Type::Hyperbolic;
   }
   else if (IsParabolic(eccentricity))
   {
      orbitType = Orbit::Type::Parabolic;
   }
   else if (IsCircular(eccentricity))
   {
      orbitType = Orbit::Type::Circular;
   }
   else
   {
      OTL_ERROR() << "Invalid eccentricity " << Bracket(eccentricity);
   }

   return orbitType;
}

} // namespace keplerian

} // namespace otl