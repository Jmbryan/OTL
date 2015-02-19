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
#include <OTL/Core/Export.h>
#include <memory>

namespace otl
{

// Forward declarations
class Time;
class StateVector;

class OTL_CORE_API IPropagator
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   IPropagator();

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~IPropagator();

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector in time
   ///
   /// Calculates the final state vector after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// \param initialStateVector StateVector before propagation
   /// \param timeDelta Propgation time (may be negative)
   /// \param mu Gravitational parameter of the central body
   /// \return StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   StateVector Propagate(const StateVector& initialStateVector, const Time& timeDelta, double mu);

protected:
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
   /// \param timeDelta Propgation time (may be negative)
   /// \param mu Gravitational parameter of the central body
   /// \return StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual StateVector VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu) = 0;
};

typedef std::shared_ptr<IPropagator> PropagatorPointer;

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::IPropagator
///
/// Interface class for all propagation algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////