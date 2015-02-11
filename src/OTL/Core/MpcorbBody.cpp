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
IEphemerisBody(name, PhysicalProperties(), 1.0, test::StateVector()),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
MpcorbBody::MpcorbBody(const std::string& name,
                       const MpcorbEphemerisPointer& ephemeris,
                       const Epoch& epoch) :
IEphemerisBody(name, PhysicalProperties(), 1.0, test::StateVector()),
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
   m_ephemeris->Initialize();

   // Init the physical properties
   SetPhysicalProperties(
      m_ephemeris->GetPhysicalProperties(GetName()));

   // Init the gravitational parameter of central body
   SetGravitationalParameterCentralBody(
      m_ephemeris->GetGravitationalParameterCentralBody(GetName()));

   // Init the state vector
   SetStateVector(
      m_ephemeris->GetStateVector(GetName(), GetEpoch()));
}

////////////////////////////////////////////////////////////
test::StateVector MpcorbBody::VQueryStateVectorr(const Epoch& epoch)
{
   PropagateTo(epoch);
   return GetStateVector();

   //if (m_ephemeris)
   //{
   //   return m_ephemeris->GetStateVector(GetName(), epoch);
   //}
   //else
   //{
   //   OTL_ERROR() << "Failed to query state vector for mpcorb body " << Bracket(GetName())
   //      << ": Invalid ephemeris pointer.";
   //   return test::StateVector();
   //}
}

} // namespace otl