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

#pragma once
#include <OTL/Core/Flyby.hpp>

namespace otl
{

namespace keplerian
{

class FlybyUnpowered : public IFlybyAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the flyby maneuver
   ///
   /// Calculates the absolute velocity after performing an
   /// unpowered flyby (gravity assist). The altitude and
   /// BPlaneAngle define the shape and orientation of the flyby
   /// hyperbola respectively.
   ///
   /// \param approachVelocity Absolute velocity before the flyby
   /// \param orbitalBody Orbital body to flyby
   /// \param altitude Minimum altitude of the flyby hyperbola
   /// \param BPlaneAngle Orientation of the flyby hyperbola
   /// \param [out] departureVelocity Absolute velocity after the flyby
   ///
   ////////////////////////////////////////////////////////////
   virtual void Evaluate(const Vector3d& approachVelocity,
                         const OrbitalBody& orbitalBody,
                         double altitude,
                         double BPlaneAngle,
                         Vector3d& departureVelocity) override;

protected:
   Vector3d m_VInfinityIn;    ///< Temporary Vector3d for storing the relative approach velocity
   Vector3d m_VInfinityOut;   ///< Temporary Vector3d for storing the relative departure velocity
   Vector3d m_B1, m_B2, m_B3; ///< Temporary Vector3d's for storing intermediate data
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::FlybyUnpowered
/// \ingroup keplerian
///
/// Models the keplerian dynamics of an unpowered flyby.
///
/// Flybys (also refered to as gravity assist maneuvers)
/// work by exchanging momentum between a spacecraft and
/// an orbital body. A passing spacecraft may gain or lose
/// momentum simply by altering the direction in which
/// it passes by the orbital body. Stealing momentum from
/// the orbital body is effectively an orbit raising maneuver
/// (extends the apoapsis) whereas giving momentum to
/// the orbital body is effectively an orbit lowering
/// maneuver (shortens the periapsis).
///
/// An unpowered flyby differs from  a powered flyby in
/// which it assumes that there is no net change in
/// relative velocity before and after passing by the
/// orbital body (i.e. no thrusting).
///
/// There are many ways to define the orientation of the flyby
/// hyperbola in 3D space. This implementation is based on
/// the method developed by Dario Izzo which uses the turn angle
/// along with a B-Plane inclination angle. The B-Plane angle
/// is the angle of the hyperbolic plane relative to the "B"
/// reference frame which is defined as:
///
/// \f$ \hat{B_i} = \frac{V_\inf^-}{||V_\inf^-||} \f$\n
/// \f$ B_j = \hat{B_i} \times \frac{V_{planet}}{||V_{planet}||} \f$\n
/// \f$ \hat{B_j} = \frac{B_j}{||B_j||} \f$\n
/// \f$ B_k = \hat{B_i} \times \hat{B_j} \f$\n
/// \f$ \hat{B_k} = \frac{B_k}{||B_k||} \f$
///
/// \reference D. Izzo. Advances in global optimisation for space trajectory design.
/// Proceedings of 25th International Symposium on Space Technology and Science, 2006.
/// 
////////////////////////////////////////////////////////////