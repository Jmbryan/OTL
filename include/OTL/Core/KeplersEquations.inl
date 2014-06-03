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
double KeplersEquationElliptical::Evaluate(double eccentricity,
                                           double meanAnomaly)
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
double KeplersEquationHyperbolic::Evaluate(double eccentricity,
                                           double meanAnomaly)
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