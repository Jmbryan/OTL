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
#include <OTL/Core/PropagateLagrangian.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
void PropagateLagrangian::Propagate(const OrbitalElements& initialOrbitalElements, double mu, const Time& timeDelta, OrbitalElements& finalOrbitalElements)
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
   else if (e > (1.0 + MATH_TOLERANCE)) // Hyperbolic
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
      StateVector stateVector;
      ConvertOrbitalElements2StateVector(initialOrbitalElements, stateVector, mu);
      double h = stateVector.position.Cross(stateVector.velocity).norm();    
      double p = SQR(h) / mu;
      double B0 = ConvertTrueAnomaly2ParabolicAnomaly(TA0);
      double M0 = B0 + pow(B0, 3.0) / 3.0;
      double B = 0.0;// m_keplerParabolic.Evaluate(p, timeDelta);
      TA = ConvertParabolicAnomaly2TrueAnomaly(B);
   }

   //double f = 1.0 - mu * r0 / SQR(h) * (1.0 - cos(TA - TA0));

   finalOrbitalElements = initialOrbitalElements;
   finalOrbitalElements.trueAnomaly = TA;
}

////////////////////////////////////////////////////////////
void PropagateLagrangian::PropagateX(const OrbitalElements& initialOrbitalElements, double mu, const Time& timeDelta, OrbitalElements& finalOrbitalElements)
{
   const double a = initialOrbitalElements.semiMajorAxis;
   const double e = initialOrbitalElements.eccentricity;
   double TA0 = initialOrbitalElements.trueAnomaly;

   double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA0));
   double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
   double vr0 = (sqrt(mu) * e * sin(TA0)) / sqrt(a * (1.0 - SQR(e)));
   double rdotv = r0 * vr0;

   double alpha = 1.0 / a;
   double x = CalculateUniversalVariable2(r0, v0, rdotv, timeDelta.Seconds(), mu);

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

   finalOrbitalElements = initialOrbitalElements;
   finalOrbitalElements.trueAnomaly = TA;
}

////////////////////////////////////////////////////////////
void PropagateLagrangian::Propagate(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector)
{
   double seconds = timeDelta.Seconds();

   // Unpack the initial state vector and compute frequently used variables
   const Vector3d& R = initialStateVector.position;
   const Vector3d& V = initialStateVector.velocity;
   double r0 = R.GetNorm();
   double v0 = V.GetNorm();
   double rDotv = R.Dot(V);
   double sqrtMu = sqrt(mu);

   // Reciprocal of semi-major axis
   double alpha = 2.0 / r0 - SQR(v0) / mu;

   // Compute universal variable initial guess depending on orbit type
   double x = 1.0;
   double alphaThreshold = 0.000001 * (ASTRO_MU_EARTH / mu); // Reference?
   if (alpha > alphaThreshold) // Circle or Ellipse
   {
      x = sqrtMu * seconds * alpha;
   }
   else if (alpha < -alphaThreshold) // Hyperbola
   {
      double a = 1.0 / alpha;
      x = Sign(seconds) * sqrt(-a) * log((-2.0 * mu * alpha * seconds) /
                                         (rDotv + Sign(seconds) * sqrt(-mu * a) * (1.0 - r0 * alpha)));
   }
   else // Parabola
   {
      double h = 0.5 * SQR(v0) - mu / r0;
      double p = SQR(h) / mu;
      double s = 0.5 * acot(3.0 * sqrt(mu / pow(p, 3.0)) * seconds);
      double w = pow(tan(s), 1.0 / 3.0);
      x = 2.0 * sqrt(p) * cot(2.0 * w);
   }
    
   // Solve for the universal variable using Newton-Raphson iteration
   double xPrev = MATH_INFINITY;
   int iter = 0;
   const int MAX_ITER = 1000;
   double r;
   double psi, c2, c3;
   while (fabs(x - xPrev) >= MATH_TOLERANCE && iter++ < MAX_ITER)
   {
      xPrev = x;
      psi = SQR(x) * alpha;
      CalculateC2C3(psi, c2, c3);
      r = SQR(x) * c2 + (rDotv / sqrtMu) * x * (1.0 - psi * c3) + r0 * (1.0 - psi * c2);
      x += (sqrtMu * seconds - pow(x, 3.0) * c3 - rDotv / sqrtMu * SQR(x) * c2 - r0 * x * (1.0 - psi * c3)) / r;
   }

   if (iter == MAX_ITER)
   {
      OTL_WARN() << "PropagateLagrangian::Propagate(): Max iterations reached for universal variable with error " << Bracket(abs(x - xPrev));
   }

   // Compute the resulting Lagrange coefficients
   double f    = 1.0 - SQR(x) / r0 * c2;
   double fDot = sqrtMu / r / r0 * x * (psi * c3 - 1.0);
   double g = seconds - pow(x, 3.0) / sqrtMu * c3;
   double gDot = 1.0 - SQR(x) / r * c2;

   double sanityCheck = (f * gDot - fDot * g) - 1.0;
   if (abs(sanityCheck) > MATH_TOLERANCE)
   {
      OTL_WARN() << "PropagateLagrangian::Propagate(): Lagrange coefficient sanity check " << Bracket(abs(sanityCheck)) << " failed!";
   }

   // Return the state vector
   finalStateVector.position = f * R + g * V;
   finalStateVector.velocity = fDot * R + gDot * V;
}

void PropagateLagrangian::PropagateX(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector)
{
   // Unpack the inputs
   const Vector3d& R = initialStateVector.position;
   const Vector3d& V = initialStateVector.velocity;
   double seconds = timeDelta.Seconds();

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double r0 = R.GetNorm();
   double v0 = V.GetNorm();
   double rdotv = R.Dot(V);

   // Compute the universal variable
   CalculateUniversalVariable1(r0, v0, rdotv, seconds, mu);

   // Compute the Lagrange coefficients
   CalculateLagrangeCoefficientsX(r0, seconds, sqrtMu);

   // Compute the final state vector
   finalStateVector.position = m_f    * R + m_g    * V;
   finalStateVector.velocity = m_fDot * R + m_gDot * V;
}

void PropagateLagrangian::CalculateLagrangeCoefficientsX(double r0, double seconds, double sqrtMu)
{
   m_f    = 1.0 - SQR(m_x) / r0 * m_c2;
   m_fDot = sqrtMu / m_r / r0 * m_x * (m_psi * m_c3 - 1.0);
   m_g    = seconds - pow(m_x, 3.0) / sqrtMu * m_c3;
   m_gDot = 1.0 - SQR(m_x) / m_r * m_c2;

   double sanityCheck = abs((m_f * m_gDot - m_fDot * m_g) - 1.0);
   if (sanityCheck > MATH_TOLERANCE)
   {
      OTL_WARN() << "PropagateLagrangian::CalculateLagrangeCoefficients(): Sanity check failed for " << Bracket(sanityCheck);
   }
}

////////////////////////////////////////////////////////////
// Vallado
void PropagateLagrangian::CalculateUniversalVariable1(double r0, double v0, double rdotv, double seconds, double mu)
{
   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   double alpha = 2.0 / r0 - SQR(v0) / mu; // Reciprocal of semi-major axis

   // Compute the initial guess depending on orbit type
   m_x = CalculateUniversableVariableInitialGuess(r0, v0, rdotv, alpha, seconds, mu);

   // Solve using Newton-Raphson iteration
   int iter = 0;
   const int MAX_ITERATIONS = 100;
   double xPrev, error = MATH_INFINITY;
   while (error >= MATH_TOLERANCE && iter++ < MAX_ITERATIONS)
   {
      xPrev = m_x;
      double xSquared = SQR(m_x);
      m_psi = xSquared * alpha;
      CalculateC2C3(m_psi, m_c2, m_c3);
      m_r = xSquared * m_c2 + (rdotv / sqrtMu) * m_x * (1.0 - m_psi * m_c3) + r0 * (1.0 - m_psi * m_c2);
      m_x += (sqrtMu * seconds - pow(m_x, 3.0) * m_c3 - rdotv / sqrtMu * xSquared * m_c2 - r0 * m_x * (1.0 - m_psi * m_c3)) / m_r;
      error = fabs(m_x - xPrev);
   }

   if (iter == MAX_ITERATIONS)
   {
      OTL_WARN() << "PropagateLagrangian::CalculateUniversalVariable(): Max iterations reached with error " << Bracket(error);
   }
}

////////////////////////////////////////////////////////////
// Curtis
void PropagateLagrangian::CalculateUniversalVariable2(double r0, double v0, double rdotv, double seconds, double mu)
{
   // Unpack the inputs
   //double a = orbitalElements.semiMajorAxis;
   //double e = orbitalElements.eccentricity;
   //double TA = orbitalElements.trueAnomaly;
   //double seconds = timeDelta.Seconds();

   // Compute frequently used variables
   double sqrtMu = sqrt(mu);
   //double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA));
   //double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
   //double vr0 = (sqrtMu * e * sin(TA)) / sqrt(a * (1.0 - SQR(e)));
   //double rdotv = r0 * vr0;
   //double alpha = 1.0 / a;

   double alpha = 2.0 / r0 - SQR(v0) / mu; // Reciprocal of semi-major axis
   double vr0 = rdotv / r0;

   // Compute the initial guess depending on orbit type
   double x = CalculateUniversableVariableInitialGuess(r0, v0, rdotv, alpha, seconds, mu);

   // Solve using Newton-Raphson iteration
   int iter = 0;
   const int MAX_ITERATIONS = 100;
   double psi, c2, c3, ratio = MATH_INFINITY;
   while (abs(ratio) > MATH_TOLERANCE && iter++ < MAX_ITERATIONS)
   {
      double xSquared = SQR(x);
      psi = xSquared * alpha;
      CalculateC2C3(psi, c2, c3); 
      double F = r0 * vr0 / sqrtMu * xSquared * c2 + (1.0 - alpha * r0) * pow(x, 3.0) * c3 + r0 * x - sqrtMu * seconds;
      double dFdx = r0 * vr0 / sqrtMu * x * (1.0 - alpha * xSquared * c3) + (1.0 - alpha * r0) * xSquared * c2 + r0;
      ratio = F / dFdx;
      x -= ratio;
   }

   if (iter >= MAX_ITERATIONS)
   {
      OTL_WARN() << "PropagateLagrangian::CalculateUniversalVariable(): Max iterations reached with ratio " << Bracket(abs(ratio));
   }
}

////////////////////////////////////////////////////////////
double PropagateLagrangian::CalculateUniversableVariableInitialGuess(double r0, double v0, double rdotv, double alpha, double seconds, double mu)
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
void PropagateLagrangian::CalculateC2C3(double psi, double& c2, double& c3)
{
    if (psi > 1.0e-6)
    {
        double sqrtPsi = sqrt(psi);
        c2 = (1.0 - cos(sqrtPsi)) / psi;
        c3 = (sqrtPsi - sin(sqrtPsi)) / pow(psi, 1.5);
    }
    else if (psi < -1.0e-6)
    {
        double sqrtPsi = sqrt(-psi);
        c2 = (1.0 - cosh(sqrtPsi)) / psi;
        c3 = (sinh(sqrtPsi) - sqrtPsi) / pow(-psi, 1.5);
    }
    else
    {
        c2 = 0.5;
        c3 = 1.0 / 6.0;
    }
}

} // namespace keplerian

} // namespace otl