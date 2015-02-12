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

#include <OTL/Core/EphemerisBody.h>

namespace otl
{

////////////////////////////////////////////////////////////
IEphemerisBody::IEphemerisBody() :
OrbitalBody(),
m_initialized(false),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
IEphemerisBody::IEphemerisBody(const std::string& name,
                               const PhysicalProperties& physicalProperties,
                               double gravitationalParameterCentralBody,
                               const test::StateVector& stateVector,
                               const Epoch& epoch) :
OrbitalBody(name, physicalProperties, gravitationalParameterCentralBody, stateVector, epoch),
m_initialized(false),
m_maxPropagationTime(Time::Infinity())
{

}

////////////////////////////////////////////////////////////
IEphemerisBody::~IEphemerisBody()
{

}

////////////////////////////////////////////////////////////
PhysicalProperties IEphemerisBody::GetPhysicalProperties() const
{
   if (!m_initialized)
   {
      const_cast<IEphemerisBody*>(this)->ForceInitialize();
   }
   return OrbitalBody::GetPhysicalProperties();
}

////////////////////////////////////////////////////////////
keplerian::Orbit IEphemerisBody::GetOrbit() const
{
   if (!m_initialized)
   {
      const_cast<IEphemerisBody*>(this)->ForceInitialize();
   }
   return OrbitalBody::GetOrbit();
}

////////////////////////////////////////////////////////////
void IEphemerisBody::BlendedPropagate(const Time& timeDelta)
{
   if (IsEphemerisUpdateRequired(timeDelta))
   {
      QueryStateVector(GetEpoch());
   }
   else
   {
      OrbitalBody::Propagate(timeDelta);
   }
}

////////////////////////////////////////////////////////////
void IEphemerisBody::BlendedPropagateTo(const Epoch& epoch)
{
   BlendedPropagate(epoch - GetEpoch());
}

////////////////////////////////////////////////////////////
void IEphemerisBody::SetMaxPropagationTime(const Time& maxTime)
{
   m_maxPropagationTime = maxTime;
}

////////////////////////////////////////////////////////////
void IEphemerisBody::ForceInitialize()
{
   m_initialized = true;
   VInitialize();
}

////////////////////////////////////////////////////////////
const test::StateVector& IEphemerisBody::QueryStateVector(const Epoch& epoch)
{
   if (!m_initialized)
   {
      const_cast<IEphemerisBody*>(this)->ForceInitialize();
   }
   SetStateVector(VQueryStateVector(epoch));
   return GetStateVector();
}

////////////////////////////////////////////////////////////
bool IEphemerisBody::IsEphemerisUpdateRequired(const Time& timeDelta)
{
   return (VGetEphemeris() && (GetOrbit().GetElapsedPropagationTime() + timeDelta) > m_maxPropagationTime);
}

} // namespace otl