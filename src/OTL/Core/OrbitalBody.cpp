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
   os << "m=" << m_mass << " r=" << m_radius << std::endl;

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
m_name("Unknown"),
m_physicalProperties(),
m_epoch(),
m_orbit(),
m_ephemeris(nullptr),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name,
                         const PhysicalProperties& physicalProperties,
                         double gravitationalParameterCentralBody,
                         const test::StateVector& stateVector,
                         const Epoch& epoch) :
m_name(name),
m_physicalProperties(physicalProperties),
m_epoch(epoch),
m_orbit(gravitationalParameterCentralBody, stateVector),
m_ephemeris(nullptr),
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
   ExecuteDelayedCommand("PhysicalProperties");
   return m_physicalProperties;
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetGravitationalParameterCentralBody() const
{
   ExecuteDelayedCommand("CentralBodyMu");
   return m_orbit.GetMu();
}

////////////////////////////////////////////////////////////
StateVector OrbitalBody::GetCartesianStateVector() const
{
   return GetOrbit().GetCartesianStateVector();
}

//////////////////////////////////////////////////////////////
OrbitalElements OrbitalBody::GetOrbitalElements() const
{
   return GetOrbit().GetOrbitalElements();
}

////////////////////////////////////////////////////////////
const test::StateVector& OrbitalBody::GetStateVector() const
{
   return GetOrbit().GetStateVector();
}

////////////////////////////////////////////////////////////
const keplerian::Orbit& OrbitalBody::GetOrbit() const
{
   ExecuteDelayedCommand("CentralBodyMu");
   ExecuteDelayedCommand("StateVector");
   return m_orbit;
}

////////////////////////////////////////////////////////////
keplerian::Orbit::Type OrbitalBody::GetOrbitType() const
{
   return GetOrbit().GetOrbitType();
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetOrbitRadius() const
{
   return GetOrbit().GetOrbitRadius();
}

////////////////////////////////////////////////////////////
const Epoch& OrbitalBody::GetEpoch() const
{
   return m_epoch;
}

////////////////////////////////////////////////////////////
bool OrbitalBody::IsOrbitType(keplerian::Orbit::Type orbitType) const
{
   return GetOrbit().IsType(orbitType);
}

////////////////////////////////////////////////////////////
void OrbitalBody::Propagate(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   if (IsEphemerisUpdateRequired(timeDelta))
   {
      QueryStateVector(m_epoch);
   }
   else
   {
      ExecuteDelayedCommand("CentralBodyMu");
      ExecuteDelayedCommand("StateVector");
      VPropagate(timeDelta);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateTo(const Epoch& epoch)
{
   Propagate(epoch - m_epoch);
}

////////////////////////////////////////////////////////////
const PhysicalProperties& OrbitalBody::QueryPhysicalProperties()
{
   RemoveDelayedCommand("PhysicalProperties");
   VQueryPhysicalProperties();
   return GetPhysicalProperties();
}

////////////////////////////////////////////////////////////
double OrbitalBody::QueryCentralBodyMu()
{
   RemoveDelayedCommand("CentralBodyMu");
   VQueryCentralBodyMu();
   return m_orbit.GetMu();
}

////////////////////////////////////////////////////////////
const test::StateVector& OrbitalBody::QueryStateVector(const Epoch& epoch)
{
   m_epoch = epoch;
   ExecuteDelayedCommand("CentralBodyMu");

   RemoveDelayedCommand("StateVector");
   VQueryStateVector(epoch);
   return GetStateVector();
}

////////////////////////////////////////////////////////////
void OrbitalBody::LazyQueryPhysicalProperties()
{
   AddDelayedCommand("PhysicalProperties", std::bind(&OrbitalBody::VQueryPhysicalProperties, this));
}

////////////////////////////////////////////////////////////
void OrbitalBody::LazyQueryCentralBodyMu()
{
   AddDelayedCommand("CentralBodyMu", std::bind(&OrbitalBody::VQueryCentralBodyMu, this));
}

////////////////////////////////////////////////////////////
void OrbitalBody::LazyQueryStateVector(const Epoch& epoch)
{
   m_epoch = epoch;
   AddDelayedCommand("StateVector", std::bind(&OrbitalBody::VQueryStateVector, this, epoch));
}

////////////////////////////////////////////////////////////
std::string OrbitalBody::ToString() const
{
   std::ostringstream os;
   os << "name=" << GetName() << " prop=" << GetPhysicalProperties() << " epoch=" << GetEpoch();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string OrbitalBody::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Name: " << GetName() << std::endl;
   os << prefix << "Physical properties:" << std::endl;
   os << GetPhysicalProperties().ToDetailedString(prefix + "   ");
   os << prefix << "Epoch:" << std::endl;
   os << GetEpoch().ToDetailedString(prefix + "   ");
   os << prefix << "Orbit:" << std::endl;
   os << GetOrbit().ToDetailedString(prefix + "   ");
   os << prefix << "Ephemeris:" << std::endl;
   //os << m_ephemeris->ToDetailedString(prefix + "   "); [TODO]
   os << prefix << "Max propagation time:" << std::endl;
   os << m_maxPropagationTime.ToDetailedString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void OrbitalBody::VPropagate(const Time& timeDelta)
{
   m_orbit.Propagate(timeDelta);
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryPhysicalProperties()
{
   if (m_ephemeris)
   {
      PhysicalProperties physicalProperties;
      m_ephemeris->GetPhysicalProperties(m_name, physicalProperties);
      SetPhysicalProperties(physicalProperties);
   }
   else
   {
      OTL_ERROR() << "Failed to query physical properties for orbital body " << Bracket(m_name) << ": Invalid ephemeris pointer.";
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryCentralBodyMu()
{
   if (m_ephemeris)
   {
      double gravitationalParameterCentralBody;
      m_ephemeris->GetGravitationalParameterCentralBody(m_name, gravitationalParameterCentralBody);
      SetGravitationalParameterCentralBody(gravitationalParameterCentralBody);
   }
   else
   {
      OTL_ERROR() << "Failed to query gravitational parameter of central body for orbital body " << Bracket(m_name) << ": Invalid ephemeris pointer.";
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::VQueryStateVector(const Epoch& epoch)
{
   if (m_ephemeris)
   {
      test::StateVector stateVector;
      m_ephemeris->GetStateVector(m_name, epoch, stateVector);
      m_orbit.SetStateVector(stateVector);
   }
   else
   {
      OTL_ERROR() << "Failed to query state vector for orbital body " << Bracket(m_name) << ": Invalid ephemeris pointer.";
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetPhysicalProperties(const PhysicalProperties& physicalProperties)
{
   m_physicalProperties = physicalProperties;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetGravitationalParameterCentralBody(double centralBodyMu)
{
   m_orbit.SetMu(centralBodyMu);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetStateVector(const test::StateVector& stateVector)
{
   m_orbit.SetStateVector(stateVector);
}

////////////////////////////////////////////////////////////
void OrbitalBody::AddDelayedCommand(const std::string& name, const DelayedCommand& delayedCommand)
{
   m_delayedCommands[name] = delayedCommand;
}

////////////////////////////////////////////////////////////
void OrbitalBody::RemoveDelayedCommand(const std::string& name) const
{
   auto it = m_delayedCommands.find(name);
   if (it != m_delayedCommands.end())
   {
      m_delayedCommands.erase(it);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateEpoch(const Time& timeDelta)
{
   m_epoch += timeDelta;
}

////////////////////////////////////////////////////////////
bool OrbitalBody::IsEphemerisUpdateRequired(const Time& timeDelta)
{
   return (m_ephemeris && (m_orbit.GetElapsedPropagationTime() + timeDelta) > m_maxPropagationTime);
}

////////////////////////////////////////////////////////////
void OrbitalBody::ExecuteDelayedCommand(const std::string& name) const
{
   auto it = m_delayedCommands.find(name);
   if (it != m_delayedCommands.end())
   {
      it->second();
      m_delayedCommands.erase(it);
   }
}

////////////////////////////////////////////////////////////
void OrbitalBody::ExecuteAllDelayedCommands() const
{
   auto it = m_delayedCommands.begin();
   while (it != m_delayedCommands.end())
   {
      it->second();
      it = m_delayedCommands.erase(it);
   }
}

} // namespace otl