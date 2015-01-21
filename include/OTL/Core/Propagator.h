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

namespace keplerian
{

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
   /// \brief Propagate the orbital elements in time
   ///
   /// Calculates the final orbital elements after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// \param initialOrbitalElements OrbitalElements before propagation
   /// \param timeDelta Propgation time (may be negative)
   /// \param mu Gravitational parameter of the central body
   /// \returns OrbitalElements after propagation
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements Propagate(const OrbitalElements& initialOrbitalElements, const Time& timeDelta, double mu);
     
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
   /// \returns StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   StateVector Propagate(const StateVector& initialStateVector, const Time& timeDelta, double mu);

protected:
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
   /// \param timeDelta Propgation time (may be negative)
   /// \param mu Gravitational parameter of the central body
   /// \returns OrbitalElements after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual OrbitalElements VPropagate(const OrbitalElements& initialOrbitalElements, const Time& timeDelta, double mu) = 0;

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
   /// \returns StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual StateVector VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu) = 0;
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::IPropagator
/// \ingroup keplerian
///
/// Interface class for all propagation algorithms.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////