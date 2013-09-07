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
inline void Orbit::SetMu(double mu)
{
   m_mu = mu;
}

////////////////////////////////////////////////////////////
inline void Orbit::SetStateVector(const StateVector& stateVector)
{
   m_stateVector = stateVector;  
   UpdateOrbitalElements();
   UpdateRadius();
   UpdateOrbitType();
}

////////////////////////////////////////////////////////////
inline void Orbit::SetOrbitalElements(const OrbitalElements& orbitalElements)
{
   m_orbitalElements = orbitalElements;  
   UpdateStateVector();
   UpdateRadius();
   UpdateOrbitType();
}

////////////////////////////////////////////////////////////
inline double Orbit::GetMu() const
{
   return m_mu;
}

////////////////////////////////////////////////////////////
inline const StateVector& Orbit::GetStateVector() const
{
   return m_stateVector;
}
////////////////////////////////////////////////////////////
inline const OrbitalElements& Orbit::GetOrbitalElements() const
{
   return m_orbitalElements;
}

////////////////////////////////////////////////////////////
inline double Orbit::GetRadius() const
{
   return m_radius;
}

////////////////////////////////////////////////////////////
inline Orbit::Type Orbit::GetType() const
{
   return m_orbitType;
}

////////////////////////////////////////////////////////////
inline void Orbit::UpdateStateVector()
{
   ConvertOrbitalElements2StateVector(m_orbitalElements, m_stateVector);
}

////////////////////////////////////////////////////////////
inline void Orbit::UpdateOrbitalElements()
{
   ConvertStateVector2OrbitalElements(m_stateVector, m_orbitalElements);
}

////////////////////////////////////////////////////////////
inline void Orbit::UpdateRadius()
{
   m_radius = sqrt(SQR(m_stateVector.position.x) * SQR(m_stateVector.position.y) + SQR(m_stateVector.position.z));
}