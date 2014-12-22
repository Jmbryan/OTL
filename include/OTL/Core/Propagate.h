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

namespace otl
{

namespace keplerian
{

class IPropagateAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   IPropagateAlgorithm() {}

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~IPropagateAlgorithm() {}

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital elements in time
   ///
   /// Calculates the final orbital elements after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param initialOrbitalElements OrbitalElements before propagation
   /// \param mu Gravitational parameter of the central body
   /// \param timeDelta Propgation time (may be negative)
   /// \param [out] finalOrbitalElements OrbitalElements after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual void Propagate(const OrbitalElements& initialOrbitalElements, double mu, const Time& timeDelta, OrbitalElements& finalOrbitalElements) = 0;
   
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector in time
   ///
   /// Calculates the final state vector after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param initialStateVector StateVector before propagation
   /// \param mu Gravitational parameter of the central body
   /// \param timeDelta Propgation time (may be negative)
   /// \param [out] finalStateVector StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual void Propagate(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector) = 0;
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::IPropagateAlgorithm
/// \ingroup keplerian
///
/// Interface class for all propagation algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////