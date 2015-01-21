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
#include <OTL/Core/Export.h>

namespace otl
{

struct OTL_CORE_API OrbitalElements
{
   double semiMajorAxis;      ///< SemiMajor axis (a)
   double eccentricity;       ///< Eccentricity (e)
   double inclination;        ///< Inclination (i)
   double argOfPericenter;    ///< Argument of pericenter (omega)
   double lonOfAscendingNode; ///< Longitude of the ascending node ()
   double trueAnomaly;        ///< True Anomaly (theta)
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::OrbitalElements
///
/// Basic construct representing a three dimensonal orbit in space.
///
/// <ul>
/// <li>The Semimajor Axis defines the length of the primary axis</li>
/// <li>The Eccentricity defines the shape of the orbit
/// <ul>
/// <li>0 for circular orbits</li>
/// <li>(0, 1) for elliptical orbits</li>
/// <li>1 for parabolic orbits</li>
/// <li>(1, infinity) for hyperbolic orbits</li>
/// </ul>
/// <li>The Inclination, Argument of Pericenter, and Longitude
/// of Ascending Node all define the orientation of the
/// orbit in 3D space. These parameters are uncessary for 2D orbits.</li>
/// <li>The true anomaly defines the current point on the orbit. Neglecting
/// external disturbances, this is the only parameter that varies
/// in time.</li>
/// </ul>
///
/// The Longitude of Ascending Node is also sometimes referred
/// to as the Right Ascension of the Ascending Node (RAAN).
///
////////////////////////////////////////////////////////////