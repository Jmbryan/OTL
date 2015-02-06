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

#include <OTL/Core/LagrangianPropagator.h>

namespace otl
{

////////////////////////////////////////////////////////////
LagrangianPropagator::LagrangianPropagator() :
   IPropagator()
{

}

////////////////////////////////////////////////////////////
LagrangianPropagator::~LagrangianPropagator()
{

}

////////////////////////////////////////////////////////////
void LagrangianPropagator::VPropagate(const test::StateVector& initialStateVector, const Time& timeDelta, double mu, test::StateVector& finalStateVector)
{
   // Convert the state vector to cartesian state vector
   const StateVector& cartesianStateVector = initialStateVector.ToCartesianStateVector(mu);

   // Unpack the initial cartesian vectors and time duration in seconds
   const Vector3d& R1 = cartesianStateVector.position;
   const Vector3d& V1 = cartesianStateVector.velocity;
   double seconds = timeDelta.Seconds();

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double r0 = R1.norm();
   double v0 = V1.norm();
   double rdotv = R1.dot(V1);

   // Compute the universal variable results
   auto results = CalculateUniversalVariable(r0, v0, rdotv, seconds, mu);

   // Compute the Lagrange coefficients
   auto coeff = CalculateLagrangeCoefficients(r0, seconds, sqrtMu, results);

   // Compute the final cartesian vectors
   auto R2 = coeff.f    * R1 + coeff.g    * V1;
   auto V2 = coeff.fDot * R1 + coeff.gDot * V1;

   finalStateVector = StateVector(R2, V2);
}

////////////////////////////////////////////////////////////
// Vallado, preferred method but nearly identical performance-wise as Curtis
LagrangianPropagator::UniversalVariableResult
LagrangianPropagator::CalculateUniversalVariable(double r0, double v0, double rdotv, double seconds, double mu)
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
   OTL_WARN_IF(iter == MAX_ITERATIONS, "Max iterations << " << Bracket(MAX_ITERATIONS) << " reached before error " << Bracket(error) << " within tolerance " << Bracket(MATH_TOLERANCE));

   return results;
}

////////////////////////////////////////////////////////////
double LagrangianPropagator::CalculateUniversableVariableInitialGuess(double r0, double v0, double rdotv, double alpha, double seconds, double mu)
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
LagrangianPropagator::StumpffParameters LagrangianPropagator::CalculateStumpffParameters(double psi)
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
LagrangianPropagator::LagrangeCoefficients LagrangianPropagator::CalculateLagrangeCoefficients(double r0, double seconds, double sqrtMu, const UniversalVariableResult& result)
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
   OTL_WARN_IF(sanityCheck > MATH_TOLERANCE, "Sanity check failed for " << Bracket(sanityCheck) << " == " << Bracket(MATH_TOLERANCE));

   return coeff;
}

} // namespace otl