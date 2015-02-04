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

namespace otl
{

// Forward declarations
class OrbitalBody;

namespace keplerian
{

class OTL_CORE_API IFlybyAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   IFlybyAlgorithm() {}
   
   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~IFlybyAlgorithm() {}

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the flyby maneuver
   ///
   /// Calculates the absolute velocity after performing an
   /// unpowered flyby (gravity assist). The altitude and
   /// BPlaneAngle define the shape and orientation of the flyby
   /// hyperbola respectively.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param approachVelocity Absolute velocity before the flyby
   /// \param orbitalBody OrbitalBody to flyby
   /// \param altitude Minimum altitude of the flyby hyperbola
   /// \param BPlaneAngle Orientation of the flyby hyperbola
   /// \param [out] departureVelocity Absolute velocity after the flyby
   ///
   ////////////////////////////////////////////////////////////
   virtual void Evaluate(const Vector3d& approachVelocity,
                         const OrbitalBody& orbitalBody,
                         double altitude,
                         double BPlaneAngle,
                         Vector3d& departureVelocity) = 0;
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::IFlybyAlgorithm
/// \ingroup keplerian
///
/// Interface class for all flyby algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////