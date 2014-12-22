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

#include <OTL/Core/LambertExponentialSinusoid.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
void LambertExponentialSinusoid::Evaluate(const Vector3d& initialPosition,
                                          const Vector3d& finalPosition,
                                          const Time& timeDelta,
                                          const Orbit::Direction& orbitDirection,
                                          int maxRevolutions,
                                          double mu,
                                          Vector3d& initialVelocity,
                                          Vector3d& finalVelocity)
{
    double seconds = timeDelta.Seconds();
    assert(seconds >= 0.0);
    
    // Non-dimensional units
    double DU, VU, TU;
    DU = initialPosition.Magnitude();
    VU = sqrt(mu / DU);
    TU = DU / VU;

     // Non-dimensionalize the position vectors
    Vector3d R1 = initialPosition;
    Vector3d R2 = finalPosition;
    R1 *= 1.0 / DU;
    R2 *= 1.0 / DU;

    double r2 = R2.Magnitude();

    // Non-dimensionalize the time of flight
    double tof = seconds / TU;

    // Cross product and dot product of initial and final position.
    Vector3d CrossR1R2;
    Vector3d::Cross(R1, R2, CrossR1R2);
    double crossR1R2 = CrossR1R2.Magnitude();
    double dotR1R2 = Vector3d::Dot(R1, R2);

    double trueAnomaly = acos(dotR1R2 / r2);

    // Direction of travel
    if (orbitDirection == Orbit::Direction::Prograde && CrossR1R2.z <= 0.0)
    {
        trueAnomaly = MATH_2_PI - trueAnomaly;
    }
    else if (orbitDirection == Orbit::Direction::Retrograde && CrossR1R2.z >= 0.0)
    {
        trueAnomaly = MATH_2_PI - trueAnomaly;
    }

    int longway = 1;
    if (trueAnomaly > MATH_PI)
    {
        longway = -1;
    }

    double c      = sqrt(1.0 + r2*r2 - 2.0*r2*cos(trueAnomaly));
    double s      = 0.5*(1.0 + r2 + c);
    double aMin   = 0.5*s;
    double lambda = sqrt(r2)*cos(0.5*trueAnomaly)/s;

    double input1 = -0.5233;
    double input2 =  0.5233;
    double x1     = log(1.0 + input1);
    double x2     = log(1.0 + input2);

    double logt   = log(tof);

    double y1 = CalculateTimeOfFlight(input1, s, c, longway, maxRevolutions);
    double y2 = CalculateTimeOfFlight(input2, s, c, longway, maxRevolutions);
    y1 = log(y1) - logt;
    y2 = log(y2) - logt;

    // Newton-Raphson iteration
    double error = 1.0;
    int iteration = 0;
    double xnew, ynew, x;
    while (error > MATH_TOLERANCE && iteration < 60)
    {
        xnew = (x1*y2 - y1*x2) / (y2 - y1);
        if (maxRevolutions == 0)
        {
            x = exp(xnew) - 1.0;
        }
        else
        {
            x = 2.0 * MATH_1_OVER_PI * atan(xnew);
        }

        ynew = CalculateTimeOfFlight(x, s, c, longway, maxRevolutions);

        if (maxRevolutions == 0)
        {
            ynew = log(ynew) - logt;
        }
        else
        {
            ynew = ynew - tof;
        }

        x1 = x2;    x2 = xnew;
        y1 = y2;    y2 = ynew;

        error = abs(x1 - xnew);
        iteration++;
    }

    double a = aMin / (1.0 - x*x);

    double alpha, beta, sinpsi, sinhpsi, eta, eta2;
    if (x < 1.0) // ellipse
    {
        alpha   = 2.0 * acos(x);
        beta    = 2.0 * longway * asin(sqrt(0.5 * (s - c) / a));
        sinpsi  = sin(0.5 * (alpha - beta));
        eta2    = 2.0 * a * sinpsi * sinpsi / s;
        eta     = sqrt(eta2);
    }
    else // hyperbolic
    {
        alpha   = 2.0 * acosh(x);
        beta    = 2.0 * longway * asinh(sqrt(0.5 * (c - s) / a));
        sinhpsi = sinh(0.5 * (alpha - beta));
        eta2    = -2.0 * a * sinhpsi * sinhpsi / s;
        eta     = sqrt(eta2);
    }

    Vector3d Ih = (longway / crossR1R2) * CrossR1R2; 
    Vector3d R2u = R2;
    R2u.Normalize();

    Vector3d CrossIhR1, CrossIhR2u;
    Vector3d::Cross(Ih, R1, CrossIhR1);
    Vector3d::Cross(Ih, R2u, CrossIhR2u);

    double sinHalfTheta = sin(0.5 * trueAnomaly);

    // Radial and tangential departure velocity
    double vr1 = (1.0 / eta / sqrt(aMin)) * ((2.0 * lambda * aMin) - lambda - (x * eta));
    double vt1 = sqrt((r2 / aMin / eta2) * (sinHalfTheta * sinHalfTheta));

    // Radial and tangential arrival velocity
    double vt2 = vt1 / r2;
    double vr2 = (vt1 - vt2) / tan(0.5 * trueAnomaly) - vr1;

    // Velocity vectors
    initialVelocity = (vr1 * R1)  + (vt1 * CrossIhR1);
    finalVelocity =   (vr2 * R2u) + (vt2 * CrossIhR2u);

    // Convert back to dimensional units
    initialVelocity *= VU;
    finalVelocity *= VU;
}

////////////////////////////////////////////////////////////
double LambertExponentialSinusoid::CalculateTimeOfFlight(double x, double s, double c, int longway, int maxRevolutions)
{
    double timeOfFlight = 0.0;

    double a = 0.5*s / (1.0 - x*x);
    
    double alpha, beta;   
    if (x < 1.0) // ellipse
    {
        alpha = 2.0*acos(x);
        beta  = 2.0*longway*asin(sqrt(0.5*(s - c) / a));
        timeOfFlight  = a*sqrt(a)*((alpha - sin(alpha)) - (beta - sin(beta)) + MATH_2_PI * maxRevolutions); 
    }
    else // hyperbola
    {
        alpha = 2.0*acosh(x);
        beta  = 2.0*longway*asinh(sqrt(-0.5*(s - c) / a));
        timeOfFlight  = -a*sqrt(-a)*((sinh(alpha) - alpha) - (sinh(beta) - beta));
    }

    return timeOfFlight;
}

} // namespace keplerian

} // namespace otl