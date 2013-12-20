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
inline OrbitalBody::OrbitalBody() :
m_name(""),
m_radius(0.0),
m_mu(0.0)
{
}

////////////////////////////////////////////////////////////
inline OrbitalBody::OrbitalBody(const std::string& name, double radius, double mu) :
m_name(name),
m_radius(radius),
m_mu(mu)
{
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::SetName(const std::string& name)
{
   m_name = name;
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::SetRadius(double radius)
{
   m_radius = radius;
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::SetMu(double mu)
{
   m_mu = mu;
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::SetEpoch(const Epoch& epoch)
{
   m_epoch = epoch;
}

////////////////////////////////////////////////////////////
inline std::string OrbitalBody::GetName() const
{
   return m_name;
}

////////////////////////////////////////////////////////////
inline double OrbitalBody::GetMu() const
{
   return m_mu;
}

////////////////////////////////////////////////////////////
inline double OrbitalBody::GetRadius() const
{
   return m_radius;
}

////////////////////////////////////////////////////////////
inline const Epoch& OrbitalBody::GetEpoch() const
{
   return m_epoch;
}

////////////////////////////////////////////////////////////
inline const StateVector& OrbitalBody::GetStateVector() const
{
   return m_stateVector;
}

////////////////////////////////////////////////////////////
inline const Vector3d& OrbitalBody::GetPosition() const
{
   return m_stateVector.position;
}

////////////////////////////////////////////////////////////
inline const Vector3d& OrbitalBody::GetVelocity() const
{
   return m_stateVector.velocity;
}