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
#include <OTL/Core/CartesianStateVector.h>
#include <OTL/Core/OrbitalElements.h>
#include <OTL/Core/Matrix.h>
#include <OTL/Core/Logger.h>
#include <vector>
#include <array>

namespace otl
{

// Forward declarations
//struct CartesianStateVector;
//struct OrbitalElements;


class StateVector
{
public:
   explicit StateVector(double mu = 1.0);
   StateVector(const StateVector& other);
   StateVector(const StateVector&& other);
   explicit StateVector(const OrbitalElements& orbitalElements, double mu);
   explicit StateVector(const CartesianStateVector& cartesianStateVector, double mu);

   StateVector& operator =(const StateVector& other);
   StateVector& operator =(const StateVector&& other);

   double GetGravitationalParameterCentralBody() const;
   const OrbitalElements& GetOrbital() const;
   const CartesianStateVector& GetCartesian() const;

   std::string ToString(const std::string& prefix = "");

private:
   void UpdateOrbitalElements() const;
   void UpdateCartesianStateVector() const;

private:
   double mu;
   mutable OrbitalElements m_orbitalElements;
   mutable CartesianStateVector m_cartesianStateVector;
   mutable bool m_orbitalElementsDirty;
   mutable bool m_cartesianStateVectorDirty;
};



enum class StateVectorType
{
   Invalid = -1,        ///< Invalid state vector type
   //Generic,       ///< The state vector is represented as a Vector6d which is generic 6-dimensional vector
   Cartesian,           ///< The state vector is represented as a CartesianStateVector consisting of 3-dimensional position and velocity vectors
   Orbital,             ///< The state vector is represented as a OrbitalElements consisting of the 6 classic orbital elements
   CartesianAndOrbital, ///< The state vector is represented as both a CartesianStateVector and OrbitalElements
   Count                ///< Number of state vector types
};

namespace test1
{
class StateVector
{
public:
   typedef std::array<double, 6> StateVectorData;

   StateVector();
   StateVector(const StateVector& other);
   StateVector(const StateVector&& other);  
   explicit StateVector(const OrbitalElements& orbitalElements);
   explicit StateVector(const CartesianStateVector& cartesianStateVector);
   StateVector(double x1, double x2, double x3, double x4, double x5, double x6, const StateVectorType& type);
   StateVector(const Vector3d& a, const Vector3d& b, const StateVectorType& type);
   StateVector(const Vector6d& state, const StateVectorType& type);
   StateVector(const std::vector<double>& state, const StateVectorType& type);
   StateVector(double* state, const StateVectorType& type);   
   ~StateVector();

   StateVector& operator =(const StateVector& other);
   StateVector& operator =(const StateVector&& other);
   StateVector& operator =(const CartesianStateVector& cartesianStateVector);
   StateVector& operator =(const OrbitalElements& orbitalElements);

   bool IsCartesian() const;
   bool IsOrbital() const;
   bool IsType(const StateVectorType& type) const;
   StateVectorType GetType() const;

   const StateVectorData& GetState() const;
   //Vector6d GetGenericStateVector() const;
   //const double* GetState() const;
   //std::vector<double> GetState() const;
   //const Vector6d& GetState() const;
   //const State& GetState() const;
   //const StatePtr& GetState() const;
   //const CartesianStateVector& GetCartesianStateVector() const;
   //const OrbitalElements& GetOrbitalElements() const;
   CartesianStateVector GetCartesianStateVector() const;
   OrbitalElements GetOrbitalElements() const;

   CartesianStateVector ToCartesianStateVector(double mu) const;
   OrbitalElements ToOrbitalElements(double mu) const;

   void Set(double x1, double x2, double x3, double x4, double x5, double x6, const StateVectorType& type);
   void Set(const Vector3d& a, const Vector3d& b, const StateVectorType& type);
   void Set(const Vector6d& state, const StateVectorType& type);
   void Set(const std::vector<double>& state, const StateVectorType& type);
   void Set(double* state, const StateVectorType& type);
   
   void ConvertTo(const StateVectorType& type, double mu);

   //double& Get(int index);
   //double Get(int index) const;
   //void Set(int index, double value);

private:
   StateVectorType m_type;
   StateVectorData m_state;
   //Vector6d m_state;
   //double m_state[6];
   //std::vector<double> m_state;
   //Vector6d m_state;
   //State m_state;
   //StatePtr m_state;  
};
}

namespace test2
{
class StateVector
{
public:
   StateVector();
   StateVector(const StateVector& other);
   //StateVector(const StateVector&& other);
   explicit StateVector(const OrbitalElements& orbitalElements, double mu);
   explicit StateVector(const CartesianStateVector& cartesianStateVector, double mu);
   StateVector(double* state, double mu, const StateVectorType& type);

   StateVector& operator =(const StateVector& other);
   //StateVector& operator =(const StateVector&& other);

   void Set(const CartesianStateVector& cartesianStateVector, double mu);
   void Set(const OrbitalElements& orbitalElements, double mu);
   void Set(double* state, double mu, const StateVectorType& type);

   void ConvertTo(const StateVectorType& type) const;

   bool IsCartesian() const;
   bool IsOrbital() const;
   bool IsType(const StateVectorType& type) const;
   StateVectorType GetType() const;

   double GetMu() const;

   const CartesianStateVector& GetCartesianStateVector() const;
   const OrbitalElements& GetOrbitalElements() const;

private:
   mutable StateVectorType m_type;
   mutable CartesianStateVector m_cartesian;
   mutable OrbitalElements m_orbital;
   double m_mu;
};
}

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