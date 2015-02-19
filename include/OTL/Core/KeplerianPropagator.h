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
#include <OTL/Core/Propagator.h>
#include <OTL/Core/KeplersEquations.h>

namespace otl
{

namespace keplerian
{

class OTL_CORE_API KeplerianPropagator : public otl::IPropagator
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default Constructor
   ////////////////////////////////////////////////////////////
   KeplerianPropagator();

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~KeplerianPropagator();

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector in time using Kepler's equations
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
   /// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 7, section 2.3, page 89
   ///
   ////////////////////////////////////////////////////////////
   virtual StateVector VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu) override;

private:
   keplerian::KeplersEquationElliptical m_keplerElliptical; ///< Functor  for solving Kepler's Equation for elliptical orbits
   keplerian::KeplersEquationHyperbolic m_keplerHyperbolic; ///< Functor  for solving Kepler's Equation for hyperbolic orbits
   //keplerian::KeplersequationsParabolic m_keplerParabolic; ///< Functor  for solving Kepler's Equation for parabolic orbits
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::KeplerianPropagator
/// \ingroup keplerian
///
/// Propagates a keplerian orbit forward or backwards in
/// time using the Kepler's equations. All orbit types are
/// inheritely supported.
///
/// Usage example:
/// \code
/// auto propagator = std::make_shared<otl::keplerian::KeplerianPropagator>();
///
/// // Setup inputs
/// OrbitalElements orbitalElements;
/// initialOrbitalElements.semimajorAxis = ..
/// ...
/// double mu = ASTRO_MU_SUN;                                        // Gravitational parameter of the Sun
/// Time timeDelta = Time::Days(150.0);                              // Propagate forward 150 days
///
/// StateVector initialStateVector = orbitalElements;
///
/// // Propagate the orbital elements forwards in time
/// auto finalStateVector = propagator->Propagate(initialStateVector, mu, timeDelta);
///
/// // Now propagate backwards in time to verify we end up where we started
/// auto initialStateVector2 = propagator->Propagate(finalStateVector, mu, -timeDelta);
///
/// OTL_ASSERT(initialStateVector == initialStateVector2);
/// \endcode
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 7, section 2.3, page 89
/// 
////////////////////////////////////////////////////////////