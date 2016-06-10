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
#include <OTL/Core/StateVector.h>

namespace otl
{

// Forward declarations
class Time;

class OTL_CORE_API LagrangianPropagator
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the results of the Stumpff Functions
   ////////////////////////////////////////////////////////////
   struct StumpffParameters
   {
      double c2;  ///< Result of Stumpff function c2
      double c3;  ///< Result of Stumpff function c3
   };

   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the results of the Universal Variable iteration
   ////////////////////////////////////////////////////////////
   struct UniversalVariableResult
   {
      double x;                  ///< Universal variable
      double r;                  ///< Position magnitude
      double psi;                ///< Ratio of the universal variable squared and the semimajor axis
      StumpffParameters stumpff; ///< Stumpff function results
   };

   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the Lagrange Coefficients
   ////////////////////////////////////////////////////////////
   struct LagrangeCoefficients
   {
      double f;      ///< Lagrange coefficient F
      double g;      ///< Lagrange coefficient G
      double fDot;   ///< Derivative of Lagrange coefficient F
      double gDot;   ///< Derivative of Lagrange coefficient G
   };

   ////////////////////////////////////////////////////////////
   /// \brief Default Constructor
   ////////////////////////////////////////////////////////////
   LagrangianPropagator();

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~LagrangianPropagator();

   //virtual StateVectorType GetType() const override;

//protected:
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector in time using the Universal Variable and Lagrange coefficients
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
   /// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 8, section 2.3, page 101
   ///
   ////////////////////////////////////////////////////////////
   StateVector Propagate(const Vector3d& position, const Vector3d& velocity, double mu, const Time& timeDelta);
   StateVector Propagate(const StateVector& StateVector, double mu, const Time& timeDelta);
   //virtual StateVector VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu) override;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Calculate the universal variable
   ///
   /// Calculates the universal variable after
   /// propagating forwards or backwards in time.
   ///
   /// \reference Vallado [TODO]
   ///
   /// \param r0 Magnitude of position vector
   /// \param v0 Magnitude of velocity vector
   /// \param rdotv Dot product of position and velocity vectors
   /// \param seconds Propagation time in seconds
   /// \param mu Gravitational parameter of the central body
   /// \returns UniversalVariableResult object containing the final universal variable and other auxillory parameters
   ///
   ////////////////////////////////////////////////////////////
   UniversalVariableResult CalculateUniversalVariable(double r0, double v0, double rdotv, double seconds, double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Calculate an initial guess for the universal varable
   ///
   /// Calculates an initial guess for universal variable based on
   /// orbit type and shape.
   ///
   /// \reference Vallado [TODO]
   ///
   /// \param r0 Magnitude of position vector
   /// \param v0 Magnitude of velocity vector
   /// \param rdotv Dot product of position and velocity vectors
   /// \param alpha Reciprocal of semimajor axis
   /// \param seconds Propagation time in seconds
   /// \param mu Gravitational parameter of the central body
   /// \returns Universal variable initial guess
   ///
   ////////////////////////////////////////////////////////////
   double CalculateUniversableVariableInitialGuess(double r0, double v0, double rdotv, double alpha, double seconds, double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Calculate Stumpff Functions C2(psi) and C3(psi)
   ///
   /// Calculates the Stumpff Functions defined as (elliptical orbits):
   ///
   /// \f$ c_2(psi) = \frac{(1 - \cos{\sqrt{\psi}})}{\psi} \f$\n
   /// \f$ c_3(psi) = \frac{(\sqrt{\psi} - \sin{\sqrt{\psi}})}{\sqrt{\psi^3}} \f$
   ///
   /// The equations for hyperbolic orbits are similar, but
   /// they use hyperbolic sine and cosine functions instead.
   ///
   /// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 1, section 2.2, page 71
   ///
   /// \param psi Ratio of the unverisal variable squared and the semimajor axis
   /// \returns StumpffParameters object containing the results to the Stumpff functions, c2 & c3
   ///
   ////////////////////////////////////////////////////////////
   StumpffParameters CalculateStumpffParameters(double psi);

   ////////////////////////////////////////////////////////////
   /// \brief Calculate the Lagrange Coefficients
   ///
   /// Calculates the Lagrange Coefficients using the
   /// Universal Variable.
   ///
   /// \reference Vallado [TODO]
   ///
   /// \param r0 Magnitude of position vector
   /// \param seconds Propagation time in seconds
   /// \param sqrtMu Square root of the gravitational parameter of the central body
   /// \param results UniversalVariableResult containing the final Univeral Variable and other auxillary parameters
   /// \returns LagrangeCoefficients object containing the coefficients F, G, F', and G'
   ///
   ////////////////////////////////////////////////////////////
   LagrangeCoefficients CalculateLagrangeCoefficients(double r0, double seconds, double sqrtMu, const UniversalVariableResult& results);
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::LagrangianPropagator
/// \ingroup otl
///
/// Propagates a state vector forward or backwards in
/// time using the Universal Variable and Lagrange Coeficients.
/// All orbit types are inheritely supported. The Universal Variable
/// is computed using a Newton-Raphson iteration.
///
/// Usage example:
/// \code
/// auto propagator = std::make_shared<otl::LagrangianPropagator>();
///
/// // Setup inputs
/// StateVector StateVector;
/// StateVector.position = Vector3d(1000.0, 2000.0, 3000.0);  // Absolute position (km)
/// StateVector.velocity = Vector3d(1.0, 2.0, 3.0);           // Absolute velocity (km/s)
/// double mu = ASTRO_MU_SUN;                                          // Gravitational parameter of the Sun
/// Time timeDelta = Time::Days(150.0);                                // Propagate forward 150 days
///
/// StateVector initialStateVector = StateVector;
///
/// // Propagate the state vector forwards in time
/// auto finalStateVector = propagator->Propagate(initialStateVector, mu, timeDelta);
///
/// // Now propagate backwards in time to verify we end up where we started
/// auto initialStateVector2 = propagator->Propagate(finalStateVector, mu, -timeDelta);
///
/// OTL_ASSERT(initialStateVector == initialStateVector2);
/// \endcode
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 8, section 2.3, pages 101-102
/// 
////////////////////////////////////////////////////////////