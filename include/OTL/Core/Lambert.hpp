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
#include <OTL/Core/Base.hpp>
#include <OTL/Core/Orbit.hpp>

namespace otl
{

class ILambertAlgorithm
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
   /// \brief Evaluate Lambert's Problem
   ///
   /// Calculates the initial and final velocity vectors given
   /// an initial position, final position, and time delta.  
   ///
   /// This is a pure virtual function that muse be re-implemented
   /// by the derived class.
   ///
   /// \param initialPosition Vector3d consisting of the initial position
   /// \param finalPosition Vector3d consisting of the final position
   /// \param seconds Total time of flight in seconds
   /// \param orbitDirection Either Orbit::Direction::Prograde or Orbit::Direciton::Retrograde
   /// \param maxRevolutions Maximum number of revolutions allowed
   /// \param mu Gravitational parameter of the central body
   /// \param [out] initialVelocity Vector3d consisting of computed initial velocity
   /// \param [out] finalVelocity Vector3d consisting of computed final velocity
   ///
   ////////////////////////////////////////////////////////////
   virtual void Evaluate(const Vector3d& initialPosition,
                         const Vector3d& finalPosition,
                         double seconds,
                         const Orbit::Direction& orbitDirection,
                         int maxRevolutions,
                         double mu,
                         Vector3d& initialVelocity,
                         Vector3d& finalVelocity) = 0;
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::ILambertAlgorithm
/// \ingroup core
///
/// otl::ILambertAlgorithm is a simple class that defines the interface
/// for all Lambert algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////