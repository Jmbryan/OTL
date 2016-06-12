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

#include <OTL/Core/JplApproximateBody.h>
#include <OTL/Core/Logger.h>

namespace otl
{

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody() :
OrbitalBody()
{

}

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody(const std::string& name, const Epoch& epoch) :
OrbitalBody(name, epoch)
{

}

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody(const std::string& name, const JplApproximateEphemeris& ephemeris, const Epoch& epoch) :
OrbitalBody(name, epoch),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void JplApproximateBody::SetEphemeris(const JplApproximateEphemeris& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void JplApproximateBody::LoadEphemerisDataFile(const std::string& filename)
{
   m_ephemeris.LoadDataFile(filename);
}

////////////////////////////////////////////////////////////
//std::string JplApproximateBody::ToString() const
//{
//   std::ostringstream os;
//   os << "name=" << GetName() << " epoch=" << GetEpoch();
//
//   return os.str();
//}

////////////////////////////////////////////////////////////
std::string JplApproximateBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "(Orbital Body)" << std::endl;
   os << OrbitalBody::ToString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void JplApproximateBody::VInitialize()
{
   const auto& name = GetName();
   const auto& epoch = GetEpoch();

   // Init the physical properties
   m_physicalProperties = m_ephemeris.GetPhysicalProperties(name);
   //SetPhysicalProperties(
   //   m_ephemeris.GetPhysicalProperties(name));

   // Init the orbit
   double mu = m_ephemeris.GetGravitationalParameterCentralBody(name);
   auto orbitalElements = QueryOrbitalElementsAt(epoch);
   m_orbit = keplerian::Orbit(mu, orbitalElements, keplerian::Orbit::Direction::Prograde);

   // Init the gravitational parameter of the central body
   //m_orbit.SetGravitationalParameterCentralBody(
   //   m_ephemeris.GetGravitationalParameterCentralBody(name));
   //SetGravitationalParameterCentralBody(
   //   m_ephemeris.GetGravitationalParameterCentralBody(name));

   // Init the orbital elements
   //QueryOrbitalElements();
   //m_orbit.SetOrbitalElements(QueryOrbitalElements(epoch));
   //SetOrbitalElements(
   //   m_ephemeris.GetOrbitalElements(name, epoch), keplerian::Orbit::Direction::Prograde);
}

////////////////////////////////////////////////////////////
void JplApproximateBody::VPropagateTo(const Epoch& epoch)
{
   m_orbit.SetOrbitalElements(QueryOrbitalElementsAt(epoch));
}

////////////////////////////////////////////////////////////
OrbitalElements JplApproximateBody::QueryOrbitalElementsAt(const Epoch& epoch)
{
   return m_ephemeris.GetOrbitalElements(GetName(), epoch);
}

////////////////////////////////////////////////////////////
//EphemerisPointer JplApproximateBody::VGetEphemeris()
//{
//   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
//}

////////////////////////////////////////////////////////////
//StateVector JplApproximateBody::VQueryStateVector(const Epoch& epoch)
//{
//   if (m_ephemeris)
//   {
//      return m_ephemeris->GetStateVector(GetName(), epoch);
//   }
//   else
//   {
//      OTL_ERROR() << "Failed to query state vector for planet " << Bracket(GetName())
//         << ": Invalid ephemeris pointer.";
//      return StateVector();
//   }
//}

} // namespace otl