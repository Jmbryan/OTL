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

std::string Orbit::OrbitProperties::ToString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Type:                      ";
   std::string anomalyType = "";
   switch (type)
   {
   case Orbit::Type::Elliptical:
      os << "Elliptical";
      anomalyType = "Eccentric";
      break;

   case Orbit::Type::Hyperbolic:
      os << "Hyperbolic";
      anomalyType = "Hyperbolic";
      break;

   case Orbit::Type::Parabolic:
      os << "Parabolic";
      anomalyType = "Parabolic";
      break;

   case Orbit::Type::Circular:
      os << "Circular";
      break;

   default:
      os << "Unknown";
      break;
   }
   os << std::endl;

   os << prefix << "Mean Radius:               " << std::setprecision(6) << std::fixed << radius << std::endl;
   if (type == Orbit::Type::Elliptical)
      os << prefix << "Eccentric Anomaly          " << std::setprecision(6) << std::fixed << anomaly << std::endl;
   else if (type == Orbit::Type::Hyperbolic)
      os << prefix << "Hyperbolic Anomaly         " << std::setprecision(6) << std::fixed << anomaly << std::endl;
   else if (type == Orbit::Type::Parabolic)
      os << prefix << "Parabolic Anomaly          " << std::setprecision(6) << std::fixed << anomaly << std::endl;
   os << prefix << "True Anomaly:              " << std::setprecision(6) << std::fixed << trueAnomaly << std::endl;
   os << prefix << "Mean Motion:               " << std::setprecision(6) << std::fixed << meanMotion << std::endl;
   os << prefix << "Period:                    " << std::setprecision(6) << std::fixed << period << std::endl;
   os << prefix << "Time Since Perapsis:       " << std::setprecision(6) << std::fixed << timeSincePerapsis << std::endl;
   os << prefix << "Specific Angular Momentum: " << std::setprecision(6) << std::fixed << specificAngularMomentum << std::endl;
   os << prefix << "Semiparameter:             " << std::setprecision(6) << std::fixed << semiparameter << std::endl;

   return os.str();
}


////////////////////////////////////////////////////////////
Orbit::Orbit() :
m_gravitationalParameterCentralBody(1.0),
m_direction(Direction::Invalid),
m_propertiesDirty(false),
m_orbitalElementsDirty(false),
m_StateVectorDirty(false)
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

Orbit::Orbit(double mu, const OrbitalElements& orbitalElements, Direction orbitDirection) :
m_gravitationalParameterCentralBody(mu),
m_orbitalElements(orbitalElements),
m_direction(orbitDirection),
m_propertiesDirty(true),
m_orbitalElementsDirty(false),
m_StateVectorDirty(true)
{

}

Orbit::Orbit(double mu, const StateVector& StateVector) :
m_gravitationalParameterCentralBody(mu),
m_StateVector(StateVector),
m_direction(Direction::Invalid),
m_propertiesDirty(true),
m_orbitalElementsDirty(true),
m_StateVectorDirty(false)
{

}

////////////////////////////////////////////////////////////
Orbit::~Orbit()
{
}

////////////////////////////////////////////////////////////
void Orbit::SetGravitationalParameterCentralBody(double mu)
{
   m_gravitationalParameterCentralBody = mu;
}

void Orbit::SetOrbitalElements(const OrbitalElements& orbitalElements, Direction orbitDirection)
{
   m_orbitalElements = orbitalElements;
   m_direction = orbitDirection;
   m_orbitalElementsDirty = false;

   m_propertiesDirty = true;  
   m_StateVectorDirty = true;
}

void Orbit::SetStateVector(const StateVector& StateVector)
{
   m_StateVector = StateVector;
   m_StateVectorDirty = false;

   m_propertiesDirty = true;
   m_orbitalElementsDirty = true;
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
double Orbit::GetGravitationalParameterCentralBody() const
{
   return m_gravitationalParameterCentralBody;
}

////////////////////////////////////////////////////////////
const StateVector& Orbit::GetStateVector() const
{ 
   UpdateStateVector();
   return m_StateVector;
}
//StateVector Orbit::GetStateVector() const
//{
//   if (m_stateVector.IsCartesian())
//   {
//      return m_stateVector.GetStateVector();
//   }
//   else if (m_cachedStateVectorDirty)
//   {
//      m_cachedStateVector = m_stateVector.ToStateVector(m_mu);
//      m_cachedStateVectorDirty = false;
//   }
//
//   return m_cachedStateVector.GetStateVector();
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
   UpdateOrbitProperties();
   return m_direction;
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
   keplerian::KeplerianPropagator propagator;
   if (m_propertiesDirty || m_orbitalElementsDirty)
   {
      m_orbitalElements.meanAnomaly = propagator.PropagateMeanAnomaly(
         GetOrbitalElements(), m_gravitationalParameterCentralBody, timeDelta);
   }
   else
   {
      m_orbitalElements.meanAnomaly = propagator.PropagateMeanAnomaly(
         m_orbitalElements.meanAnomaly, m_properties.meanMotion, timeDelta);
   }
   

   m_orbitalElementsDirty = false;
   m_propertiesDirty = true;
   m_StateVectorDirty = true;
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
   UpdateOrbitalElements();
   m_orbitalElements.meanAnomaly = meanAnomaly;

   m_orbitalElementsDirty = false;
   m_propertiesDirty = true;
   m_StateVectorDirty = true;
}
void Orbit::PropagateToTrueAnomaly(double trueAnomaly)
{
   UpdateOrbitalElements();
   m_orbitalElements.meanAnomaly = ConvertTrueAnomaly2MeanAnomaly(m_orbitalElements.eccentricity, trueAnomaly);

   m_orbitalElementsDirty = false;
   m_propertiesDirty = true;
   m_StateVectorDirty = true;
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
//      os << m_stateVector.GetStateVector();
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
   UpdateStateVector();
   UpdateOrbitalElements();
   UpdateOrbitProperties();

   std::ostringstream os;
   os << prefix << "Direction:                              ";
   switch (m_direction)
   {
   case Direction::Prograde:
      os << "Prograde" << std::endl;
      break;

   case Direction::Retrograde:
      os << "Retrograde" << std::endl;
      break;

   default:
      os << "Unknown" << std::endl;
      break;
   }  
   os << prefix << "Gravitational Parameter (central body): " << std::setprecision(6) << std::fixed << m_gravitationalParameterCentralBody << std::endl;
   os << prefix << "Properties:" << std::endl << m_properties.ToString(prefix + "  ");
   os << prefix << "Orbital Elements:" << std::endl << m_orbitalElements.ToDetailedString(prefix + "  ");
   os << prefix << "State Vector:" << std::endl << m_StateVector.ToDetailedString(prefix + "  ");
   

   return os.str();
}

////////////////////////////////////////////////////////////
void Orbit::UpdateOrbitProperties() const
{
   UpdateOrbitalElements();
   m_properties = ComputeOrbitProperties(m_gravitationalParameterCentralBody, m_orbitalElements);

   m_propertiesDirty = false;
}

void Orbit::UpdateOrbitalElements() const
{
   if (m_orbitalElementsDirty)
   {
      m_orbitalElements = ConvertStateVector2OrbitalElements(m_StateVector, m_gravitationalParameterCentralBody);
      m_orbitalElementsDirty = false;
   }
}

void Orbit::UpdateStateVector() const
{
   if (m_StateVectorDirty)
   {
      m_StateVector = ConvertOrbitalElements2StateVector(m_orbitalElements, m_gravitationalParameterCentralBody);
      m_StateVectorDirty = false;
   }
}

////////////////////////////////////////////////////////////
Orbit::OrbitProperties ComputeOrbitProperties(double mu, const OrbitalElements& orbitalElements)
{
   Orbit::OrbitProperties properties;

   // Unpack the orbital elements
   const double a = orbitalElements.semiMajorAxis;
   const double e = orbitalElements.eccentricity;
   //const double M = orbitalElements.meanAnomaly;

   // Compute orbit type
   properties.type = ComputeOrbitType(mu, orbitalElements);

   // Compute anomaly (eccentric, hyperbolic, parabolic)
   properties.anomaly = 1.0;// ConvertMeanAnomaly2Anomaly(e, M);

   // Compute true anomaly
   properties.trueAnomaly = ConvertAnomaly2TrueAnomaly(e, properties.anomaly);

   // Compute radius
   properties.radius = ComputeOrbitRadius(orbitalElements, properties.trueAnomaly);

   // Compute mean motion
   properties.meanMotion = sqrt(mu / pow(a, 3.0));

   // Compute period
   properties.period = 1.0 / properties.meanMotion;

   // Compute specific angular momentum (h)
   properties.specificAngularMomentum = 1.0;

   // Compute semiparameter
   properties.semiparameter = 1.0;

   // Compute time since periapsis
   properties.timeSincePerapsis = 1.0;

   return properties;
}

////////////////////////////////////////////////////////////
Orbit::OrbitProperties ComputeOrbitProperties(double mu, const StateVector& StateVector)
{
   //OrbitalElements orbitalElements = ConvertStateVector2OrbitalElements(StateVector, mu);
   //return ComputeOrbitProperties(mu, orbitalElements);

   Orbit::OrbitProperties properties;

   // Unpack the cartesian state vector
   const Vector3d& R = StateVector.position;
   const Vector3d& V = StateVector.velocity;
   const double e = ((SQR(V.norm()) / mu - 1.0 / R.norm()) * R - (R.dot(V) / mu) * V).norm();

   // Compute orbit type
   properties.type = ComputeOrbitType(mu, StateVector);

   // Compute anomaly (eccentric, hyperbolic, parabolic)
   properties.anomaly = 1.0;// ConvertMeanAnomaly2Anomaly(e, M);

   // Compute true anomaly
   properties.trueAnomaly = ConvertAnomaly2TrueAnomaly(e, properties.anomaly);

   // Compute radius
   properties.radius = ComputeOrbitRadius(StateVector);

   // Compute mean motion
   properties.meanMotion = 1.0;

   // Compute period
   properties.period = 1.0 / properties.meanMotion;

   // Compute specific angular momentum (h)
   properties.specificAngularMomentum = R.cross(V).norm();

   // Compute semiperimeter
   properties.semiparameter = 1.0;

   // Compute time since periapsis
   properties.timeSincePerapsis = 1.0;

   return properties;
}

////////////////////////////////////////////////////////////
Orbit::Type ComputeOrbitType(double eccentricity)
{
   Orbit::Type orbitType = Orbit::Type::Invalid;
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