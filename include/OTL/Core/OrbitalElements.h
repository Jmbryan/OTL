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
   double trueAnomaly;        ///< True Anomaly (ta)
   double inclination;        ///< Inclination (i)
   double argOfPericenter;    ///< Argument of pericenter (w)
   double lonOfAscendingNode; ///< Longitude of the ascending node (l)

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   OrbitalElements();

   ////////////////////////////////////////////////////////////
   /// \brief Copy constructor
   ////////////////////////////////////////////////////////////
   OrbitalElements(const OrbitalElements& other);

   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ////////////////////////////////////////////////////////////
   OrbitalElements(const OrbitalElements&& other);

   ////////////////////////////////////////////////////////////
   /// \brief Construct orbital elements from components
   ///
   /// \param _semiMajorAxis Semimajor Axis
   /// \param _eccentricity Eccentricity
   /// \param _trueAnomaly True Anomaly in radians
   /// \param _inclination Inclination in radians
   /// \param _argOfPericenter Arguement of Pericenter in radians
   /// \param _lonOfAscendingNode Longitude of Ascending Node in radians
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements(double _semiMajorAxis, double _eccentricity, double _trueAnomaly,
                   double _inclination, double _argOfPericenter, double _lonOfAscendingNode);

   ////////////////////////////////////////////////////////////
   /// \brief Construct orbital elements from an initializer list
   ///
   /// The orbital elements will be filled in the order that they
   /// are defined. If less than six values are supplied,
   /// the remaining elements are initialized to zero.
   ///
   /// \param list std::initializer_list<double> containing the orbital elements
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements(std::initializer_list<double> list);

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for assignment
   ///
   /// \param other Other OrbitalElements being assigned to 
   /// \returns Reference to this
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements& operator =(const OrbitalElements& other);

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for move
   ///
   /// \param other Other OrbitalElements being moved to 
   /// \returns Reference to this
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements& operator =(const OrbitalElements&& other);

   bool IsZero() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbital elements to a single-line formatted string
   ///
   /// The orbital elements are converted to a single-line string
   /// with the following format:
   ///
   /// "a=[semimajorAxis] e=[eccentricity] ta=[trueAnomaly]deg i=[inclination]deg w=[argOfPericenter]deg l=[lonOfAscendingNode]deg"
   ///
   /// e.g.
   ///
   /// "a=10000 e=0.8 ta=45deg i=5deg w=15deg l=30deg"
   ///
   /// \note Semimajor axis units are not shown because that information is stored in the OrbitalElements
   ///
   /// \returns std::string Stringified orbital elements
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbital elements to a detailed multi-line formatted string
   ///
   /// The orbital elements are converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Orbital Elements:
   ///     Semimajor Axis:              [semiMajorAxis]
   ///     Eccentricity:                [eccentricity]
   ///     True Anomaly:                [trueAnomaly] deg
   ///     Inclination:                 [inclination] deg
   ///     Argument of Pericenter:      [argOfPericenter] deg
   ///     Longitude of Ascending Node: [lonOfAscendingNode] deg
   /// "
   ///
   /// e.g.
   ///
   /// "Orbital Elements:
   ///     Semimajor Axis:              10000.000000
   ///     Eccentricity:                0.800000
   ///     True Anomaly:                45.000000 deg
   ///     Inclination:                 5.000000 deg
   ///     Argument of Pericenter:      15.000000 deg
   ///     Longitude of Ascending Node: 30.000000 deg
   /// "
   ///
   /// \note Semimajor axis units are not shown because that information is stored in the OrbitalElements
   ///
   /// \returns std::string Stringified orbital elements
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString(std::string prefix = "") const;
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates OrbitalElements
///
/// The OrbitalElements is converted to a string by calling the
/// OrbitalElements::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const OrbitalElements& orbitalElements)
{
   stream << orbitalElements.ToString();
   return stream;
}

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator==
/// \relates OrbitalElements
///
/// This operator compares approximate equality between two
/// sets of orbital elements.
///
/// \note Internally, the IsApprox() function is used to compare floating point values with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a OrbitalElements)
/// \param right right operand (a OrbitalElements)
/// \returns True if left is equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator==(const OrbitalElements& lhs, const OrbitalElements& rhs);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator!=
/// \relates OrbitalElements
///
/// This operator compares approximate inequality between two
/// sets of orbital elements.
///
/// \note Internally, the IsApprox() function is used to compare each value with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a OrbitalElements)
/// \param right right operand (a OrbitalElements)
/// \returns True if left is not equal to right
///
////////////////////////////////////////////////////////////
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
/// <li>The True Anomaly defines the current point along the orbit. A true
/// anomaly of zero occurs at the periapsis of the orbit and a true
/// anomaly of 180 degrees occurs at the apoapsis of the orbit.</li>
/// <li>The Inclination, Argument of Pericenter, and Longitude
/// of Ascending Node all define the orientation of the
/// orbit in 3D space. These parameters are unecessary for 2D orbits.</li>
/// </ul>
///
/// \Note Neglecting external disturbances, the true anomaly is the only parameter that varies in time
///
/// \Note The Longitude of Ascending Node is also referred to as the Right Ascension of the Ascending Node (RAAN)
///
////////////////////////////////////////////////////////////