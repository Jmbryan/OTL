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

void PropagateAnalytical::Propagate(Orbit& orbit, double seconds)
{
   const double mu = orbit.GetMu();

   const OrbitalElements& orbitalElements = orbit.GetOrbitalElements();
   const double a = orbitalElements.semiMajorAxis;
   const double e = orbitalElements.eccentricity;
   double TA = orbitalElements.trueAnomaly;

   if (orbit.GetType() == Orbit::Type::Elliptical)
   {
      double T = 2.0 * MATH_2_PI * pow(a, 1.5) / sqrt(mu);

      double E1 = 2.0 * atan(sqrt((1.0 - e) / (1.0 + e)) * tan(TA / 2.0));
      double M1 = E1 - e * sin(E1);
      double t1 = M1 * T / MATH_2_PI;

      double t2 = seconds + t1;
      double M2 = MATH_2_PI * t2 / T;

      double E2 = SolveKeplersEquationElliptical(e, M2);

      TA = 2.0 * atan(sqrt((1.0 + e) / (1.0 - e)) * tan(E2 / 2.0));
   }
   else
   {
      double h = sqrt(mu * a * (1.0 - SQR(e)));

      double F1 = 2.0 * atanh(sqrt((e - 1.0) / (e + 1.0)) * tan(TA / 2.0));
      double M1 = e * sinh(F1) - F1;
      double t1 = M1 * pow(h, 3.0) / SQR(mu) / pow(SQR(e) - 1.0, 1.5);

      double t2 = seconds + t1;
      double M2 = t2 / pow(h, 3.0) * SQR(mu) * pow(SQR(e) - 1.0, 1.5);
      double F2 = SolveKeplersEquationHyperbolic(e, M2);

      TA = 2.0 * atan(sqrt((e + 1.0) / (e - 1.0)) * tanh(F2 / 2.0));
   }

   orbit.SetTrueAnomaly(TA);
}

} // namespace otl