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

#include <OTL/Core/NaturalBody.h>
#include <OTL/Core/Ephemeris.h>
#include <OTL/Core/Logger.h>

namespace otl
{

////////////////////////////////////////////////////////////
NaturalBody::PhysicalProperties::PhysicalProperties() :
mass(1.0),
mu(ASTRO_GRAVITATIONAL_CONSTANT),
radius(1.0)

{

}

////////////////////////////////////////////////////////////
NaturalBody::PhysicalProperties::PhysicalProperties(double _mass, double _mu, double _radius) :
mass(_mass),
mu(_mu),
radius(_radius)

{

}

////////////////////////////////////////////////////////////
std::string NaturalBody::PhysicalProperties::ToString() const
{
   std::ostringstream os;
   os << "m=" << mass << " mu=" << mu << " r=" << radius << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
std::string NaturalBody::PhysicalProperties::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Mass:                    " << mass << std::endl;
   os << prefix << "Gravitational Parameter: " << mu << std::endl;
   os << prefix << "Equatorial Radius:       " << radius << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody() :
OrbitalBody(),
m_physicalProperties(),
m_useEphemerisForPropagation(false)
{

}

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch) :
OrbitalBody(name, mass),
m_physicalProperties(mass, mass * ASTRO_GRAVITATIONAL_CONSTANT, radius),
m_epoch(epoch),
m_useEphemerisForPropagation(false)
{

}

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch, const EphemerisPointer& ephemeris) :
OrbitalBody(name, mass),
m_physicalProperties(mass, mass * ASTRO_GRAVITATIONAL_CONSTANT, radius),
m_epoch(epoch),
m_ephemeris(ephemeris),
m_useEphemerisForPropagation(true)
{

}

////////////////////////////////////////////////////////////
NaturalBody::~NaturalBody()
{

}

////////////////////////////////////////////////////////////
void NaturalBody::SetPhysicalProperties(const PhysicalProperties& physicalProperties)
{
   m_physicalProperties = physicalProperties;
}

////////////////////////////////////////////////////////////
void NaturalBody::SetEpoch(const Epoch& epoch)
{
   m_epoch = epoch;
   if (m_useEphemerisForPropagation)
   {
      if (m_ephemeris)
      {
         StateVector newStateVector;
         m_ephemeris->GetStateVector(GetName(), m_epoch, newStateVector);
         SetStateVector(newStateVector);
      }
      else
      {
         OTL_ERROR() << "Ephemeris pointer invalid";
      }
   }
}

////////////////////////////////////////////////////////////
void NaturalBody::SetEphemeris(const EphemerisPointer& ephemeris)
{
    m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void NaturalBody::UseEphemerisForPropagation(bool useEphemerisForPropagation)
{
    m_useEphemerisForPropagation = useEphemerisForPropagation;
}

////////////////////////////////////////////////////////////
const Epoch& NaturalBody::GetEpoch() const
{
   return m_epoch;
}

////////////////////////////////////////////////////////////
const NaturalBody::PhysicalProperties& NaturalBody::GetPhysicalProperties() const
{
   return m_physicalProperties;
}

////////////////////////////////////////////////////////////
void NaturalBody::Propagate(const Time& timeDelta)
{
    if (m_useEphemerisForPropagation)
    {
        if (m_ephemeris)
        {
            Epoch newEpoch = m_epoch + timeDelta;
            StateVector newStateVector;
            m_ephemeris->GetStateVector(GetName(), newEpoch, newStateVector);
            SetStateVector(newStateVector);
        }
        else
        {
            OTL_ERROR() << "Ephemeris pointer invalid";
        }
    }
    else
    {
        OrbitalBody::Propagate(timeDelta);
    }
}

////////////////////////////////////////////////////////////
std::string NaturalBody::ToString() const
{
   std::ostringstream os;
   os << "name=" << GetName() << " epoch=" << GetEpoch();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string NaturalBody::ToDetailedString(std::string prefix) const
{
   std::ostringstream os;
   os << prefix << "Name: " << GetName() << std::endl;
   os << prefix << "Physical Properties: " << std::endl;
   os << GetPhysicalProperties().ToDetailedString(prefix + "   ");
   os << prefix << "Epoch:" << std::endl;
   os << GetEpoch().ToDetailedString(prefix + "   ");
   os << prefix << "Orbit:" << std::endl;
   os << GetOrbit().ToDetailedString(prefix + "   ");

   return os.str();
}

} // namespace otl