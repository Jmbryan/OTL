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
#include <OTL/Core/Lambert.hpp>

namespace otl
{

namespace keplerian
{

class LambertExponentialSinusoid : public ILambertAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Evaluate Lambert's Problem
   ///
   /// Calculates the initial and final velocity vectors given
   /// an initial position, final position, and time delta.  
   ///
   /// \param initialPosition Vector3d consisting of the initial position
   /// \param finalPosition Vector3d consisting of the final position
   /// \param timeDelta Total time of flight between initial and final positions
   /// \param orbitDirection Either Orbit::Direction::Prograde or Orbit::Direciton::Retrograde
   /// \param maxRevolutions Maximum number of revolutions allowed
   /// \param mu Gravitational parameter of the central body
   /// \param [out] initialVelocity Vector3d consisting of computed initial velocity
   /// \param [out] finalVelocity Vector3d consisting of computed final velocity
   ///
   ////////////////////////////////////////////////////////////
    virtual void Evaluate(const Vector3d& initialPosition,
                          const Vector3d& finalPosition,
                          const Time& timeDelta,
                          const Orbit::Direction& orbitDirection,
                          int maxRevolutions,
                          double mu,
                          Vector3d& initialVelocity,
                          Vector3d& finalVelocity);

private:
   ////////////////////////////////////////////////////////////
   /// \brief Calculate the time of flight
   ///
   /// \param x
   /// \param s
   /// \param c
   /// \param longway Equal to 1 for the longway, -1 for the shortway
   /// \param maxRevolutions Maximum number of revolutions allowed
   /// \returns time of flight
   ///
   ////////////////////////////////////////////////////////////
    double CalculateTimeOfFlight(double x, double s, double c, int longway, int maxRevolutions);
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::LambertExponentialSinusoid
/// \ingroup keplerian
///
/// Implements Lambert's problem using exponential sinusoids.
/// In this method, trajectories are modeled using exponential
/// sinusoids as opposed to classic ballistic arcs (i.e. conic
/// sections). An exponential sinusoid is defined as:
///
/// \f$ r = k_0 e^{[k_1\sin{(k_2 \theta) + \phi}]} \f$
///
/// It was found that given an initial and final position, transfer
/// angle, and maximum number of revolutions, then for all
/// values of k2 there exist a class of exponential sinusoids
/// that pass through both points parameterized only by the
/// flight path angle.
///
/// Usage example:
/// \code
/// using otl;
/// using otl::keplerian;
///
/// ILambertAlgorithm* lambert = new LambertExponentialSinusoid();
///
/// // Setup the inputs:
/// Vector3d initialPosition = Vector3d(1.0, 2.0, 3.0);      // Initial absolute position (km)
/// Vector3d finalPosition = Vector3d(4.0, 5.0, 6.0);        // Final absolute position (km)
/// Time timeDelta = Time::Days(150.0);                      // Total time of flight is 150 days
/// Orbit::Direction orbitDirection = otl::Orbit::Prograde;  // Prograde orbit
/// int maxRevolutions = 1;                                  // Allow at most 1 full revolution
/// double mu = ASTRO_MU_SUN;                                // Gravitational parameter of the Sun
///
/// // Setup the outputs:
/// Vector3d initialVelocity, finalVelocity;
///
/// // Evaluate Lambert's problem
/// lambert->Evaluate(initialPosition,
///                   finalPosition,
///                   timeDelta,
///                   orbitDirection,
///                   maxRevolutions,
///                   mu,
///                   initialVelocity,
///                   finalVelocity);
///
/// OTL_SAFE_DELETE(lambert);
/// \endcode
///
/// \reference D. Izzo. Lambert's problem for exponential sinusoids.
/// Journal of Guidance Control and Dynamics, 29(5):1242-1245, 2006.
/// 
////////////////////////////////////////////////////////////