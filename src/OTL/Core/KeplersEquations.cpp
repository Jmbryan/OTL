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
void IKeplersEquation::SetMaxIterations(int maxIterations)
{
   OTL_ASSERT(maxIterations > 0, "Max iterations must be greater than zero");
   m_maxIterations = maxIterations;
}

////////////////////////////////////////////////////////////
void IKeplersEquation::SetTolerance(double tolerance)
{
   OTL_ASSERT(tolerance > 0.0, "Tolerance must be greater than zero");
   m_tolerance = tolerance;
}

////////////////////////////////////////////////////////////
double IKeplersEquation::Evaluate(double eccentricity,
                                  double meanAnomaly)
{
   // Determine the initial guess
   double anomaly = CalculateInitialGuess(eccentricity, meanAnomaly);

   // Newton-Raphson iteration
   int iteration = 0;
   double ratio = MATH_INFINITY;
   while (iteration++ < m_maxIterations && fabs(ratio) > m_tolerance)
   {
      double numerator = meanAnomaly - SolveInverseEquation(eccentricity, anomaly);
      double denominator = SolveInverseDerivative(eccentricity, anomaly);
      if (fabs(denominator) > MATH_NEAR_ZERO)
      {
         ratio = numerator / denominator;
      }
      else
      {
         std::cout << "IKeplersEquation::Evaluate: SolveInverseDerivative() returned zero!" << std::endl;
         break;
      }
      anomaly -= ratio;
   }
   if (iteration >= m_maxIterations)
   {
      std::cout << "IKeplersEquation::Evaluate: Max iterations exceeded!" << std::endl;
   }

   return anomaly;
}

////////////////////////////////////////////////////////////
KeplersEquationElliptical::KeplersEquationElliptical(int maxIterations, double tolerance)
: IKeplersEquation(maxIterations, tolerance)
{
}

////////////////////////////////////////////////////////////
KeplersEquationHyperbolic::KeplersEquationHyperbolic(int maxIterations, double tolerance)
: IKeplersEquation(maxIterations, tolerance)
{
}

} // namespace keplerian

} // namespace otl