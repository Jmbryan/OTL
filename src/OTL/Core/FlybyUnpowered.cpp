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

#include <OTL/Core/FlybyUnpowered.hpp>
#include <OTL/Core/OrbitalBody.hpp>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
void FlybyUnpowered::Evaluate(const Vector3d& approachVelocity,
                         const OrbitalBody& orbitalBody,
                         double altitude,
                         double BPlaneAngle,
                         Vector3d& departureVelocity)
{
   const Vector3d& planetVelocity = orbitalBody.GetVelocity();

   // VInfinityIn is the relative velocity of the object
   // as it approaches the planet.
   m_VInfinityIn = approachVelocity - planetVelocity;

   // The magnitude of this velocity vector remains constant throughout the flyby.
   double vInfinity = m_VInfinityIn.Magnitude();

   m_B1 = m_VInfinityIn;
   m_B1.Normalize();

   Vector3d::Cross(m_B1, planetVelocity / planetVelocity.Magnitude(), m_B2);
   m_B2.Normalize();

   Vector3d::Cross(m_B1, m_B2, m_B3);
   m_B3.Normalize();

   // Flyby hyperbola
   double radiusOfPeriapsis = orbitalBody.GetRadius() + altitude;
   double eccentricity = 1.0 + radiusOfPeriapsis * SQR(vInfinity) / orbitalBody.GetMu();

   double turnAngle = 2.0 * asin(1.0 / eccentricity);

   // VInfinityOut is the relative velocity of the object
   // as it departs from the planet.
   m_VInfinityOut = vInfinity * (m_B1 * cos(turnAngle) +
                                 m_B2 * cos(BPlaneAngle) * sin(turnAngle) +
                                 m_B3 * sin(BPlaneAngle) * sin(turnAngle));

   departureVelocity = planetVelocity + m_VInfinityOut;                     
}

} // namespace keplerian

} // namespace otl
