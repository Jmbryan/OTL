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

#include <OTL/Core/Orbit.h>
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
OrbitalElements KeplerianPropagator::VPropagate(const OrbitalElements& initialOrbitalElements, const Time& timeDelta, double mu)
{
   // Unpack the inputs
   const double a = initialOrbitalElements.semiMajorAxis;
   const double e = initialOrbitalElements.eccentricity;
   double TA0 = initialOrbitalElements.trueAnomaly;

   // Compute frequently used variables
   double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA0));
   double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
   double vr0 = (sqrt(mu) * e * sin(TA0)) / sqrt(a * (1.0 - SQR(e)));
   double rdotv = r0 * vr0;

   // Compute the Universal variable results
   auto results = CalculateUniversalVariable(r0, v0, rdotv, timeDelta.Seconds(), mu);
   const double& x = results.x;

   // Propagate the true anomaly depending on orbit type
   double TA;
   if (e < (1.0 - MATH_TOLERANCE)) // Circular or elliptical
   {
      double E0 = ConvertTrueAnomaly2EccentricAnomaly(e, TA0);
      double E = E0 + x / sqrt(a);
      TA = ConvertEccentricAnomaly2TrueAnomaly(e, E);
   }
   else if (e > (1.0 + MATH_TOLERANCE)) // Hyperbolic
   {
      double H0 = ConvertTrueAnomaly2HyperbolicAnomaly(e, TA0);
      double H = H0 + x / sqrt(-a);
      TA = ConvertHyperbolicAnomaly2TrueAnomaly(e, H);
   }
   else // Parabolic
   {
      double h = 0.5 * SQR(v0) - mu / r0;
      double P0 = ConvertTrueAnomaly2ParabolicAnomaly(TA0);
      double P = P0 + x * sqrt(mu) / h;
      TA = ConvertParabolicAnomaly2TrueAnomaly(P);
   }

   // Compute the final orbital elements
   OrbitalElements finalOrbitalElements = initialOrbitalElements;
   finalOrbitalElements.trueAnomaly = TA;

   return finalOrbitalElements;
}

StateVector KeplerianPropagator::VPropagate(const StateVector& initialStateVector, const Time& timeDelta, double mu)
{
   // Unpack the inputs
   const Vector3d& R = initialStateVector.position;
   const Vector3d& V = initialStateVector.velocity;
   double seconds = timeDelta.Seconds();

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double r0 = R.norm();
   double v0 = V.norm();
   double rdotv = R.dot(V);

   // Compute the universal variable results
   auto results = CalculateUniversalVariable(r0, v0, rdotv, seconds, mu);

   // Compute the Lagrange coefficients
   auto coeff = CalculateLagrangeCoefficients(r0, seconds, sqrtMu, results);

   // Compute the final state vector
   StateVector finalStateVector;
   finalStateVector.position = coeff.f    * R + coeff.g    * V;
   finalStateVector.velocity = coeff.fDot * R + coeff.gDot * V;

   return finalStateVector;
}

////////////////////////////////////////////////////////////
// Vallado, preferred method but nearly identical performance-wise as Curtis
KeplerianPropagator::UniversalVariableResult
KeplerianPropagator::CalculateUniversalVariable(double r0, double v0, double rdotv, double seconds, double mu)
{
   // Create aliases for results
   UniversalVariableResult results;
   double& x = results.x;
   double& r = results.r;
   double& psi = results.psi;
   double& c2 = results.stumpff.c2;
   double& c3 = results.stumpff.c3;
   auto& stumpff = results.stumpff;

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double alpha = 2.0 / r0 - SQR(v0) / mu; // Reciprocal of semi-major axis

   // Compute the initial guess depending on orbit type
   x = CalculateUniversableVariableInitialGuess(r0, v0, rdotv, alpha, seconds, mu);

   // Solve using Newton-Raphson iteration
   int iter = 0;
   const int MAX_ITERATIONS = 100;
   double xPrev, error = MATH_INFINITY;
   while (error >= MATH_TOLERANCE && iter++ < MAX_ITERATIONS)
   {
      xPrev = x;
      double xSquared = SQR(x);
      psi = xSquared * alpha;
      stumpff = CalculateStumpffParameters(psi);
      r = xSquared * c2 + (rdotv / sqrtMu) * x * (1.0 - psi * c3) + r0 * (1.0 - psi * c2);
      x += (sqrtMu * seconds - pow(x, 3.0) * c3 - rdotv / sqrtMu * xSquared * c2 - r0 * x * (1.0 - psi * c3)) / r;
      error = fabs(x - xPrev);
   }

   if (iter == MAX_ITERATIONS)
   {
      OTL_WARN() << "KeplerianPropagator::CalculateUniversalVariable(): Max iterations reached with error " << Bracket(error);
   }

   return results;
}

////////////////////////////////////////////////////////////
double KeplerianPropagator::CalculateUniversableVariableInitialGuess(double r0, double v0, double rdotv, double alpha, double seconds, double mu)
{
   double x = 1.0;
   double alphaThreshold = 0.000001 * (ASTRO_MU_EARTH / mu); // Reference?
   if (alpha > alphaThreshold) // Circle or Ellipse
   {
      x = sqrt(mu) * alpha * seconds;
   }
   else if (alpha < -alphaThreshold) // Hyperbola
   {
      double a = 1.0 / alpha;
      x = Sign(seconds) * sqrt(-a) * log((-2.0 * mu * alpha * seconds) /
         (rdotv + Sign(seconds) * sqrt(-mu * a) * (1.0 - r0 * alpha)));
   }
   else // Parabola
   {
      double h = 0.5 * SQR(v0) - mu / r0;
      double p = SQR(h) / mu;
      double s = 0.5 * acot(3.0 * sqrt(mu / pow(p, 3.0)) * seconds);
      double w = pow(tan(s), 1.0 / 3.0);
      x = 2.0 * sqrt(p) * cot(2.0 * w);
   }

   return x;
}

////////////////////////////////////////////////////////////
KeplerianPropagator::StumpffParameters KeplerianPropagator::CalculateStumpffParameters(double psi)
{
   StumpffParameters stumpff;

    if (psi > 1.0e-6)
    {
        double sqrtPsi = sqrt(psi);
        stumpff.c2 = (1.0 - cos(sqrtPsi)) / psi;
        stumpff.c3 = (sqrtPsi - sin(sqrtPsi)) / pow(psi, 1.5);
    }
    else if (psi < -1.0e-6)
    {
        double sqrtPsi = sqrt(-psi);
        stumpff.c2 = (1.0 - cosh(sqrtPsi)) / psi;
        stumpff.c3 = (sinh(sqrtPsi) - sqrtPsi) / pow(-psi, 1.5);
    }
    else
    {
       stumpff.c2 = 0.5;
       stumpff.c3 = 1.0 / 6.0;
    }

    return stumpff;
}

////////////////////////////////////////////////////////////
KeplerianPropagator::LagrangeCoefficients KeplerianPropagator::CalculateLagrangeCoefficients(double r0, double seconds, double sqrtMu, const UniversalVariableResult& result)
{
   // Unpack the inputs
   const double& x = result.x;
   const double& r = result.r;
   const double& psi = result.psi;
   const double& c2 = result.stumpff.c2;
   const double& c3 = result.stumpff.c3;

   // Create aliases for Lagrange coefficients
   LagrangeCoefficients coeff;
   double& f = coeff.f;
   double& g = coeff.g;
   double& fDot = coeff.fDot;
   double& gDot = coeff.gDot;

   f = 1.0 - SQR(x) / r0 * c2;
   fDot = sqrtMu / r / r0 * x * (psi * c3 - 1.0);
   g = seconds - pow(x, 3.0) / sqrtMu * c3;
   gDot = 1.0 - SQR(x) / r * c2;

   double sanityCheck = abs((f * gDot - fDot * g) - 1.0);
   if (sanityCheck > MATH_TOLERANCE)
   {
      OTL_WARN() << "KeplerianPropagator::CalculateLagrangeCoefficients(): Sanity check failed for " << Bracket(sanityCheck);
   }

   return coeff;
}

////////////////////////////////////////////////////////////
// Alternative implementations/approaches
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Slightly more performanant than universal variable approach
// Should the PropagateK methods have their own class?
void KeplerianPropagator::PropagateK(const OrbitalElements& initialOrbitalElements, double mu, const Time& timeDelta, OrbitalElements& finalOrbitalElements)
{
   const double a = initialOrbitalElements.semiMajorAxis;
   const double e = initialOrbitalElements.eccentricity;
   double TA0 = initialOrbitalElements.trueAnomaly;

   double TA = 0.0;
   if (e < (1.0 - MATH_TOLERANCE)) // Circular or elliptical
   {
      double n = sqrt(mu / pow(a, 3.0));
      double E0 = ConvertTrueAnomaly2EccentricAnomaly(e, TA0);
      double M0 = E0 - e*sin(E0);
      double M = M0 + n * timeDelta.Seconds();
      double E = m_keplerElliptical.Evaluate(e, M);
      TA = ConvertEccentricAnomaly2TrueAnomaly(e, E);
   }
   else if (e >(1.0 + MATH_TOLERANCE)) // Hyperbolic
   {
      double n = sqrt(mu / pow(-a, 3.0));
      double H0 = ConvertTrueAnomaly2HyperbolicAnomaly(e, TA0);
      double M0 = e * sinh(H0) - H0;
      double M = M0 + n * timeDelta.Seconds();
      double H = m_keplerHyperbolic.Evaluate(e, M);
      TA = ConvertHyperbolicAnomaly2TrueAnomaly(e, H);
   }
   else // Parabolic
   {
      OTL_ERROR() << "Propagate for parabolic orbits is not implemented yet";
      auto stateVector = ConvertOrbitalElements2StateVector(initialOrbitalElements, mu);
      double h = stateVector.position.cross(stateVector.velocity).norm();
      double p = SQR(h) / mu;
      double B0 = ConvertTrueAnomaly2ParabolicAnomaly(TA0);
      double M0 = B0 + pow(B0, 3.0) / 3.0;
      double B = 0.0;// m_keplerParabolic.Evaluate(p, timeDelta);
      TA = ConvertParabolicAnomaly2TrueAnomaly(B);
   }

   finalOrbitalElements = initialOrbitalElements;
   finalOrbitalElements.trueAnomaly = TA;
}

////////////////////////////////////////////////////////////
// Much less performant than universal variable approach, but included for completeness
// Can the deltaTA and r be computed more effienently? i.e. without calling ConvertStateVector2OrbitalElements()
// Results are not matching Propagate()
void KeplerianPropagator::PropagateK(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector)
{
   // Unpack the inputs
   const Vector3d& R = initialStateVector.position;
   const Vector3d& V = initialStateVector.velocity;

   // Compute frequently used variables
   double r0 = R.norm();
   double v0 = V.norm();
   double h = 0.5 * SQR(v0) - mu / r0;
   double h1 = R.cross(V).norm();
   double hSquared = SQR(h);

   // Convert to orbital elements
   auto initialOrbitalElements = ConvertStateVector2OrbitalElements(initialStateVector, mu);

   // Propagate the orbital elements
   OrbitalElements finalOrbitalElements;
   PropagateK(initialOrbitalElements, mu, timeDelta, finalOrbitalElements);

   double e = initialOrbitalElements.eccentricity;
   double TA0 = initialOrbitalElements.trueAnomaly;
   double TA = finalOrbitalElements.trueAnomaly;

   // Compute change in true anomaly and final position magnitude
   double deltaTA = TA - TA0;
   double r = (hSquared / mu) / (1.0 + e * cos(TA));

   double cosDeltaTA = cos(deltaTA);
   double sinDeltaTA = sin(deltaTA);

   // Compute the Lagrange coefficients
   // Ref: Curtis equations 2.148a-d
   LagrangeCoefficients coeff;
   coeff.f = 1.0 - mu * r / hSquared * (1.0 - cosDeltaTA);
   coeff.g = r * r0 / h * sinDeltaTA;
   coeff.fDot = mu / h * (1.0 - cosDeltaTA) / sinDeltaTA * (mu / hSquared * (1.0 - cosDeltaTA) - 1.0 / r0 - 1.0 / r);
   coeff.gDot = 1.0 - mu * r0 / hSquared * (1.0 - cosDeltaTA);

   // Compute the final state vector
   finalStateVector.position = coeff.f    * R + coeff.g    * V;
   finalStateVector.velocity = coeff.fDot * R + coeff.gDot * V;
}

////////////////////////////////////////////////////////////
// Nnearly identical performance-wise as Vallado's implementation
// Vallado iterates for the error instead of the ratio which is a little more intuiative
/// \reference H. Curtis. Orbital Mechanics for Engineering Students. Algorithm 3.3, section 3.7, page 138-139
KeplerianPropagator::UniversalVariableResult
KeplerianPropagator::CalculateUniversalVariableCurtis(double r0, double v0, double rdotv, double seconds, double mu)
{
   // Create aliases for results
   UniversalVariableResult results;
   double& x = results.x;
   double& r = results.r;
   double& psi = results.psi;
   double& c2 = results.stumpff.c2;
   double& c3 = results.stumpff.c3;
   auto& stumpff = results.stumpff;

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double alpha = 2.0 / r0 - SQR(v0) / mu; // Reciprocal of semi-major axis
   double vr0 = rdotv / r0;

   // Compute the initial guess depending on orbit type
   x = CalculateUniversableVariableInitialGuess(r0, v0, rdotv, alpha, seconds, mu);

   // Solve using Newton-Raphson iteration
   int iter = 0;
   const int MAX_ITERATIONS = 100;
   double ratio = MATH_INFINITY;
   while (abs(ratio) > MATH_TOLERANCE && iter++ < MAX_ITERATIONS)
   {
      double xSquared = SQR(x);
      psi = xSquared * alpha;
      stumpff = CalculateStumpffParameters(psi);
      double F = r0 * vr0 / sqrtMu * xSquared * c2 + (1.0 - alpha * r0) * pow(x, 3.0) * c3 + r0 * x - sqrtMu * seconds;
      double dFdx = r0 * vr0 / sqrtMu * x * (1.0 - alpha * xSquared * c3) + (1.0 - alpha * r0) * xSquared * c2 + r0;
      ratio = F / dFdx;
      x -= ratio;
   }

   if (iter >= MAX_ITERATIONS)
   {
      OTL_WARN() << "KeplerianPropagator::CalculateUniversalVariableCurtis(): Max iterations reached with ratio " << Bracket(abs(ratio));
   }

   return results;
}

} // namespace keplerian

} // namespace otl