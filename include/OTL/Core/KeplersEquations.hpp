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

#pragma once
#include <OTL/Core/Base.hpp>

namespace otl
{

namespace keplerian
{

class EccentricAnomalyEquation
{
public:
   double operator() (double eccentricity, double eccentricAnomaly, double meanAnomaly) const;
};

class EccentricAnomalyDerivative
{
public:
   double operator() (double eccentricity, double eccentricAnomaly) const;
};

class HyperbolicAnomalyEquation
{
public:
   double operator() (double eccentricity, double hyperbolicAnomaly, double meanAnomaly) const;
};

class HyperbolicAnomalyDerivative
{
public:
   double operator() (double eccentricity, double hyperbolicAnomaly) const;
};

double SolveKeplersEquationElliptical(double eccentricity,
                                      double meanAnomaly,
                                      int maxIterations = 1000,
                                      double tolerance = MATH_TOLERANCE);

double SolveKeplersEquationHyperbolic(double eccentricity,
                                      double meanAnomaly,
                                      int maxIterations = 1000,
                                      double tolerance = MATH_TOLERANCE);


#include <OTL/Core/KeplersEquations.inl>

} // namespace keplerian

} // namespace otl