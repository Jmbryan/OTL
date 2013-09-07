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


////////////////////////////////////////////////////////////
inline Planet::PlanetInfo::PlanetInfo() :
name(""),
radius(0.0),
mu(0.0)
{
}

////////////////////////////////////////////////////////////
inline Planet::PlanetInfo::PlanetInfo(const std::string& Name, double Radius, double Mu) :
name(Name),
radius(Radius),
mu(Mu)
{
}

////////////////////////////////////////////////////////////
inline Planet::Planet() :
OrbitalBody(),
m_id(PlanetId::Invalid)
{
}

////////////////////////////////////////////////////////////
inline Planet::Planet(Planet::PlanetId planetId) :
OrbitalBody(),
m_id(planetId)
{
   Initialize(planetId);
}

////////////////////////////////////////////////////////////
inline Planet::Planet(const std::string& name) :
OrbitalBody(),
m_id(PlanetId::Invalid)
{
   m_id = ConvertName2Identifier(name);
   Initialize(m_id);
}

////////////////////////////////////////////////////////////
inline std::string Planet::ConvertIdentifier2Name(PlanetId planetId)
{
   assert(planetId > PlanetId::Invalid && planetId < PlanetId::Count);
   return m_planetInfo.at(planetId).name;
}

////////////////////////////////////////////////////////////
inline void Planet::GetStateVectorsAtEpoch(const Epoch& epoch, StateVector& stateVector) const
{
}

////////////////////////////////////////////////////////////
inline void Planet::GetOrbitalElementsAtEpoch(const Epoch& epoch, OrbitalElements& orbitalElements) const
{
}