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

#include <OTL/Core/MpcorbBody.h>
#include <OTL/Core/MpcorbEphemeris.h>

namespace otl
{

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody() :
OrbitalBody()
{

}

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody(const std::string& name, const Epoch& epoch) :
OrbitalBody(name, epoch),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody(const std::string& name,
                       const MpcorbEphemeris& ephemeris,
                       const Epoch& epoch) :
OrbitalBody(name, epoch),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void MpcorbBody::SetEphemeris(const MpcorbEphemeris& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void MpcorbBody::LoadEphemerisDataFile(const std::string& filename)
{
   m_ephemeris.LoadDataFile(filename);
}

////////////////////////////////////////////////////////////
std::string MpcorbBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void MpcorbBody::VInitialize()
{
   const auto& name = GetName();
   const auto& epoch = GetEpoch();

   // Init the physical properties
   m_physicalProperties = m_ephemeris.GetPhysicalProperties(name);

   // Init the reference epoch and orbital elements
   m_referenceEpoch = m_ephemeris.GetReferenceEpoch(name);
   m_referenceOrbitalElements = m_ephemeris.GetReferenceOrbitalElements(name);

   // Init the orbit
   double mu = m_ephemeris.GetGravitationalParameterCentralBody(name);
   m_orbit = keplerian::Orbit(mu, m_referenceOrbitalElements, keplerian::Orbit::Direction::Prograde);
   m_orbit.Propagate(epoch - m_referenceEpoch);
}

////////////////////////////////////////////////////////////
void MpcorbBody::VPropagateTo(const Epoch& epoch)
{
   m_orbit.SetOrbitalElements(m_referenceOrbitalElements, keplerian::Orbit::Direction::Prograde);
   m_orbit.Propagate(epoch - m_referenceEpoch);
}

////////////////////////////////////////////////////////////
//EphemerisPointer MpcorbBody::VGetEphemeris()
//{
//   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
//}

////////////////////////////////////////////////////////////
//StateVector MpcorbBody::VQueryStateVector(const Epoch& epoch)
//{
//   OrbitalBody::PropagateTo(epoch);
//   return GetStateVector();
//}

} // namespace otl