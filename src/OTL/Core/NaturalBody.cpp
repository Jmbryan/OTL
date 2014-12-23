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

namespace otl
{

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody() :
OrbitalBody(),
m_radius(1.0),
m_useEphemerisForPropagation(false)
{

}

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch) :
OrbitalBody(name, mass),
m_radius(radius),
m_epoch(epoch),
m_useEphemerisForPropagation(false)
{

}

////////////////////////////////////////////////////////////
NaturalBody::NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch, const EphemerisPointer& ephemeris) :
OrbitalBody(name, mass),
m_radius(radius),
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
void NaturalBody::SetRadius(double radius)
{
   m_radius = radius;
}

////////////////////////////////////////////////////////////
void NaturalBody::SetEpoch(const Epoch& epoch)
{
   m_epoch = epoch;
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
double NaturalBody::GetRadius() const
{
   return m_radius;
}

////////////////////////////////////////////////////////////
const Epoch& NaturalBody::GetEpoch() const
{
   return m_epoch;
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
            m_ephemeris->QueryDatabase(GetName(), newEpoch, newStateVector);
            SetStateVector(newStateVector);
        }
        else
        {
            //throw InvalidPointerExeption("Ephemeris invalid");
        }
    }
    else
    {
        OrbitalBody::Propagate(timeDelta);
    }
}

} // namespace otl