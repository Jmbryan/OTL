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
#include <OTL/Core/KeplersEquations.h>
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
   double TA = initialOrbitalElements.trueAnomaly;

   double r0 = a * (1.0 - SQR(e)) / (1.0 + e * cos(TA));
   double v0 = sqrt(mu * (2.0 / r0 - 1.0 / a));
   double vr0 = (sqrt(mu) * e * sin(TA)) / sqrt(a * (1.0 - SQR(e)));

   double alpha = 1.0 / a;
   double x = CalculateUniversalVariable(r0, vr0, alpha, timeDelta, mu);

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

   finalOrbitalElements = initialOrbitalElements;
   finalOrbitalElements.trueAnomaly = TA;

}

////////////////////////////////////////////////////////////
void PropagateLagrangian::Propagate(const StateVector& initialStateVector, double mu, const Time& timeDelta, StateVector& finalStateVector)
{
   double seconds = timeDelta.Seconds();

    Vector3d R = initialStateVector.position;
    Vector3d V = initialStateVector.velocity;
    double r0 = R.GetNorm();
    double v0 = V.GetNorm();

    double rDotv = R.Dot(V);

    // Specific Angular Momentum
    double h = 0.5 * SQR(v0) - mu / r0;

    double alpha = -SQR(v0) / mu + 2.0 / r0;

    double x;

    // Circle or Ellipse
    double alphaThreshold = 0.000001 * (ASTRO_MU_EARTH / mu);
    if (alpha >alphaThreshold)
    {
        x = sqrt(mu) * seconds * alpha;
    }
    // Hyperbola
    else if (alpha < -alphaThreshold)
    {
        double a = 1.0 / alpha;

        x = Sign(seconds) * sqrt(-a) * log((-2.0 * mu * alpha * seconds) /
                                           (rDotv + Sign(seconds) * sqrt(-mu * a) * (1.0 - r0 * alpha)));
    }
    // Parabola
    else
    {
       Vector3d H = R.Cross(V);
       double h = H.GetNorm();
       double p = SQR(h) / mu;
       //double p = SQR(R.Cross(V).GetNorm()) / mu;
    }
    
    double xPrev = MATH_INFINITY;
    int iter = 0;
    const int MAX_ITER = 100;
    double r;
    double psi, c2, c3;
    while (fabs(x - xPrev) >= MATH_TOLERANCE && iter++ < MAX_ITER)
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

    double sanityCheck = (f * gDot - fDot * g) - 1.0;
    if (abs(sanityCheck) > MATH_TOLERANCE)
    {
        OTL_WARN() << "PropagateLagrangian::Propagate(): Lagrange coefficient sanity check [" << abs(sanityCheck) << "] failed!";
    }

    finalStateVector.position = f * R + g * V;
    finalStateVector.velocity = fDot * R + gDot * V;

}

////////////////////////////////////////////////////////////
double PropagateLagrangian::CalculateUniversalVariable(double r0, double vr0, double alpha, const Time& timeDelta, double mu)
{
    const double TOL = 1.e-8;
    const int MAX_ITERATIONS = 100;

    double dt = timeDelta.Seconds();
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
       OTL_WARN() << "PropagateLagrangian::CalculateUniversalVariable(): Max iterations [" << MAX_ITERATIONS << "] exceeded!";
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