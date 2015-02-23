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

#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
IKeplersEquation::IKeplersEquation(int maxIterations, double tolerance)
{
   SetMaxIterations(maxIterations);
   SetTolerance(tolerance);
}

////////////////////////////////////////////////////////////
IKeplersEquation::~IKeplersEquation()
{

}

////////////////////////////////////////////////////////////
void IKeplersEquation::SetMaxIterations(int maxIterations)
{
    if (maxIterations <= 0)
    {
        OTL_ERROR() << "Max iterations " << Bracket(maxIterations) << " must be greater than zero. Setting to 1000.";
        maxIterations = 0;
    }
    m_maxIterations = maxIterations;
}

////////////////////////////////////////////////////////////
void IKeplersEquation::SetTolerance(double tolerance)
{
    if (tolerance <= 0.0)
    {
        OTL_ERROR() << "Tolerance " << Bracket(tolerance) << " must be greater than zero. Setting to 1e-8";
        tolerance = 1e-8;
    }
    m_tolerance = tolerance;
}

////////////////////////////////////////////////////////////
double IKeplersEquation::Evaluate(double eccentricity, double meanAnomaly)
{
   // Determine the initial guess
   double anomaly = CalculateInitialGuess(eccentricity, meanAnomaly);

   // Newton-Raphson iteration
   int iteration = 0;
   double ratio = MATH_INFINITY;
   while (iteration++ < m_maxIterations && fabs(ratio) > m_tolerance)
   {
      double numerator = SolveInverseEquation(eccentricity, anomaly) - meanAnomaly;
      double denominator = SolveInverseDerivative(eccentricity, anomaly);
      if (fabs(denominator) > MATH_NEAR_ZERO)
      {
         ratio = numerator / denominator;
      }
      else
      {
          OTL_WARN() << "IKeplersEquation::Evaluate: SolveInverseDerivative() must return greater than or equal to zero, but returned " << Bracket(denominator);
          break;
      }
      anomaly -= ratio;
   }
   if (iteration >= m_maxIterations)
   {
       OTL_WARN() << "IKeplersEquation::Evaluate: Max iterations " << Bracket(m_maxIterations) << " exceeded!";
   }

   return anomaly;
}

////////////////////////////////////////////////////////////
KeplersEquationElliptical::KeplersEquationElliptical(int maxIterations, double tolerance) :
IKeplersEquation(maxIterations, tolerance)
{
}

////////////////////////////////////////////////////////////
double KeplersEquationElliptical::Evaluate(double eccentricity, double meanAnomaly)
{
   return IKeplersEquation::Evaluate(eccentricity, meanAnomaly);
}

////////////////////////////////////////////////////////////
double KeplersEquationElliptical::CalculateInitialGuess(double eccentricity, double meanAnomaly)
{
   double eccentricAnomaly;
   if (meanAnomaly < MATH_PI)
      eccentricAnomaly = meanAnomaly + eccentricity / 2.0;
   else
      eccentricAnomaly = meanAnomaly - eccentricity / 2.0;
   return eccentricAnomaly;
}

////////////////////////////////////////////////////////////
double KeplersEquationElliptical::SolveInverseEquation(double eccentricity, double eccentricAnomaly)
{
   return (eccentricAnomaly - eccentricity * sin(eccentricAnomaly));
}

////////////////////////////////////////////////////////////
double KeplersEquationElliptical::SolveInverseDerivative(double eccentricity, double eccentricAnomaly)
{
   return (1.0 - eccentricity * cos(eccentricAnomaly));
}

////////////////////////////////////////////////////////////
KeplersEquationHyperbolic::KeplersEquationHyperbolic(int maxIterations, double tolerance) :
IKeplersEquation(maxIterations, tolerance)
{
}

////////////////////////////////////////////////////////////
double KeplersEquationHyperbolic::Evaluate(double eccentricity, double meanAnomaly)
{
   return IKeplersEquation::Evaluate(eccentricity, meanAnomaly);
}

////////////////////////////////////////////////////////////
double KeplersEquationHyperbolic::CalculateInitialGuess(double eccentricity, double meanAnomaly)
{
   double hyperbolicAnomaly = meanAnomaly;
   return hyperbolicAnomaly;
}

////////////////////////////////////////////////////////////
double KeplersEquationHyperbolic::SolveInverseEquation(double eccentricity, double hyperbolicAnomaly)
{
   return (eccentricity * sinh(hyperbolicAnomaly) - hyperbolicAnomaly);
}

////////////////////////////////////////////////////////////
double KeplersEquationHyperbolic::SolveInverseDerivative(double eccentricity, double hyperbolicAnomaly)
{
   return (eccentricity * cosh(hyperbolicAnomaly) - 1.0);
}

////////////////////////////////////////////////////////////
double SolveKeplersEquation(double eccentricity, double meanAnomaly, int maxIterations, double tolerance)
{
   if (IsCircularOrElliptical(eccentricity))
   {
      KeplersEquationElliptical kepler(maxIterations, tolerance);
      return kepler.Evaluate(eccentricity, meanAnomaly);
   }
   else if (IsHyperbolic(eccentricity))
   {
      KeplersEquationHyperbolic kepler(maxIterations, tolerance);
      return kepler.Evaluate(eccentricity, meanAnomaly);
   }
   else if (IsParabolic(eccentricity))
   {
      return meanAnomaly;
   }

   OTL_ERROR() << "Invalid orbit type";
   return 0.0;
}

} // namespace keplerian

} // namespace otl