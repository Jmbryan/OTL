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

#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Ephemeris.h>

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
   os << prefix << "Mass:                    " << m_mass;
   os << prefix << "Equatorial Radius:       " << m_radius;
   os << prefix << "Safe Radius:             " << m_safeRadius;
   os << prefix << "Gravitational Parameter: " << m_mu;

   return os.str();
}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody() :
m_name(""),
m_physicalProperties(),
m_orbit(),
m_epoch(),
m_ephemeris(),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name,
                         const PhysicalProperties& physicalProperties,
                         double gravitationalParameterCentralBody,
                         const StateVector& stateVector,
                         const Epoch& epoch) :
m_name(name),
m_physicalProperties(physicalProperties),
m_orbit(gravitationalParameterCentralBody, stateVector),
m_epoch(epoch),
m_ephemeris(),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name,
                         const PhysicalProperties& physicalProperties,
                         double gravitationalParameterCentralBody,
                         const OrbitalElements& orbitalElements,
                         const Epoch& epoch) :
m_name(name),
m_physicalProperties(physicalProperties),
m_orbit(gravitationalParameterCentralBody, orbitalElements),
m_epoch(epoch),
m_ephemeris(),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
OrbitalBody::~OrbitalBody()
{

}

////////////////////////////////////////////////////////////
void OrbitalBody::SetPropagator(const PropagatorPointer& propagator)
{
   m_orbit.SetPropagator(propagator);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetEphemeris(const EphemerisPointer& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetMaxPropagationTime(const Time& maxTime)
{
   m_maxPropagationTime = maxTime;
}

////////////////////////////////////////////////////////////
const std::string& OrbitalBody::GetName() const
{
   return m_name;
}

////////////////////////////////////////////////////////////
const PhysicalProperties& OrbitalBody::GetPhysicalProperties() const
{
   return m_physicalProperties;
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetGravitationalParameterCentralBody() const
{
   return m_orbit.GetMu();
}

//////////////////////////////////////////////////////////////
//const Vector3d& OrbitalBody::GetPosition() const
//{
//   return m_orbit.GetStateVector().position;
//}
//
//////////////////////////////////////////////////////////////
//const Vector3d& OrbitalBody::GetVelocity() const
//{
//   return m_orbit.GetStateVector().velocity;
//}
//
//////////////////////////////////////////////////////////////
//const StateVector& OrbitalBody::GetStateVector() const
//{
//   return m_orbit.GetStateVector();
//}
//
//////////////////////////////////////////////////////////////
//const OrbitalElements& OrbitalBody::GetOrbitalElements() const
//{
//   return m_orbit.GetOrbitalElements();
//}

////////////////////////////////////////////////////////////
const keplerian::Orbit& OrbitalBody::GetOrbit() const
{
   ExecuteDelayedEphemerisQuery();
   return m_orbit;
}

//////////////////////////////////////////////////////////////
//keplerian::Orbit::Type OrbitalBody::GetOrbitType() const
//{
//   return m_orbit.GetOrbitType();
//}
//
//////////////////////////////////////////////////////////////
//double OrbitalBody::GetOrbitRadius() const
//{
//   return m_orbit.GetOrbitRadius();
//}

////////////////////////////////////////////////////////////
const Epoch& OrbitalBody::GetEpoch() const
{
   return m_epoch;
}

//////////////////////////////////////////////////////////////
//bool OrbitalBody::IsOrbitType(keplerian::Orbit::Type orbitType) const
//{
//   return m_orbit.IsType(orbitType);
//}

////////////////////////////////////////////////////////////
void OrbitalBody::Propagate(const Time& timeDelta, const PropagationType& propagationType)
{
   PropagateEpoch(timeDelta);
   if (IsEphemerisUpdateRequired(timeDelta))
   {
      switch (propagationType)
      {
      case PropagationType::OrbitalElements:
         QueryOrbitalElements(m_epoch);
         break;

      case PropagationType::StateVector:
         QueryStateVector(m_epoch);
         break;
      }
   }
   else
   {
      ExecuteDelayedEphemerisQuery();
      VPropagate(timeDelta, propagationType);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateTo(const Epoch& epoch, const PropagationType& propagationType)
{
   Propagate(epoch - m_epoch, propagationType);
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateOrbitalElements(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   if (IsEphemerisUpdateRequired(timeDelta))
   {
      QueryOrbitalElements(m_epoch);
   }
   else
   {
      ExecuteDelayedEphemerisQuery();
      VPropagateOrbitalElements(timeDelta);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateOrbitalElementsTo(const Epoch& epoch)
{
   PropagateOrbitalElements(epoch - m_epoch);
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateStateVector(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   if (IsEphemerisUpdateRequired(timeDelta))
   {
      QueryStateVector(m_epoch);
   }
   else
   {
      ExecuteDelayedEphemerisQuery();
      VPropagateStateVector(timeDelta);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateStateVectorTo(const Epoch& epoch)
{
   PropagateStateVector(epoch - m_epoch);
}

////////////////////////////////////////////////////////////
void OrbitalBody::QueryEphemeris(const Epoch& epoch, const EphemerisQueryType& queryType)
{
   VQueryEphemeris(epoch, queryType);
}

////////////////////////////////////////////////////////////
void OrbitalBody::QueryOrbitalElements(const Epoch& epoch)
{
   m_epoch = epoch;
   m_delayedEphemerisQuery = std::bind(&OrbitalBody::VQueryOrbitalElements, this, epoch);
   //VQueryOrbitalElements(epoch);
}

////////////////////////////////////////////////////////////
void OrbitalBody::QueryStateVector(const Epoch& epoch)
{
   m_epoch = epoch;
   m_delayedEphemerisQuery = std::bind(&OrbitalBody::VQueryStateVector, this, epoch);
   //VQueryStateVector(epoch);
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

////////////////////////////////////////////////////////////
void OrbitalBody::VPropagate(const Time& timeDelta, const PropagationType& propagationType)
{
   m_orbit.Propagate(timeDelta, propagationType);
}

////////////////////////////////////////////////////////////
void OrbitalBody::VPropagateOrbitalElements(const Time& timeDelta)
{
   m_orbit.PropagateOrbitalElements(timeDelta);
}

////////////////////////////////////////////////////////////
void OrbitalBody::VPropagateStateVector(const Time& timeDelta)
{
   m_orbit.PropagateStateVector(timeDelta);
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryEphemeris(const Epoch& epoch, const EphemerisQueryType& queryType)
{
   switch (queryType)
   {
   case EphemerisQueryType::OrbitalElements:
      QueryOrbitalElements(epoch);
      //m_delayedEphemerisQuery = std::bind(&OrbitalBody::QueryOrbitalElements, this, std::placeholders::_1);
      //VQueryOrbitalElements(epoch);
      break;

   case EphemerisQueryType::StateVector:
      QueryStateVector(epoch);
      //m_delayedEphemerisQuery = std::bind(&OrbitalBody::QueryStateVector, this, std::placeholders::_1);
      //VQueryStateVector(epoch);
      break;

   default:
      OTL_ERROR() << "Failed to query ephemeris database. Invalid ephemeris query type";
      break;
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryOrbitalElements(const Epoch& epoch)
{
   if (m_ephemeris)
   {
      //m_orbit.SetOrbitalElements(m_ephemeris->GetOrbitalElements(m_name, epoch));
      OrbitalElements coes;
      m_ephemeris->GetOrbitalElements(m_name, epoch, coes);
      m_orbit.SetOrbitalElements(coes);
   }
   else
   {
      OTL_ERROR() << "Failed to query orbital elements for orbital body " << Bracket(m_name) << ": Invalid ephemeris pointer.";
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryStateVector(const Epoch& epoch)
{
   if (m_ephemeris)
   {
      //m_orbit.SetStateVector(m_ephemeris->GetStateVector(m_name, epoch));
      StateVector sv;
      m_ephemeris->GetStateVector(m_name, epoch, sv);
      m_orbit.SetStateVector(sv);
   }
   else
   {
      OTL_ERROR() << "Failed to query state vector for orbital body " << Bracket(m_name) << ": Invalid ephemeris pointer.";
   }
}

////////////////////////////////////////////////////////////
bool OrbitalBody::VInitializeEphemeris()
{
   return true;
}

////////////////////////////////////////////////////////////
bool OrbitalBody::IsEphemerisUpdateRequired(const Time& timeDelta)
{
   return (m_ephemeris && (m_orbit.GetElapsedPropagationTime() + timeDelta) > m_maxPropagationTime);
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateEpoch(const Time& timeDelta)
{
   m_epoch += timeDelta;
}

////////////////////////////////////////////////////////////
void OrbitalBody::ExecuteDelayedEphemerisQuery() const
{
   if (m_delayedEphemerisQuery)
   {
      m_delayedEphemerisQuery(m_epoch);
      m_delayedEphemerisQuery = nullptr;
   }
}

} // namespace otl