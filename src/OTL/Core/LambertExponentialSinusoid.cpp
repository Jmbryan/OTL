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
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
void LambertExponentialSinusoid::Evaluate2(const Vector3d& initialPosition,
                                           const Vector3d& finalPosition,
                                           const Time& timeDelta,
                                           const Orbit::Direction& orbitDirection,
                                           int maxRevolutions,
                                           double mu,
                                           Vector3d& initialVelocity,
                                           Vector3d& finalVelocity)
{
   Vector3d R1 = initialPosition;
   Vector3d R2 = finalPosition;

   // 1 - Get lambda and T
   double c = (R2 - R1).norm();
   double r1 = R1.norm();
   double r2 = R2.norm();
   double s = 0.5 * (c + r1 + r2);

   Vector3d ir1 = R1.normalized();
   Vector3d ir2 = R2.normalized();
   Vector3d ih = ir1.cross(ir2).normalized();
   OTL_ERROR_IF(ih.z() == 0.0, "The angular momentum vector has no z component, impossible to automatically define direction");

   double lambdaSquared = 1.0 - c / s;
   double lambda = sqrt(lambdaSquared);

   Vector3d it1, it2;
   if (ih.z() < 0.0) // Transfer angle is larger than 180 degrees as seen from above the z axis
   {
      lambda *= -1.0;
      it1 = ir1.cross(ih).normalized();
      it2 = ir2.cross(ih).normalized();
   }
   else
   {
      it1 = ih.cross(ir1).normalized();
      it2 = ih.cross(ir2).normalized();
   }

   if (orbitDirection == Orbit::Direction::Retrograde)
   {
      lambda *= -1.0;
      it1 *= -1.0;
      it2 *= -1.0;
   }

   double lambdaCubed = lambda * lambdaSquared;
   double T = sqrt(2.0 * mu / pow(s, 3.0)) * timeDelta.Seconds();

   // 2 - Find all x
   int Nmax = static_cast<int>(T / MATH_PI);
   double T00 = acos(lambda) + lambda * sqrt(1.0 - lambdaSquared);
   double T0 = (T00 + Nmax * MATH_PI);
   double T1 = 2.0 / 3.0 * (1.0 - lambdaCubed);
   double DT = 0.0;
   double DDT = 0.0;
   double DDDT = 0.0;
   if (Nmax > 0.0)
   {
      if (T < T0) // Use Halley iterations to find xM and TM
      {
         int iter = 0;
         double error = 1.0;
         double TMin = T0;
         double xOld = 0.0;
         double xNew = 0.0;
         while (true)
         {
            //dTdx(DT, DDT, DDDT, xOld, TMin);
            if (DT != 0.0)
            {
               xNew = xOld - DT * DDT / (SQR(DDT) - DT * DDDT / 2.0);
            }
            error = fabs(xNew - xOld);
            if (error < 1e-13 || iter > 12)
            {
               break;
            }
            //x2tof(TMin, xNew, Nmax);
            xOld = xNew;
            iter++;
         }
         if (TMin > T)
         {
            Nmax *= -1;
         }
      }
   }

   // We exit this if clause with Mmax being the maximum number of revolutions
   // for which there exists a solution. We crop it to maxRevolutions
   Nmax = std::min(maxRevolutions, Nmax);

   std::vector<Vector3d> m_v1(Nmax * 2 + 1);
   std::vector<Vector3d> m_v2(Nmax * 2 + 1);
   std::vector<int> m_iters(Nmax * 2 + 1);
   std::vector<double> m_x(Nmax * 2 + 1);

   // 3 - Find all solutions in x,y
   if (T >= T00)
   {
      m_x[0] = -(T - T00) / (T - T00 + 4);
   }
   else if (T <= T1)
   {
      m_x[0] = T1 * (T1 - T) / (2.0 / 5.0 * (1.0 - lambdaSquared * lambdaCubed) * T) + 1;
   }
   else
   {
      m_x[0] = pow(T / T00, 0.69314718055994529 / log(T1 / T00)) - 1.0;
   }

   //m_iters[0] = householder(T, m_x[0], 0.0, 1e-5, 15);

   // Find the x value for each multi rev solution
   double temp;
   for (int i = 1; i < Nmax + 1; ++i)
   {
      // left
      temp = pow((i * MATH_PI + MATH_PI) / (8.0 * T), 2.0 / 3.0);
      m_x[2 * i - 1] = (temp - 1.0) / (temp + 1.0);
      //m_iters[2 * i - 1] = householder(T, m_x[2 * i - 1], i, 1e-8, 15);

      // right
      temp = pow((8.0 * T) / (i * MATH_PI), 2.0 / 3.0);
      m_x[2 * i] = (temp - 1.0) / (temp + 1.0);
      //m_iters[2 * i] = householder(T, m_x[2 * i], i, 1e-8, 15);
   }

   // For each found x value, reconstruct the terminal velocities
   double gamma = sqrt(mu * s / 2.0);
   double rho = (r1 - r2) / c;
   double sigma = sqrt(1.0 - SQR(rho));
   double vr1, vt1, vr2, vt2, y;
   for (std::size_t i = 0; i < m_x.size(); ++i)
   {
      y = sqrt(1.0 - lambdaSquared + lambdaSquared * SQR(m_x[i]));
      vr1 = gamma * ((lambda * y - m_x[i]) - rho * (lambda * y + m_x[i])) / r1;
      vr2 = -gamma * ((lambda * y - m_x[i]) + rho * (lambda * y + m_x[i])) / r2;
      double vt = gamma * sigma * (y + lambda * m_x[i]);
      vt1 = vt / r1;
      vt2 = vt / r2;
      for (int j = 0; j < 3; ++j)
      {
         m_v1[i][j] = vr1 * ir1[j] + vt1 * it1[j];
         m_v2[i][j] = vr2 * ir2[j] + vt2 * it2[j];
      }
   }
}

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
    OTL_ASSERT(seconds >= 0.0);
    
    // Non-dimensional units
    double DU, VU, TU;
    DU = initialPosition.norm();
    VU = sqrt(mu / DU);
    TU = DU / VU;

     // Non-dimensionalize the position vectors
    Vector3d R1 = initialPosition;
    Vector3d R2 = finalPosition;
    R1 *= 1.0 / DU;
    R2 *= 1.0 / DU;

    double r2 = R2.norm();

    // Non-dimensionalize the time of flight
    double tof = seconds / TU;

    // Cross product and dot product of initial and final position.
    Vector3d CrossR1R2 = R1.cross(R2);
    double crossR1R2 = CrossR1R2.norm();
    double dotR1R2 = R1.dot(R2);

    double trueAnomaly = acos(dotR1R2 / r2);

    // Direction of travel
    if (orbitDirection == Orbit::Direction::Prograde && CrossR1R2.z() <= 0.0)
    {
        trueAnomaly = MATH_2_PI - trueAnomaly;
    }
    else if (orbitDirection == Orbit::Direction::Retrograde && CrossR1R2.z() >= 0.0)
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
    Vector3d R2u = R2.normalized();

    Vector3d CrossIhR1 = Ih.cross(R1);
    Vector3d CrossIhR2u = Ih.cross(R2u);

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