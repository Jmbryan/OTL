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
#include <OTL/Core/Matrix.h>

namespace otl
{

// Forward declarations
struct StateVector;
struct OrbitalElements;

enum class StateVectorType
{
   Invalid = -1,  ///< Invalid state vector type
   Generic,       ///< The state vector is represented as a Vector6d which is generic 6-dimensional vector
   Cartesian,     ///< The state vector is represented as a CartesianStateVector consisting of 3-dimensional position and velocity vectors
   Orbital,       ///< The state vector is represented as a OrbitalElements consisting of the 6 classic orbital elements
   Count          ///< Number of state vector types
};

namespace test
{

typedef otl::StateVector CartesianStateVector;

class StateVector
{
public:
   StateVector();
   StateVector(const StateVector& other);
   explicit StateVector(const Vector6d& genericStateVector);
   explicit StateVector(const OrbitalElements& orbitalElements);
   explicit StateVector(const CartesianStateVector& cartesianStateVector);

   StateVector& operator =(const StateVector& other);
   StateVector& operator =(const Vector6d& genericStateVector);
   StateVector& operator =(const CartesianStateVector& cartesianStateVector);
   StateVector& operator =(const OrbitalElements& orbitalElements);

   StateVectorType GetType() const;

   const Vector6d& GetGenericStateVector() const;
   CartesianStateVector GetCartesianStateVector() const;
   OrbitalElements GetOrbitalElements() const;

   CartesianStateVector ToCartesianStateVector(double mu) const;
   OrbitalElements ToOrbitalElements(double mu) const;

private:
   StateVectorType m_type;
   Vector6d m_state;
};

}

} // namespace otl