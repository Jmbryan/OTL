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
//#include <OTL/Core/CartesianStateVector.h>
//#include <OTL/Core/OrbitalElements.h>

#include <OTL/Core/Matrix.h>
#include <vector>

//#include <boost/variant.hpp>

namespace otl
{

// Forward declarations
struct CartesianStateVector;
struct OrbitalElements;

enum class StateVectorType
{
   Invalid = -1,  ///< Invalid state vector type
   Generic,       ///< The state vector is represented as a Vector6d which is generic 6-dimensional vector
   Cartesian,     ///< The state vector is represented as a CartesianStateVector consisting of 3-dimensional position and velocity vectors
   Orbital,       ///< The state vector is represented as a OrbitalElements consisting of the 6 classic orbital elements
   Count          ///< Number of state vector types
};

class StateVector
{
public:
   StateVector();
   StateVector(const StateVector& other);
   StateVector(const StateVector&& other);
   //explicit StateVector(const Vector6d& genericStateVector);
   explicit StateVector(const OrbitalElements& orbitalElements);
   explicit StateVector(const CartesianStateVector& cartesianStateVector);
   ~StateVector();

   StateVector& operator =(const StateVector& other);
   StateVector& operator =(const StateVector&& other);
   //StateVector& operator =(const Vector6d& genericStateVector);
   StateVector& operator =(const CartesianStateVector& cartesianStateVector);
   StateVector& operator =(const OrbitalElements& orbitalElements);

   StateVectorType GetType() const;

   //Vector6d GetGenericStateVector() const;
   //const double* GetState() const;
   //std::vector<double> GetState() const;
   const Vector6d& GetState() const;
   CartesianStateVector GetCartesianStateVector() const;
   OrbitalElements GetOrbitalElements() const;

   CartesianStateVector ToCartesianStateVector(double mu) const;
   OrbitalElements ToOrbitalElements(double mu) const;

   double GetX() const;
   double GetSemimajorAxis() const;

   //void Set(const Vector6d& genericStateVector, const StateVectorType& stateVectorType);

private:
   StateVectorType m_type;
   //Vector6d m_state;
   //double m_state[6];
   //std::vector<double> m_state;
   Vector6d m_state;
};

//namespace test
//{
//
//class StateVector2
//{
//public:
//   StateVector2(const CartesianStateVector& cartesianStateVector);
//   StateVector2(const OrbitalElements& orbitalElements);
//
//   const CartesianStateVector& GetCartesianStateVector() const;
//
//private:
//   StateVectorType m_type;
//   boost::variant<Vector6d, CartesianStateVector, OrbitalElements> m_state;
//};
//
//}

} // namespace otl