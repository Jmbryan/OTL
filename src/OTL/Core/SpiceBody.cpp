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

#include <OTL/Core/SpiceBody.h>
#include <OTL/Core/SpiceEphemeris.h>

namespace otl
{

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody() :
IEphemerisBody(),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& observerBodyName,
                     const Epoch& epoch,
                     const std::string& targetBodyName,
                     const std::string& referenceFrameName) :
IEphemerisBody(observerBodyName, PhysicalProperties(), 1.0, StateVector(), epoch),
m_ephemeris(nullptr)
{

}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& observerBodyName,
                     const SpiceEphemerisPointer& ephemeris,
                     const Epoch& epoch,
                     const std::string& targetBodyName,
                     const std::string& referenceFrameName) :
IEphemerisBody(observerBodyName, PhysicalProperties(), 1.0, StateVector(), epoch),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void SpiceBody::SetEphemeris(const SpiceEphemerisPointer& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void SpiceBody::VInitialize()
{
   // Init the ephemeris
   if (!m_ephemeris)
   {
      m_ephemeris = std::make_shared<SpiceEphemeris>();
   }
   
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
EphemerisPointer SpiceBody::VGetEphemeris()
{
   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
}

////////////////////////////////////////////////////////////
StateVector SpiceBody::VQueryStateVector(const Epoch& epoch)
{
   if (m_ephemeris)
   {
      return m_ephemeris->GetStateVector(GetName(), epoch);
   }
   else
   {
      OTL_ERROR() << "Failed to query state vector for spice body " << Bracket(GetName())
         << ": Invalid ephemeris pointer.";
      return StateVector();
   }
}

} // namespace otl