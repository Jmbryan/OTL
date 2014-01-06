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
#include <OTL/Core/Orbit.hpp>
#include <OTL/Core/PropagateAnalytical.hpp>
#include <OTL/Core/KeplersEquations.hpp>

namespace otl
{

void PropagateAnalytical::Propagate(OrbitalElements& orbitalElements, double mu, double seconds)
{
   const double a = orbitalElements.semiMajorAxis;
   const double e = orbitalElements.eccentricity;
   double TA = orbitalElements.trueAnomaly;

   //// Elliptical orbits
   //if (e > 0.0 && e < 1.0)
   //{
   //   double T = 2.0 * MATH_2_PI * pow(a, 1.5) / sqrt(mu);

   //   double E1 = 2.0 * atan(sqrt((1.0 - e) / (1.0 + e)) * tan(TA / 2.0));
   //   double M1 = E1 - e * sin(E1);
   //   double t1 = M1 * T / MATH_2_PI;

   //   double t2 = seconds + t1;
   //   double M2 = MATH_2_PI * t2 / T;

   //   double E2 = SolveKeplersEquationElliptical(e, M2);

   //   TA = 2.0 * atan(sqrt((1.0 + e) / (1.0 - e)) * tan(E2 / 2.0));
   //}
   //// Hyperpolic orbits
   //else if (e > 1.0)
   //{
   //   double h = sqrt(mu * a * (1.0 - SQR(e)));

   //   double F1 = 2.0 * atanh(sqrt((e - 1.0) / (e + 1.0)) * tan(TA / 2.0));
   //   double M1 = e * sinh(F1) - F1;
   //   double t1 = M1 * pow(h, 3.0) / SQR(mu) / pow(SQR(e) - 1.0, 1.5);

   //   double t2 = seconds + t1;
   //   double M2 = t2 / pow(h, 3.0) * SQR(mu) * pow(SQR(e) - 1.0, 1.5);
   //   double F2 = SolveKeplersEquationHyperbolic(e, M2);

   //   TA = 2.0 * atan(sqrt((e + 1.0) / (e - 1.0)) * tanh(F2 / 2.0));
   //}

   //orbitalElements.trueAnomaly = TA;
  
   double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA));
   double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
   double vr0 = (sqrt(mu) * e * sin(TA)) / sqrt(a * (1.0 - SQR(e)));

   double alpha = 1.0 / a;
   double x = CalculateUniversalVariable(r0, vr0, alpha, seconds, mu);

   // Ellpitcal orbits
   if (e > 0.0 && e < 1.0)
   {
       double E0 = 2.0 * atan(sqrt((1.0 - e) / (1.0 + e)) * tan(0.5 * TA));
       double E = E0 + x / sqrt(a);
       TA = 2.0 * atan(sqrt((1.0 + e) / (1.0 - e)) * tan(0.5 * E));
   }
   // Hyperbolic orbits
   else if (e > 1.0)
   {
       double F0 = 2.0 * atanh(sqrt((e - 1.0) / (e + 1.0)) * tan(0.5 * TA));
       double F = F0 + x / sqrt(-a);
       TA = 2.0 * atan(sqrt((e + 1.0) / (e - 1.0)) * tanh(0.5 * F)); 
   }
   // Circular orbits
   else if (e == 0.0)
   {
       
   }
   // Parabolic orbits
   else if (e == 1.0)
   {

   }

   orbitalElements.trueAnomaly = TA;

}

void PropagateAnalytical::Propagate(StateVector& stateVector, double mu, double seconds)
{
    //OrbitalElements orbitalElements;
    //ConvertStateVector2OrbitalElements(stateVector, orbitalElements, mu);
    //Propagate(orbitalElements, mu, seconds);
    //ConvertOrbitalElements2StateVector(orbitalElements, stateVector, mu);
    //return;

    Vector3d R = stateVector.position;
    Vector3d V = stateVector.velocity;
    double r0 = R.Magnitude();
    double v0 = V.Magnitude();

    double rDotv = Vector3d::Dot(R, V);

    // Specific Angular Momentum
    double h = 0.5 * SQR(v0) - mu / r0;

    double alpha = -SQR(v0) / mu + 2.0 / r0;

    double x;

    // Circle or Ellipse
    if (alpha > 0.000001)
    {
        x = sqrt(mu) * seconds * alpha;
    }
    // Hyperbola
    else if (alpha < -0.00001)
    {
        double a = 1.0 / alpha;

        x = Sign(seconds) * sqrt(-a) * log((-2.0 * mu * alpha * seconds) /
                                           (rDotv + Sign(seconds) * sqrt(-mu * a) * (1.0 - r0 * alpha)));
    }
    // Parabola
    else if (fabs(alpha) <= 0.00001)
    {
        Vector3d H;
        Vector3d::Cross(R, V, H);
        double h = H.Magnitude();

        double p = SQR(h) / mu;
    }
    
    double xPrev = MATH_INFINITY;
    int iter = 0;
    const int MAX_ITER = 100;
    double r;
    double psi, c2, c3;
    while (fabs(x - xPrev) >= 1.0e-6 && iter++ < MAX_ITER)
    {
        psi = SQR(x) * alpha;
        CalculateC2C3(psi, c2, c3);

        r = SQR(x) * c2 + (rDotv / sqrt(mu)) * x * (1.0 - psi * c3) + r0 * (1.0 - psi * c2);

        xPrev = x;
        x += (sqrt(mu) * seconds - pow(x, 3.0) * c3 - rDotv / sqrt(mu) * SQR(x) * c2 - r0 * x * (1.0 - psi * c3)) / r;
    }

    double f    = 1.0 - SQR(x) / r0 * c2;
    double fDot = sqrt(mu) / r / r0 * x * (psi * c3 - 1.0);
    double g    = seconds - pow(x, 3.0) / sqrt(mu) * c3;
    double gDot = 1.0 - SQR(x) / r * c2;

    stateVector.position = f * R + g * V;
    stateVector.velocity = fDot * R + gDot * V;

}

double PropagateAnalytical::CalculateUniversalVariable(double r0, double vr0, double alpha, double dt, double mu)
{
    const double TOL = 1.e-8;
    const int MAX_ITERATIONS = 100;

    double x = sqrt(mu) * abs(alpha) * dt;

    int iteration = 0;
    double ratio = 1.0;
    double psi, c2, c3;
    while (abs(ratio) > TOL && iteration++ < MAX_ITERATIONS)
    {
        psi = SQR(x) * alpha;
        CalculateC2C3(psi, c2, c3); 

        double F = r0 * vr0 / sqrt(mu) * SQR(x) * c2 + (1.0 - alpha * r0) * pow(x, 3.0) * c3 + r0 * x - sqrt(mu) * dt;

        double dFdx = r0 * vr0 / sqrt(mu) * x * (1.0 - alpha * SQR(x) * c3) + (1.0 - alpha * r0) * SQR(x) * c2 + r0;

        ratio = F / dFdx;
        x -= ratio;
    }
    if (iteration >= MAX_ITERATIONS)
   {
      std::cout << "PropagateAnalytical::CalculateUniversalVariable(): Max iterations exceeded!" << std::endl;
   }

    return x;
}

void PropagateAnalytical::CalculateC2C3(double psi, double& c2, double& c3)
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


} // namespace otl