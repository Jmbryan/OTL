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
m_name("Unknown"),
m_initialized(false)
{

}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name, const Epoch& epoch) :
m_epoch(epoch),
m_name(name),
m_initialized(false)
{

}

////////////////////////////////////////////////////////////
OrbitalBody::OrbitalBody(const std::string& name,
                         const PhysicalProperties& physicalProperties,
                         double gravitationalParameterCentralBody,
                         const OrbitalElements& orbitalElements,
                         const Epoch& epoch) :
m_epoch(epoch),
m_physicalProperties(physicalProperties),
m_orbit(gravitationalParameterCentralBody, orbitalElements),
m_name(name),
m_initialized(false)
{

}
OrbitalBody::OrbitalBody(const std::string& name,
                         const PhysicalProperties& physicalProperties,
                         double gravitationalParameterCentralBody,
                         const StateVector& StateVector,
                         const Epoch& epoch) :
m_epoch(epoch),
m_physicalProperties(physicalProperties),
m_orbit(gravitationalParameterCentralBody, StateVector),
m_name(name),
m_initialized(false)
{

}
//OrbitalBody::OrbitalBody(const std::string& name,
//                         const PhysicalProperties& physicalProperties,
//                         double gravitationalParameterCentralBody,
//                         const StateVector& stateVector,
//                         const Epoch& epoch) :
//m_name(name),
//m_physicalProperties(physicalProperties),
//m_epoch(epoch),
//m_orbit(gravitationalParameterCentralBody, stateVector)
//{
//
//}

////////////////////////////////////////////////////////////
OrbitalBody::~OrbitalBody()
{

}

////////////////////////////////////////////////////////////
//void OrbitalBody::SetPropagator(const PropagatorPointer& propagator)
//{
//   m_orbit.SetPropagator(propagator);
//}

////////////////////////////////////////////////////////////
const std::string& OrbitalBody::GetName() const
{
   return m_name;
}

////////////////////////////////////////////////////////////
const Epoch& OrbitalBody::GetEpoch() const
{
   return m_epoch;
}

////////////////////////////////////////////////////////////
const PhysicalProperties& OrbitalBody::GetPhysicalProperties() const
{
   Initialize();
   return m_physicalProperties;
}

////////////////////////////////////////////////////////////
const keplerian::Orbit& OrbitalBody::GetOrbit() const
{
   Initialize();
   return m_orbit;
}

//////////////////////////////////////////////////////////////
const OrbitalElements& OrbitalBody::GetOrbitalElements() const
{
   return GetOrbit().GetOrbitalElements();
}

const OrbitalElements& OrbitalBody::GetOrbitalElementsAt(const Epoch& epoch)
{
   PropagateTo(epoch);
   return GetOrbit().GetOrbitalElements();
}

////////////////////////////////////////////////////////////
const StateVector& OrbitalBody::GetStateVector() const
{
   return GetOrbit().GetStateVector();
}

////////////////////////////////////////////////////////////
const StateVector& OrbitalBody::GetStateVectorAt(const Epoch& epoch)
{
   PropagateTo(epoch);
   return GetOrbit().GetStateVector();
}

////////////////////////////////////////////////////////////
//const StateVector& OrbitalBody::GetStateVector() const
//{
//   return GetOrbit().GetStateVector();
//}

////////////////////////////////////////////////////////////
keplerian::Orbit::Type OrbitalBody::GetOrbitType() const
{
   return GetOrbit().GetOrbitType();
}

////////////////////////////////////////////////////////////
const keplerian::Orbit::OrbitProperties& OrbitalBody::GetOrbitProperties() const
{
   return GetOrbit().GetOrbitProperties();
}

////////////////////////////////////////////////////////////
double OrbitalBody::GetGravitationalParameterCentralBody() const
{
   return GetOrbit().GetGravitationalParameterCentralBody();
}

////////////////////////////////////////////////////////////
bool OrbitalBody::IsOrbitType(keplerian::Orbit::Type orbitType) const
{
   return GetOrbit().IsType(orbitType);
}

////////////////////////////////////////////////////////////
void OrbitalBody::Propagate(const Time& timeDelta)
{
   PropagateTo(m_epoch + timeDelta);
}

////////////////////////////////////////////////////////////
void OrbitalBody::PropagateTo(const Epoch& epoch)
{
   Initialize();
   m_epoch = epoch;
   VPropagateTo(epoch);
}

////////////////////////////////////////////////////////////
void OrbitalBody::Initialize() const
{
   if (!m_initialized)
   {
      m_initialized = true;
      const_cast<OrbitalBody*>(this)->VInitialize();
   }
}

////////////////////////////////////////////////////////////
//std::string OrbitalBody::ToString() const
//{
//   std::ostringstream os;
//   os << "name=" << GetName() << " prop=" << GetPhysicalProperties() << " epoch=" << GetEpoch();
//
//   return os.str();
//}

////////////////////////////////////////////////////////////
std::string OrbitalBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "Name: " << GetName() << std::endl;
   os << prefix << "Physical properties:" << std::endl;
   os << GetPhysicalProperties().ToDetailedString(prefix + "   ");
   os << prefix << "Epoch:" << std::endl;
   os << GetEpoch().ToDetailedString(prefix + "   ");
   os << prefix << "Orbit:" << std::endl;
   os << GetOrbit().ToString(prefix + "   ") << std::endl;
   //os << prefix << "Ephemeris:" << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetName(const std::string& name)
{
   m_name = name;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetEpoch(const Epoch& epoch)
{
   m_epoch = epoch;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetPhysicalProperties(const PhysicalProperties& physicalProperties)
{
   m_physicalProperties = physicalProperties;
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetGravitationalParameterCentralBody(double gravitationalParameterCentralBody)
{
   m_orbit.SetGravitationalParameterCentralBody(gravitationalParameterCentralBody);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetOrbitalElements(const OrbitalElements& orbitalElements, keplerian::Orbit::Direction direction)
{
   m_orbit.SetOrbitalElements(orbitalElements, direction);
}

////////////////////////////////////////////////////////////
void OrbitalBody::SetStateVector(const StateVector& StateVector)
{
   m_orbit.SetStateVector(StateVector);
}

////////////////////////////////////////////////////////////
//void OrbitalBody::SetStateVector(const StateVector& stateVector)
//{
//   m_orbit.SetStateVector(stateVector);
//}

} // namespace otl