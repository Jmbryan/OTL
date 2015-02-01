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
#define EIGEN_MATRIXBASE_PLUGIN <OTL/Core/MatrixPlugin.h>
#include <Eigen/Dense>

namespace otl
{

using Vector3d = Eigen::Vector3d; ///< Alias the Eigen Vector3d class
using Matrix3d = Eigen::Matrix3d; ///< Alias the Eigen Matrix3d class

struct OTL_CORE_API StateVector
{
   Vector3d position;   ///< 3-dimensional position vector
   Vector3d velocity;   ///< 3-dimensional velocity vector

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   StateVector();

   ////////////////////////////////////////////////////////////
   /// \brief Copy constructor
   ////////////////////////////////////////////////////////////
   StateVector(const StateVector& other);

   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ////////////////////////////////////////////////////////////
   StateVector(const StateVector&& other);

   ////////////////////////////////////////////////////////////
   /// \brief Construct state vector from position and velocity vectors
   ///
   /// \param _position Absolute position
   /// \param _velocity Absolute velocity
   ///
   ////////////////////////////////////////////////////////////
   StateVector(const Vector3d& _position, const Vector3d& _velocity);

   ////////////////////////////////////////////////////////////
   /// \brief Construct state vector from position and velocity components
   ///
   /// \param x X component of position vector
   /// \param y Y component of position vector
   /// \param z Z component of position vector
   /// \param x X component of velocity vector
   /// \param y Y component of velocity vector
   /// \param z Z component of velocity vector
   ///
   ////////////////////////////////////////////////////////////
   StateVector(double x, double y, double z, double vx, double vy, double vz);

   ////////////////////////////////////////////////////////////
   /// \brief Construct state vector from an initializer list
   ///
   /// The position vector will be filled first followed by the
   /// velocity vector. If less than six values are supplied,
   /// the remaining components are initialized to zero.
   ///
   /// \param list std::initializer_list<double> containing the position and velocity elements
   ///
   ////////////////////////////////////////////////////////////
   StateVector(std::initializer_list<double> list);

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for assignment
   ///
   /// \param other Other StateVector being assigned to 
   /// \returns Reference to this
   ///
   ////////////////////////////////////////////////////////////
   StateVector& operator =(const StateVector& other);

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for move
   ///
   /// \param other Other StateVector being moved to 
   /// \returns Reference to this
   ///
   ////////////////////////////////////////////////////////////
   StateVector& operator =(const StateVector&& other);

   bool IsZero() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the state vector to a single-line formatted string
   ///
   /// The state vector is converted to a single-line string
   /// with the following format:
   ///
   /// "x=[x position] y=[y position] z=[z position] vx=[x velocity] vy=[y velocity] vz=[z velocity]"
   ///
   /// e.g. "x=10000.0 y=8000.0 z=0.0 vx=2.5 vy=2.4 vz=0.0"
   ///
   /// \note Units are not shown because that information is not stored in the StateVector
   ///
   /// \returns std::string Stringified state vector
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the state vector to a detailed multi-line formatted string
   ///
   /// The state vector is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "State Vector:
   ///     Position:
   ///        X: [x position]
   ///        Y: [y position]
   ///        Z: [z position]
   ///     Velocity:
   ///        X: [x velocity]
   ///        Y: [y velocity]
   ///        Z: [z velocity]
   /// "
   ///
   /// e.g.
   ///
   /// "State Vector:
   ///     Position:
   ///        X: 10000.0
   ///        Y: 8000.0
   ///        Z: 0.0
   ///     Velocity:
   ///        X: 2.5
   ///        Y: 2.4
   ///        Z: 0.0
   /// "
   /// \note Units are not shown because that information is not stored in the StateVector
   ///
   /// \returns std::string Stringified state vector
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString(std::string prefix = "") const;
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates StateVector
///
/// The StateVector is converted to a string by calling the
/// StateVector::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const StateVector& stateVector)
{
   stream << stateVector.ToString();
   return stream;
}

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator==
/// \relates StateVector
///
/// This operator compares approximate equality between two
/// state vectors.
///
/// \note Internally, the Eigen::isApprox() function is used to compare each vector with epsilon = 2 * MATH_EPSILON
/// \warning This function cannot be used to check whether the position and velocity vectors are approximately equal to the zero vector
///
/// \param left Left operand (a StateVector)
/// \param right right operand (a StateVector)
/// \returns True if left is equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator==(const StateVector& lhs, const StateVector& rhs);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator!=
/// \relates StateVector
///
/// This operator compares approximate inequality between two
/// state vectors.
///
/// \note Internally, the Eigen::isApprox() function is used to compare each vector with epsilon = 2 * MATH_EPSILON
/// \warning This function cannot be used to check whether the position and velocity vectors are approximately inequal to the zero vector
///
/// \param left Left operand (a StateVector)
/// \param right right operand (a StateVector)
/// \returns True if left is not equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator!=(const StateVector& lhs, const StateVector& rhs);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::StateVector
///
/// Basic construct representing a three dimensional
/// position and velocity in space
///
/// In general, six elements are required to completely
/// define a keplerian orbit in three dimensional space.
/// The StateVector is one common way of expressing the
/// six elements, the other being the OrbitalElements.
///
////////////////////////////////////////////////////////////