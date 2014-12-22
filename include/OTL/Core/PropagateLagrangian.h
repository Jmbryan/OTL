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
#include <OTL/Core/Propagate.h>

namespace otl
{

namespace keplerian
{

class PropagateLagrangian : public IPropagateAlgorithm
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital elements in time
   ///
   /// Calculates the final orbital elements after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// \param initialOrbitalElements OrbitalElements before propagation
   /// \param mu Gravitational parameter of the central body
   /// \param timeDelta Propgation time (may be negative)
   /// \param [out] finalOrbitalElements OrbitalElements after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual void Propagate(const OrbitalElements& initialOrbitalElements, double mu, const Time& timeDelta, OrbitalElements& finalOrbitalElements);
   
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector in time
   ///
   /// Calculates the final state vector after propagating
   /// forwards or backwards in time. Backwards propgation is
   /// achieved by setting a negative timeDelta.
   ///
   /// \param initialStateVector StateVector before propagation
   /// \param mu Gravitational parameter of the central body
   /// \param timeDelta Propgation time (may be negative)
   /// \param [out] finalStateVector StateVector after propagation
   ///
   ////////////////////////////////////////////////////////////
   virtual void Propagate(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector);

private:
   ////////////////////////////////////////////////////////////
   /// \brief Calculate the universal varable
   ///
   /// Calculates the universal variable after
   /// propagating forwards or backwards in time.
   ///
   /// \reference H. Curtis. Orbital Mechanics for Engineering Students
   /// Algorithm 3.3, (3.7)138-139
   ///
   /// \param r0 Magnitude of position vector
   /// \param vr0 Radial velocity component
   /// \param alpha Reciprocal semimajor axis
   /// \param timeDelta Propagation time
   /// \param mu Gravitational parameter of the central body
   /// \returns Universal variable
   ///
   ////////////////////////////////////////////////////////////
   double CalculateUniversalVariable(double r0, double vr0, double alpha, const Time& timeDelta, double mu); 

   ////////////////////////////////////////////////////////////
   /// \brief Calculate C2 and C3 (Stumpff Functions)
   ///
   /// Calculates the Stumpff Functions defined as (elliptical orbits):
   ///
   /// \f$ c_2 = \frac{(1 - \cos{\sqrt{\psi}})}{\psi} \f$\n
   /// \f$ c_3 = \frac{(\sqrt{\psi} - \sin{\sqrt{\psi}})}{\sqrt{\psi^3}} \f$
   ///
   /// The equations for hyperbolic orbits are similar,
   /// but use hyperbolic sine and cosine functions instead.
   ///
   /// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition
   /// Algorithm 1, (2.2)71, 2007.
   ///
   /// \param psi Ratio of the unverisal variable squared and the semimajor axis
   /// \param [out] c2 Solution to Stumpff function c2(psi)
   /// \param [out] c3 Solution to Stumpff function c3(psi)
   ///
   ////////////////////////////////////////////////////////////
   void CalculateC2C3(double psi, double& c2, double& c3);
};

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::PropagateLagrangian
/// \ingroup keplerian
///
/// Propagation using Lagrange coefficients and the Universal Variable.
///
/// Propagates a keplerian orbit forward or backwards in
/// time through the use of Lagrange coeficients and
/// universal variable (universal anomaly). All orbit types
/// are inheritely supported. Internally, Kepler's
/// equations are solved using a Newton-Raphson iteration.
///
/// Usage example:
/// \code
/// using otl;
/// using otl::keplerian;
///
/// IPropagateAlgorithm* propagator = new PropagateLagrangian();
///
/// // Setup inputs
/// StateVector initialStateVector;
/// initialStateVector.position = Vector3d(1000.0, 2000.0, 3000.0);  // Absolute position (km)
/// initialStateVector.velocity = Vector3d(1.0, 2.0, 3.0);           // Absolute velocity (km/s)
/// double mu = ASTRO_MU_SUN;                                        // Gravitational parameter of the Sun
/// Time timeDelta = Time::Days(150.0);                              // Propagate forward 150 days
///
/// // Setup output
/// StateVector finalStateVector = initialStateVector;
///
/// propagator->Propagate(finalStateVector,
///                       mu,
///                       timeDelta);
///
/// // Now propagate backwards in time to verify we end up where we started
/// otl::StateVector initialStateVector2 = finalStateVector;
///
/// propagator->Propagate(initialStateVector2,
///                       mu,
///                       -timeDelta);
///
/// OTL_ASSERT(initialStateVector == initialStateVector2);
/// OTL_SAFE_DELETE(propagator); 
/// \endcode
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition
/// Algorithm 8, (2.3)101-102, 2007.
/// 
////////////////////////////////////////////////////////////