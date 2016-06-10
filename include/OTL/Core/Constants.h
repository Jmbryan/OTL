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
#include <limits>

namespace otl
{

// Math constants
const double MATH_DEG_TO_RAD = 0.0174532925;
const double MATH_RAD_TO_DEG = 57.29577951;
const double MATH_NEAR_ZERO = 2.0e-37;
const double MATH_TOLERANCE = 1.0e-8;
const double MATH_INFINITY = std::numeric_limits<double>::infinity();
const double MATH_EPSILON = std::numeric_limits<double>::epsilon();
const double MATH_E = 2.71828182845904523536;
const double MATH_LOG10E = 0.4342944819032518;
const double MATH_LOG2E = 1.442695040888963387;
const double MATH_PI = 3.14159265358979323846;
const double MATH_2_PI = 6.28318530717958647693;
const double MATH_1_OVER_PI = 0.31830988618379067154;
const double MATH_PI_OVER_2 = 1.57079632679489661923;
const double MATH_PI_OVER_4 = 0.785398163397448309616;
const double MATH_YEAR_TO_SEC = 31557600.0;
const double MATH_DAY_TO_SEC = 86400.0;
const double MATH_HOUR_TO_SEC = 3600.0;
const double MATH_MIN_TO_SEC = 60.0;
const double MATH_SEC_TO_YEAR = 1.0 / MATH_YEAR_TO_SEC;
const double MATH_SEC_TO_DAY = 1.0 / MATH_DAY_TO_SEC;
const double MATH_SEC_TO_HOUR = 1.0 / MATH_HOUR_TO_SEC;
const double MATH_SEC_TO_MIN = 1.0 / MATH_MIN_TO_SEC;

// Astrodynamics constants
const double ASTRO_GRAVITATIONAL_CONSTANT = 6.67384e-11;      // Universal gravitational constant (m^3 kg^-1 s^-2)
const double ASTRO_AU_TO_KM = 149597870.66;     // Convert Astronomical Units (AU) to km
const double ASTRO_ER_TO_KM = 6378.137;         // Mean equitorial radius of Earth (ER) to km
const double ASTRO_ECC_CIRCULAR = 0.0;              // Eccentricity of a ciruclar orbit
const double ASTRO_ECC_PARABOLIC = 1.0;              // Eccentricity of a parabolic orbit
const double ASTRO_INCL_EQUATORIAL = 0.0;              // Inclination of an equatorial orbit
const double ASTRO_MU_SUN = 132712428000.0;   // Gravitional Parameter of the Sun (km3/s2)
const double ASTRO_MU_MERCURY = 22032.0;
const double ASTRO_MU_VENUS = 325700.0;
const double ASTRO_MU_EARTH = 398600.4418;      // Gravitional Parameter of the Earth
const double ASTRO_MU_MARS = 43050.0;
const double ASTRO_MU_JUPITER = 126800000.0;
const double ASTRO_MU_SATURN = 37940000.0;
const double ASTRO_MU_URANUS = 5794000.0;
const double ASTRO_MU_NEPTUNE = 6809000.0;
const double ASTRO_MU_PLUTO = 900.0;
const double ASTRO_RADIUS_SUN = 696000.0;
const double ASTRO_RADIUS_MERCURY = 2439.0;
const double ASTRO_RADIUS_VENUS = 6052.0;
const double ASTRO_RADIUS_EARTH = 6378.1363;      // Radius of the Earth
const double ASTRO_RADIUS_MARS = 3397.2;
const double ASTRO_RADIUS_JUPITER = 71492.0;
const double ASTRO_RADIUS_SATURN = 60268.0;
const double ASTRO_RADIUS_URANUS = 25559.0;
const double ASTRO_RADIUS_NEPTUNE = 24764.0;
const double ASTRO_RADIUS_PLUTO = 1151.0;

} // namespace otl