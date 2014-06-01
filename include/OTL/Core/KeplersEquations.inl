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

////////////////////////////////////////////////////////////
inline double EccentricAnomalyEquation::operator() (double eccentricity, double eccentricAnomaly, double meanAnomaly) const
{
   return (eccentricAnomaly - eccentricity * sin(eccentricAnomaly) - meanAnomaly);
}

////////////////////////////////////////////////////////////
inline double EccentricAnomalyDerivative::operator() (double eccentricity, double eccentricAnomaly) const
{
   return (1.0 - eccentricity * cos(eccentricAnomaly));
}

////////////////////////////////////////////////////////////
inline double HyperbolicAnomalyEquation::operator() (double eccentricity, double hyperbolicAnomaly, double meanAnomaly) const
{
   return (eccentricity * sinh(hyperbolicAnomaly) - hyperbolicAnomaly - meanAnomaly);
}

////////////////////////////////////////////////////////////
inline double HyperbolicAnomalyDerivative::operator() (double eccentricity, double hyperbolicAnomaly) const
{
   return (eccentricity * cosh(hyperbolicAnomaly) - 1.0);
}

////////////////////////////////////////////////////////////
double SolveKeplersEquationElliptical(double eccentricity,
                                      double meanAnomaly,
                                      int maxIterations,
                                      double tolerance)
{
   double eccentricAnomaly;
   if (meanAnomaly < MATH_PI)
      eccentricAnomaly = meanAnomaly + eccentricity / 2.0;
   else
      eccentricAnomaly = meanAnomaly - eccentricity / 2.0;

   int iteration = 0;
   double ratio = MATH_INFINITY;
   while (iteration++ < maxIterations && fabs(ratio) > tolerance)
   {
      ratio = EccentricAnomalyEquation()(eccentricity, eccentricAnomaly, meanAnomaly) /
              EccentricAnomalyDerivative()(eccentricity, eccentricAnomaly);
      eccentricAnomaly -= ratio;
   }
   if (iteration >= maxIterations)
   {
      std::cout << "SolveKeplersEquationElliptical(): Max iterations exceeded!" << std::endl;
   }

   return eccentricAnomaly;
}

////////////////////////////////////////////////////////////
double SolveKeplersEquationHyperbolic(double eccentricity,
                                      double meanAnomaly,
                                      int maxIterations,
                                      double tolerance)
{
   double hyperbolicAnomaly = meanAnomaly;

   int iteration = 0;
   double ratio = MATH_INFINITY;
   while (iteration++ < maxIterations && fabs(ratio) > tolerance)
   {
      ratio = HyperbolicAnomalyEquation()(eccentricity, hyperbolicAnomaly, meanAnomaly) /
              HyperbolicAnomalyDerivative()(eccentricity, hyperbolicAnomaly);
      hyperbolicAnomaly -= ratio;
   }
   if (iteration >= maxIterations)
   {
      std::cout << "SolveKeplersEquationHyperbolic(): Max iterations exceeded!" << std::endl;
   }

   return hyperbolicAnomaly;
}