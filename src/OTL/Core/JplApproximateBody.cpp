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
#include <OTL/Core/JplApproximateEphemeris.h>
#include <OTL/Core/Logger.h>

namespace otl
{

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody() :
IEphemerisBody()
{

}

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody(const std::string& name,
               const Epoch& epoch) :
IEphemerisBody(name, PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
JplApproximateBody::JplApproximateBody(const std::string& name,
               const JplApproximateEphemerisPointer& ephemeris,
               const Epoch& epoch) :
IEphemerisBody(name, PhysicalProperties(), 1.0, test::StateVector(OrbitalElements()), epoch),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void JplApproximateBody::SetEphemeris(const JplApproximateEphemerisPointer& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
std::string JplApproximateBody::ToString() const
{
   std::ostringstream os;
   os << "name=" << GetName() << " epoch=" << GetEpoch();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string JplApproximateBody::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToDetailedString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void JplApproximateBody::VInitialize()
{
   // Init the ephemeris
   if (!m_ephemeris)
   {
      m_ephemeris = std::make_shared<JplApproximateEphemeris>();
   }
   m_ephemeris->Initialize();

   const auto& name = GetName();
   const auto& epoch = GetEpoch();

   // Init the physical properties
   SetPhysicalProperties(
      m_ephemeris->GetPhysicalProperties(name));

   // Init the gravitational parameter of the central body
   SetGravitationalParameterCentralBody(
      m_ephemeris->GetGravitationalParameterCentralBody(name));

   // Init the state vector
   SetStateVector(
      m_ephemeris->GetStateVector(name, epoch));
}

////////////////////////////////////////////////////////////
EphemerisPointer JplApproximateBody::VGetEphemeris()
{
   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
}

////////////////////////////////////////////////////////////
test::StateVector JplApproximateBody::VQueryStateVector(const Epoch& epoch)
{
   if (m_ephemeris)
   {
      return m_ephemeris->GetStateVector(GetName(), epoch);
   }
   else
   {
      OTL_ERROR() << "Failed to query state vector for planet " << Bracket(GetName())
         << ": Invalid ephemeris pointer.";
      return test::StateVector();
   }
}

} // namespace otl