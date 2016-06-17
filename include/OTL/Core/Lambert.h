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
#include <OTL/Core/Base.h>
#include <OTL/Core/Orbit.h>
#include <OTL/Core/Time.h>

namespace otl
{

namespace keplerian
{

class OTL_CORE_API ILambertAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   ILambertAlgorithm() {}

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~ILambertAlgorithm() {}

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the solution to Lambert's Problem
   ///
   /// Calculates the initial and final velocity vectors given
   /// an initial position, final position, time delta, and
   /// number of full revolutions.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param initialPosition Vector3d consisting of the initial cartesian position
   /// \param finalPosition Vector3d consisting of the final cartesian position
   /// \param timeDelta Total time of flight between initial and final positions
   /// \param orbitDirection Either Orbit::Direction::Prograde or Orbit::Direciton::Retrograde
   /// \param numRevolutions Number of full revolutions performed over the timeDelta
   /// \param mu Gravitational parameter of the central body
   /// \param [out] initialVelocity Vector3d consisting of computed initial cartesian velocity
   /// \param [out] finalVelocity Vector3d consisting of computed final cartesian velocity
   ///
   ////////////////////////////////////////////////////////////
   virtual void Evaluate(const Vector3d& initialPosition,
                         const Vector3d& finalPosition,
                         const Time& timeDelta,
                         const Orbit::Direction& orbitDirection,
                         int numRevolutions,
                         double mu,
                         Vector3d& initialVelocity,
                         Vector3d& finalVelocity) = 0;

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate all solutions to Lambert's Problem up to a maximum number of revolutions
   ///
   /// Calculates all initial and final velocity vectors given
   /// an initial position, final position, time delta, and
   /// maximum number of full revolutions allowed.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param initialPosition Vector3d consisting of the initial cartesian position
   /// \param finalPosition Vector3d consisting of the final cartesian position
   /// \param timeDelta Total time of flight between initial and final positions
   /// \param orbitDirection Either Orbit::Direction::Prograde or Orbit::Direciton::Retrograde
   /// \param maxRevolutions Maximum number of full revolutions allowed
   /// \param mu Gravitational parameter of the central body
   /// \param [out] initialVelocity Vector of Vector3d's consisting of computed initial cartesian velocity
   /// \param [out] finalVelocity Vector of Vector3d's consisting of computed final cartesian velocity
   ///
   ////////////////////////////////////////////////////////////
   virtual void EvaluateAll(const Vector3d& initialPosition,
                            const Vector3d& finalPosition,
                            const Time& timeDelta,
                            const Orbit::Direction& orbitDirection,
                            int maxRevolutions,
                            double mu,
                            std::vector<Vector3d>& initialVelocity,
                            std::vector<Vector3d>& finalVelocity) = 0;
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::ILambertAlgorithm
/// \ingroup keplerian
///
/// Interface class for all Lambert algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////