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
#include <OTL/Core/Constants.h>
#include <OTL/Core/Config.h>
#include <OTL/Core/Export.h>
#include <OTL/Core/Logger.h>
#include <OTL/Core/Matrix.h>
#include <OTL/Core/StateVector.h>
#include <OTL/Core/OrbitalElements.h>
#include <OTL/Core/Time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
//#include <cmath>
//#include <numeric>
//#include <memory>

namespace otl
{

enum class EphemerisType
{
   Invalid = -1,
   JplApproximate,
   Jpl,
   Spice,
   Mpcorb,
   Count
};

enum class PropagatorType
{
   Invalid = -1,
   Keplerian,
   Count
};

enum class LambertType
{
   Invalid = -1,
   MultiRev,
   SingleRev,
   Count
};

enum class FlybyType
{
   Invalid = -1,
   Unpowered,
   Powered,
   Count
};

// Additional constants
const Vector3d MATH_UNIT_VEC_I      = Vector3d::UnitX();
const Vector3d MATH_UNIT_VEC_J      = Vector3d::UnitY();
const Vector3d MATH_UNIT_VEC_K      = Vector3d::UnitZ();

/// Returns the square of x
inline double SQR(double x)
{ return x * x; }

/// Returns positive one if x is positive or negative one otherwise
inline int Sign(double x)
{ return (x >= 0.0 ? 1 : -1); }

/// Returns x rounded towards zero
inline double Round0(double x)
{ return (x < 0.0 ? ceil(x) : floor(x)); }

/// Returns the inverse hyperbolic sine of x
inline double asinh(double x)
{ return log(x + sqrt(x * x + 1.0)); }

/// Returns the inverse hyperbolic cosine of x
inline double acosh(double x)
{ return log(x + sqrt(x * x - 1.0)); }

/// Returns the inverse hyperbolic tangent of x
//inline double atanh(double x)
//{ return 0.5 * log((1.0 + x) / (1.0 - x)); }

/// Returns the cotangent of x
inline double cot(double x)
{ return 1.0 / tan(x); }

/// Returns the inverse cotangent of x
inline double acot(double x)
{ return atan(1.0 / x); }

/// Returns the modulo of the floating point number
inline double Modulo(double dividend, double divisor)
{ return dividend - divisor * std::floor(dividend / divisor); }

/// Returns true if the floating point numbers are approximately the same
/// Combines absolute and relative errors and should work equally well for
/// very small and very large numbers.
inline bool IsApprox(double lhs, double rhs, double epsilon = 2.0 * MATH_EPSILON)
{ return std::abs(lhs - rhs) <= std::max({1.0, std::abs(lhs), std::abs(rhs)}) * epsilon; }

/// Returns true if the eccentricity is for a circular orbit
inline bool IsCircular(double eccentricity)
{ return fabs(eccentricity - ASTRO_ECC_CIRCULAR) < MATH_TOLERANCE; }

/// Returns true if the eccentricity is for a elliptical orbit
inline bool IsElliptical(double eccentricity)
{ return eccentricity > (ASTRO_ECC_CIRCULAR + MATH_TOLERANCE) && eccentricity < (ASTRO_ECC_PARABOLIC - MATH_TOLERANCE); }

/// Returns true if the eccentricity is for a hyperbolic orbit
inline bool IsHyperbolic(double eccentricity)
{ return eccentricity > (ASTRO_ECC_PARABOLIC + MATH_TOLERANCE); }

/// Returns true if the eccentricity is for a parabolic orbit
inline bool IsParabolic(double eccentricity)
{ return fabs(eccentricity - ASTRO_ECC_PARABOLIC) < MATH_TOLERANCE; }

/// Returns true if the eccentricity is for a circular or elliptical orbit
inline bool IsCircularOrElliptical(double eccentricity)
{ return eccentricity > (ASTRO_ECC_CIRCULAR - MATH_TOLERANCE) && eccentricity < (ASTRO_ECC_PARABOLIC - MATH_TOLERANCE); }

} // namespace otl