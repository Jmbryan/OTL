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

#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
KeplerianPropagator::KeplerianPropagator() :
IPropagator()
{

}

////////////////////////////////////////////////////////////
KeplerianPropagator::~KeplerianPropagator()
{

}

////////////////////////////////////////////////////////////
StateVectorType KeplerianPropagator::GetType() const
{
   return StateVectorType::Orbital;
}

////////////////////////////////////////////////////////////
StateVector KeplerianPropagator::VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu)
{  
   // Convert the state vector to orbital elements
   const auto& initialOrbitalElements = initialStateVector.GetOrbitalElements();

   // Unpack relevent orbital elements
   const double a = initialOrbitalElements.semiMajorAxis;
   const double e = initialOrbitalElements.eccentricity;
   const double TA1 = initialOrbitalElements.trueAnomaly;

   // Solve Kepler's Equation depending on orbit type
   double TA2 = 0.0;
   if (IsCircularOrElliptical(e))
   {
      // Compute mean motion
      double n = sqrt(mu / pow(a, 3.0));

      // Convert true anomaly to mean anomaly
      double E1 = ConvertTrueAnomaly2EccentricAnomaly(e, TA1);
      double M1 = ConvertEccentricAnomaly2MeanAnomaly(e, E1);

      // Propagate the mean anomaly using the mean motion
      double M2 = M1 + n * timeDelta.Seconds();
      M2 = Modulo(M2, MATH_2_PI);

      // Convert back to true anomaly
      keplerian::KeplersEquationElliptical kepler;
      double E2 = kepler.Evaluate(e, M2);
      TA2 = ConvertEccentricAnomaly2TrueAnomaly(e, E2);
   }
   else if (IsHyperbolic(e))
   {
      // Compute mean motion
      double n = sqrt(mu / pow(-a, 3.0));

      // Convert true anomaly to mean anomaly
      double H1 = ConvertTrueAnomaly2HyperbolicAnomaly(e, TA1);
      double M1 = ConvertHyperbolicAnomaly2MeanAnomaly(e, H1);

      // Propagate the mean anomaly using the mean motion
      double M2 = M1 + n * timeDelta.Seconds();
      M2 = Modulo(M2, MATH_2_PI);

      // Convert back to true anomaly
      keplerian::KeplersEquationHyperbolic kepler;
      double H2 = kepler.Evaluate(e, M2);
      TA2 = ConvertHyperbolicAnomaly2TrueAnomaly(e, H2);
   }
   else // Parabolic
   {
      OTL_ERROR() << "Parabolic orbits are not supported";
   }

   return StateVector(
      a,
      e,
      TA2,
      initialOrbitalElements.inclination,
      initialOrbitalElements.argOfPericenter,
      initialOrbitalElements.lonOfAscendingNode,
      StateVectorType::Orbital);
}

////////////////////////////////////////////////////////////
// Alternative implementations/approaches
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Slightly less performanant than Keplers equation approach
//OrbitalElements KeplerianPropagator::VPropagate(const OrbitalElements& initialOrbitalElements, const Time& timeDelta, double mu)
//{
//   // Unpack the inputs
//   const double a = initialOrbitalElements.semiMajorAxis;
//   const double e = initialOrbitalElements.eccentricity;
//   double TA0 = initialOrbitalElements.trueAnomaly;
//
//   // Compute frequently used variables
//   double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA0));
//   double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
//   double vr0 = (sqrt(mu) * e * sin(TA0)) / sqrt(a * (1.0 - SQR(e)));
//   double rdotv = r0 * vr0;
//
//   // Compute the Universal variable results
//   auto results = CalculateUniversalVariable(r0, v0, rdotv, timeDelta.Seconds(), mu);
//   const double& x = results.x;
//
//   // Propagate the true anomaly depending on orbit type
//   double TA;
//   if (e < (1.0 - MATH_TOLERANCE)) // Circular or elliptical
//   {
//      double E0 = ConvertTrueAnomaly2EccentricAnomaly(e, TA0);
//      double E = E0 + x / sqrt(a);
//      TA = ConvertEccentricAnomaly2TrueAnomaly(e, E);
//   }
//   else if (e > (1.0 + MATH_TOLERANCE)) // Hyperbolic
//   {
//      double H0 = ConvertTrueAnomaly2HyperbolicAnomaly(e, TA0);
//      double H = H0 + x / sqrt(-a);
//      TA = ConvertHyperbolicAnomaly2TrueAnomaly(e, H);
//   }
//   else // Parabolic
//   {
//      double h = 0.5 * SQR(v0) - mu / r0;
//      double P0 = ConvertTrueAnomaly2ParabolicAnomaly(TA0);
//      double P = P0 + x * sqrt(mu) / h;
//      TA = ConvertParabolicAnomaly2TrueAnomaly(P);
//   }
//
//   // Compute the final orbital elements
//   OrbitalElements finalOrbitalElements = initialOrbitalElements;
//   finalOrbitalElements.trueAnomaly = TA;
//
//   return finalOrbitalElements;
//}

////////////////////////////////////////////////////////////
// Much less performant than universal variable approach, but included for completeness
// Can the deltaTA and r be computed more effienently? i.e. without calling ConvertCartesianStateVector2OrbitalElements()
// Results are not matching Propagate()
//void KeplerianPropagator::PropagateK(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector)
//{
//   // Unpack the inputs
//   const Vector3d& R = initialStateVector.position;
//   const Vector3d& V = initialStateVector.velocity;
//
//   // Compute frequently used variables
//   double r0 = R.norm();
//   double v0 = V.norm();
//   double h = 0.5 * SQR(v0) - mu / r0;
//   double hSquared = SQR(h);
//
//   // Convert to orbital elements
//   auto initialOrbitalElements = ConvertCartesianStateVector2OrbitalElements(initialStateVector, mu);
//
//   // Propagate the orbital elements
//   OrbitalElements finalOrbitalElements;
//   PropagateK(initialOrbitalElements, mu, timeDelta, finalOrbitalElements);
//
//   double e = initialOrbitalElements.eccentricity;
//   double TA0 = initialOrbitalElements.trueAnomaly;
//   double TA = finalOrbitalElements.trueAnomaly;
//
//   // Compute change in true anomaly and final position magnitude
//   double deltaTA = TA - TA0;
//   double r = (hSquared / mu) / (1.0 + e * cos(TA));
//
//   double cosDeltaTA = cos(deltaTA);
//   double sinDeltaTA = sin(deltaTA);
//
//   // Compute the Lagrange coefficients
//   // Ref: Curtis equations 2.148a-d
//   LagrangeCoefficients coeff;
//   coeff.f = 1.0 - mu * r / hSquared * (1.0 - cosDeltaTA);
//   coeff.g = r * r0 / h * sinDeltaTA;
//   coeff.fDot = mu / h * (1.0 - cosDeltaTA) / sinDeltaTA * (mu / hSquared * (1.0 - cosDeltaTA) - 1.0 / r0 - 1.0 / r);
//   coeff.gDot = 1.0 - mu * r0 / hSquared * (1.0 - cosDeltaTA);
//
//   // Compute the final state vector
//   finalStateVector.position = coeff.f    * R + coeff.g    * V;
//   finalStateVector.velocity = coeff.fDot * R + coeff.gDot * V;
//}

////////////////////////////////////////////////////////////
// Nnearly identical performance-wise as Vallado's implementation
// Vallado iterates for the error instead of the ratio which is a little more intuiative
/// \reference H. Curtis. Orbital Mechanics for Engineering Students. Algorithm 3.3, section 3.7, page 138-139
//KeplerianPropagator::UniversalVariableResult
//KeplerianPropagator::CalculateUniversalVariableCurtis(double r0, double v0, double rdotv, double seconds, double mu)
//{
//   // Create aliases for results
//   UniversalVariableResult results;
//   double& x = results.x;
//   //double& r = results.r;
//   double& psi = results.psi;
//   double& c2 = results.stumpff.c2;
//   double& c3 = results.stumpff.c3;
//   auto& stumpff = results.stumpff;
//
//   // Compute frequently used variables
//   double sqrtMu = sqrt(mu);
//   double alpha = 2.0 / r0 - SQR(v0) / mu; // Reciprocal of semi-major axis
//   double vr0 = rdotv / r0;
//
//   // Compute the initial guess depending on orbit type
//   x = CalculateUniversableVariableInitialGuess(r0, v0, rdotv, alpha, seconds, mu);
//
//   // Solve using Newton-Raphson iteration
//   int iter = 0;
//   const int MAX_ITERATIONS = 100;
//   double ratio = MATH_INFINITY;
//   while (abs(ratio) > MATH_TOLERANCE && iter++ < MAX_ITERATIONS)
//   {
//      double xSquared = SQR(x);
//      psi = xSquared * alpha;
//      stumpff = CalculateStumpffParameters(psi);
//      double F = r0 * vr0 / sqrtMu * xSquared * c2 + (1.0 - alpha * r0) * pow(x, 3.0) * c3 + r0 * x - sqrtMu * seconds;
//      double dFdx = r0 * vr0 / sqrtMu * x * (1.0 - alpha * xSquared * c3) + (1.0 - alpha * r0) * xSquared * c2 + r0;
//      ratio = F / dFdx;
//      x -= ratio;
//   }
//
//   if (iter >= MAX_ITERATIONS)
//   {
//      OTL_WARN() << "KeplerianPropagator::CalculateUniversalVariableCurtis(): Max iterations reached with ratio " << Bracket(abs(ratio));
//   }
//
//   return results;
//}

} // namespace keplerian

} // namespace otl