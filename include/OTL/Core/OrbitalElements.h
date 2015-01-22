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
#include <string>

namespace otl
{

struct OTL_CORE_API OrbitalElements
{
   double semiMajorAxis;      ///< SemiMajor axis (a)
   double eccentricity;       ///< Eccentricity (e)
   double inclination;        ///< Inclination (i)
   double argOfPericenter;    ///< Argument of pericenter (omega, w)
   double lonOfAscendingNode; ///< Longitude of the ascending node (l)
   double trueAnomaly;        ///< True Anomaly (theta, t)

   OrbitalElements();
   OrbitalElements(const OrbitalElements& other);
   OrbitalElements(const OrbitalElements&& other);
   OrbitalElements(double _semiMajorAxis, double _eccentricity, double _inclination,
                   double _argOfPericenter, double _lonOfAscendingNode, double _trueAnomaly);
   
};

template<typename T>
T& operator<<(T& stream, const OrbitalElements& orbitalElements)
{
   stream << "a=" << orbitalElements.semiMajorAxis << " "
      << "e=" << orbitalElements.eccentricity << " "
      << "i=" << orbitalElements.inclination << " "
      << "w=" << orbitalElements.argOfPericenter << " "
      << "l=" << orbitalElements.lonOfAscendingNode << " "
      << "t=" << orbitalElements.trueAnomaly;
   return stream;
}

OTL_CORE_API std::string HumanReadable(const OrbitalElements& orbitalElements);

/// Returns true if the orbital elements are identical
OTL_CORE_API bool operator==(const OrbitalElements& lhs, const OrbitalElements& rhs);
OTL_CORE_API bool operator!=(const OrbitalElements& lhs, const OrbitalElements& rhs);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::OrbitalElements
///
/// Basic construct representing the six classical orbital elements
///
/// In general, six elements are required to completely
/// define a keplerian orbit in three dimensional space.
/// The OrbitalElements is one common way of expressing the
/// six elements, the other being the StateVector.
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
/// orbit in 3D space. These parameters are unecessary for 2D orbits.</li>
/// <li>The true anomaly defines the current point along the orbit. A true
/// anomaly of zero occurs at the periapsis of the orbit and a true
/// anomaly of 180 degrees occurs at the apoapsis of the orbit.</li>
/// </ul>
///
/// \Note Neglecting external disturbances, the true anomaly is the only parameter that varies in time
///
/// \Note The Longitude of Ascending Node is also referred to as the Right Ascension of the Ascending Node (RAAN)
///
////////////////////////////////////////////////////////////