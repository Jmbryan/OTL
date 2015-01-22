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

using Vector3d = Eigen::Vector3d;
using Matrix3d = Eigen::Matrix3d;

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
   ///
   /// \param other Other StateVector being assigned to 
   ///
   ////////////////////////////////////////////////////////////
   StateVector(const StateVector& other);

   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ///
   /// \param other Other StateVector being assigned to 
   ///
   ////////////////////////////////////////////////////////////
   StateVector(const StateVector&& other);

   ////////////////////////////////////////////////////////////
   /// \brief Construct a state vector from position and velocity vectors
   ///
   /// \param _position Absolute position
   /// \param _velocity Absolute velocity
   ///
   ////////////////////////////////////////////////////////////
   StateVector(const Vector3d& _position, const Vector3d& _velocity);

   ////////////////////////////////////////////////////////////
   /// \brief Construct a state vector from position and velocity elements
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

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for equality
   ///
   /// \param other Other StateVector being compared to 
   /// \returns True if the state vectors are equal to within tolerance
   ///
   ////////////////////////////////////////////////////////////
   bool operator==(const StateVector& other);

   ////////////////////////////////////////////////////////////
   /// \brief Operator overload for inequality
   ///
   /// \param other Other StateVector being compared to 
   /// \returns True if the state vectors are not equal to within tolerance
   ///
   ////////////////////////////////////////////////////////////
   bool operator!=(const StateVector& other);
};

template<typename T>
T& operator<<(T& stream, const StateVector& stateVector)
{
   stream << "x=" << stateVector.position.x() << " "
      << "y=" << stateVector.position.y() << " "
      << "z=" << stateVector.position.z() << " "
      << "vx=" << stateVector.velocity.x() << " "
      << "vy=" << stateVector.velocity.y() << " "
      << "vz=" << stateVector.velocity.z();
   return stream;
}

OTL_CORE_API std::string HumanReadable(const StateVector& stateVector);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::StateVector
///
/// Basic construct representing a three dimensional
/// position and velocity in space.
///
/// In general, six elements are required to completely
/// define a keplerian orbit in three dimensional space.
/// The StateVector is one common way of expressing the
/// six elements, the other being the OrbitalElements.
///
////////////////////////////////////////////////////////////