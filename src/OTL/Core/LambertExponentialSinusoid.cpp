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
void LambertExponentialSinusoid::Evaluate(const Vector3d& initialPosition,
                 const Vector3d& finalPosition,
                 const Time& timeDelta,
                 const Orbit::Direction& orbitDirection,
                 int maxRevolutions,
                 double mu,
                 std::vector<Vector3d>& initialVelocities,
                 std::vector<Vector3d>& finalVelocities)
{
   const Vector3d& R1 = initialPosition;
   const Vector3d& R2 = finalPosition;

   // 1 - Get lambda and T
   double c = (R2 - R1).norm();
   double r1 = R1.norm();
   double r2 = R2.norm();
   double s = 0.5 * (r1 + r2 + c);

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

   // Use Halley iterations to find xM and TM
   if (Nmax > 0 && T < T0)
   {
      int iter = 0;
      double error = 1.0;
      double TMin = T0;
      double xOld = 0.0;
      double xNew = 0.0;
      while (true)
      {
         ComputeDerivatives(xOld, TMin, lambda, DT, DDT, DDDT);
         if (DT != 0.0)
         {
            xNew = xOld - DT * DDT / (SQR(DDT) - DT * DDDT / 2.0);
         }
         error = fabs(xNew - xOld);
         if (error < 1e-13 || iter > 12)
         {
            break;
         }
         TMin = ComputeTimeOfFlight(xNew, lambda, Nmax);
         xOld = xNew;
         iter++;
      }
      if (TMin > T)
      {
         Nmax -= 1;
      }
   }

   // We exit this if clause with Mmax being the maximum number of revolutions
   // for which there exists a solution. We crop it to maxRevolutions
   Nmax = std::min(maxRevolutions, Nmax);
   OTL_WARN_IF(maxRevolutions > Nmax, "Number of revolutions " << Bracket(maxRevolutions) << " not possible, reducing to " << Bracket(Nmax));

   auto& m_v1 = initialVelocities;
   auto& m_v2 = finalVelocities;

   m_v1.resize(Nmax * 2 + 1);
   m_v2.resize(Nmax * 2 + 1);
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

   m_iters[0] = HouseHolder(T, lambda, 0, 1e-8, 15, m_x[0]);

   // Find the x value for each multi rev solution
   double temp;
   for (int i = 1; i < Nmax + 1; ++i)
   {
      // left
      temp = pow((i * MATH_PI + MATH_PI) / (8.0 * T), 2.0 / 3.0);
      m_x[2 * i - 1] = (temp - 1.0) / (temp + 1.0);
      m_iters[2 * i - 1] = HouseHolder(T, lambda, i, 1e-8, 15, m_x[2 * i - 1]);

      // right
      temp = pow((8.0 * T) / (i * MATH_PI), 2.0 / 3.0);
      m_x[2 * i] = (temp - 1.0) / (temp + 1.0);
      m_iters[2 * i] = HouseHolder(T, lambda, i, 1e-8, 15, m_x[2 * i]);
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

int LambertExponentialSinusoid::HouseHolder(double T, double lambda, int N, double eps, int iter_max, double& x0)
{
   int iter = 0;
   double err = 1.0;
   double xnew = 0.0;
   double tof = 0.0, delta = 0.0, DT = 0.0, DDT = 0.0, DDDT = 0.0;
   while (err > eps && iter < iter_max)
   {
      tof = ComputeTimeOfFlight(x0, lambda, N);
      ComputeDerivatives(x0, tof, lambda, DT, DDT, DDDT);
      delta = tof - T;
      double DT2 = SQR(DT);
      xnew = x0 - delta * (DT2 - delta * 0.5 * DDT) / (DT * (DT2 - delta * DDT) + DDDT * SQR(delta) / 6.0);
      err = fabs(x0 - xnew);
      x0 = xnew;
      iter++;
   }

   return iter;
}

void LambertExponentialSinusoid::ComputeDerivatives(double x, double T, double lambda, double& DT, double& DDT, double& DDDT)
{
   double l2 = SQR(lambda);
   double l3 = l2 * lambda;
   double umx2 = 1.0 - SQR(x);
   double y = sqrt(1.0 - l2 * umx2);
   double y2 = SQR(y);
   double y3 = y2*y;
   DT   = 1.0 / umx2 * (3.0 * T * x - 2.0 + 2.0 * l3 * x / y);
   DDT  = 1.0 / umx2 * (3.0 * T + 5.0 * x * DT + 2.0 * (1.0 - l2) * l3 / y3);
   DDDT = 1.0 / umx2 * (7.0 * x * DDT + 8.0 * DT - 6.0 * (1.0 - l2) * l2 * l3 * x / y3 / y2);
}

double LambertExponentialSinusoid::ComputeTimeOfFlight(double x, double lambda, int N)
{
   const double battinThreshold = 0.01;
   const double lagrangeThreshold = 0.2;

   double dist = fabs(x - 1.0);
   double tof = 0.0;

   // Use Lagrange tof expression
   if (dist > battinThreshold && dist < lagrangeThreshold)
   {
      double a = 1.0 / (1.0 - SQR(x));
      if (a > 0)
      {
         double alpha = 2.0 * acos(x);
         double beta = 2.0 * asin(sqrt(SQR(lambda) / a));
         if (lambda < 0.0)
         {
            beta *= -1.0;
         }
         tof = ((a - sqrt(a) * ((alpha - sin(alpha)) - (beta - sin(beta)) + 2.0 * N * MATH_PI)) / 2.0);
      }
      else
      {
         double alpha = 2.0 * acosh(x);
         double beta = 2.0 * asinh(sqrt(-SQR(lambda) / a));
         if (lambda < 0.0)
         {
            beta *= -1.0;
         }
         tof = (-a * sqrt(-a) * ((beta - sinh(beta)) - (alpha - sinh(alpha))) / 2.0);
      }
   }
   else
   {
      double lambdaSquared = SQR(lambda);
      double E = SQR(x) - 1.0;
      double rho = fabs(E);
      double z = sqrt(1.0 + lambdaSquared * E);

      // Use Battin series tof expression
      if (dist < battinThreshold)
      {
         double eta = z - lambda * x;
         double S1 = 0.5 * (1.0 - lambda - x * eta);
         double Q = 4.0 / 3.0 * EvaluateHyperGeometricFunction(3.0, 1.0, 2.5, S1, 1e-8);
         tof = 0.5 * (pow(eta, 3) * Q + 4.0 * lambda * eta) + N * MATH_PI / pow(rho, 1.5);
      }
      // Use Lancaster tof expression
      else
      {
         double y = sqrt(rho);
         double g = x * z - lambda * E;
         double d = 0.0;
         if (E < 0.0)
         {
            double l = acos(g);
            d = N * MATH_PI + l;
         }
         else
         {
            double f = y * (z - lambda * x);
            d = log(f + g);
         }
         tof = (x - lambda * z - d / y) / E;
      }
   }

   return tof;
}

double LambertExponentialSinusoid::EvaluateHyperGeometricFunction(double a, double b, double c, double d, double tol)
{
   double Cj = 1.0;
   double Sj = 1.0;
   double err = 1.0;
   int j = 0;
   while (err < tol)
   {
      double Cj1 = Cj * (a + j) * (b + j) / (c + j) * d / (j + 1.0);
      double Sj1 = Sj + Cj1;
      err = fabs(Cj1);
      Sj = Sj1;
      Cj = Cj1;
      j += 1;
   }

   return Sj;
}

double LambertExponentialSinusoid::EvaluateHyperGeometricFunction(double z, double tol)
{
   double Cj = 1.0;
   double Sj = 1.0;
   double err = 1.0;
   int j = 0;
   while (err < tol)
   {
      double Cj1 = Cj * (3.0 + j) * (1.0 + j) / (2.5 + j) * z / (j + 1.0);
      double Sj1 = Sj + Cj1;
      err = fabs(Cj1);
      Sj = Sj1;
      Cj = Cj1;
      j += 1;
   }

   return Sj;
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