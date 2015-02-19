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
IEphemerisBody(),
m_ephemeris()
{

}

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody(const std::string& name,
                       const Epoch& epoch) :
IEphemerisBody(name, PhysicalProperties(), 1.0, StateVector()),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody(const std::string& name,
                       const MpcorbEphemerisPointer& ephemeris,
                       const Epoch& epoch) :
IEphemerisBody(name, PhysicalProperties(), 1.0, StateVector()),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void MpcorbBody::SetEphemeris(const MpcorbEphemerisPointer& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void MpcorbBody::VInitialize()
{
   // Init the ephemeris
   if (!m_ephemeris)
   {
      m_ephemeris = std::make_shared<MpcorbEphemeris>();
   }

   // Init the physical properties
   SetPhysicalProperties(
      m_ephemeris->GetPhysicalProperties(GetName()));

   // Init the gravitational parameter of central body
   SetGravitationalParameterCentralBody(
      m_ephemeris->GetGravitationalParameterCentralBody(GetName()));

   // Init the reference state vector
   SetStateVector(
      m_ephemeris->GetReferenceStateVector(GetName()));
   
   // Propagate to current epoch
   PropagateTo(GetEpoch());
}

////////////////////////////////////////////////////////////
EphemerisPointer MpcorbBody::VGetEphemeris()
{
   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
}

////////////////////////////////////////////////////////////
StateVector MpcorbBody::VQueryStateVector(const Epoch& epoch)
{
   OrbitalBody::PropagateTo(epoch);
   return GetStateVector();
}

} // namespace otl