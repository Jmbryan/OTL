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

#include <OTL/Core/StateVector.h>
#include <OTL/Core/CartesianStateVector.h>
#include <OTL/Core/OrbitalElements.h>
#include <OTL/Core/Conversion.h>

namespace otl
{

StateVector::StateVector() :
m_type(StateVectorType::Invalid),
m_state()
//m_state(6, 0.0)
{
   //m_state = new double[6];
   //for (int i = 0; i < 6; ++i)
   //{
   //   m_state[i] = 0.0;
   //}  
   //m_state.fill(0.0);

   //std::fill(m_state, m_state + 6, 0.0);
}

StateVector::StateVector(const StateVector& other)
{
   *this = other;
}

StateVector::StateVector(const StateVector&& other)
{
   *this = std::move(other);
}

//StateVector::StateVector(const Vector6d& genericStateVector)
//{
//   *this = genericStateVector;
//}

StateVector::StateVector(const OrbitalElements& orbitalElements)
{
   *this = orbitalElements;
}

StateVector::StateVector(const CartesianStateVector& cartesianStateVector)
{
   *this = cartesianStateVector;
}

StateVector::~StateVector()
{
   //if (m_state)
   //{
   //   delete[] m_state;
   //   m_state = nullptr;
   //}
   double d = 1.0;
}

StateVector& StateVector::operator =(const StateVector& other)
{
   if (this != &other)
   {
      m_type = other.GetType();
      //memcpy(m_state, other.GetState(), 6);
      m_state = other.GetState();

      //m_state = other.GetGenericStateVector();    
      //if (!m_state)
      //{
      //   m_state = new double[6];
      //}
      //m_state = other.GetState();
   }
   return *this;
}

StateVector& StateVector::operator =(const StateVector&& other)
{
   if (this != &other)
   {
      m_type = std::move(other.GetType());
      //memcpy(m_state, other.GetState(), 6);
      m_state = std::move(other.GetState());

      //m_state = std::move(other.GetGenericStateVector());
      //m_state = std::move(other.GetState());
      //double* state = other.GetState();
      //m_state = state;
      //state = nullptr;

   }
   return *this;
}

//StateVector& StateVector::operator =(const Vector6d& genericStateVector)
//{
//   m_type = StateVectorType::Generic;
//   m_state = genericStateVector;
//   return *this;
//}

StateVector& StateVector::operator =(const CartesianStateVector& cartesianStateVector)
{
   m_type = StateVectorType::Cartesian;
   //m_state.segment(0, 3) = cartesianStateVector.position;
   //m_state.segment(3, 3) = cartesianStateVector.velocity;
   m_state[0] = cartesianStateVector.position.x();
   m_state[1] = cartesianStateVector.position.y();
   m_state[2] = cartesianStateVector.position.z();
   m_state[3] = cartesianStateVector.velocity.x();
   m_state[4] = cartesianStateVector.velocity.y();
   m_state[5] = cartesianStateVector.velocity.z();
   return *this;
}

StateVector& StateVector::operator =(const OrbitalElements& orbitalElements)
{
   m_type = StateVectorType::Orbital;
   m_state[0] = orbitalElements.semiMajorAxis;
   m_state[1] = orbitalElements.eccentricity;
   m_state[2] = orbitalElements.trueAnomaly;
   m_state[3] = orbitalElements.inclination;
   m_state[4] = orbitalElements.argOfPericenter;
   m_state[5] = orbitalElements.lonOfAscendingNode;
   return *this;
}

StateVectorType StateVector::GetType() const
{
   return m_type;
}

//Vector6d StateVector::GetGenericStateVector() const
//{
//   return m_state;
//}

//const double* StateVector::GetState() const
//{
//   return m_state;
//}

//std::vector<double> StateVector::GetState() const
//{
//   return m_state;
//}

const Vector6d& StateVector::GetState() const
{
   return m_state;
}

CartesianStateVector StateVector::GetCartesianStateVector() const
{
   OTL_ASSERT(m_type == StateVectorType::Cartesian, "Invalid state vector type");
   //return CartesianStateVector(m_state.segment(0, 3), m_state.segment(3, 3));
   return CartesianStateVector(Vector3d(m_state[0], m_state[1], m_state[2]), Vector3d(m_state[3], m_state[4], m_state[5]));
}

OrbitalElements StateVector::GetOrbitalElements() const
{
   OTL_ASSERT(m_type == StateVectorType::Orbital, "Invalid state vector type");
   return OrbitalElements(m_state[0], m_state[1], m_state[2], m_state[3], m_state[4], m_state[5]);
}

CartesianStateVector StateVector::ToCartesianStateVector(double mu) const
{
   switch (m_type)
   {
   case StateVectorType::Cartesian:
      return GetCartesianStateVector();
      break;

   case StateVectorType::Orbital:
      return ConvertOrbitalElements2CartesianStateVector(GetOrbitalElements(), mu);
      break;

   default:
      OTL_ERROR() << "Failed to convert to state vector. Invalid or unknown state vector type";
      return CartesianStateVector();
      break;
   }
}

OrbitalElements StateVector::ToOrbitalElements(double mu) const
{
   switch (m_type)
   {
   case StateVectorType::Orbital:
      return GetOrbitalElements();
      break;

   case StateVectorType::Cartesian:
      return OrbitalElements();
      return ConvertCartesianStateVector2OrbitalElements(GetCartesianStateVector(), mu);
      break;

   default:
      OTL_ERROR() << "Failed to convert to orbital elements. Invalid or unknown state vector type";
      return OrbitalElements();
      break;
   }
}

double StateVector::GetX() const
{
   OTL_ASSERT(m_type == StateVectorType::Cartesian);
   return m_state[0];
}

double StateVector::GetSemimajorAxis() const
{
   OTL_ASSERT(m_type == StateVectorType::Orbital);
   return m_state[0];
}

//void StateVector::Set(const Vector6d& genericStateVector, const StateVectorType& stateVectorType)
//{
//   m_state = genericStateVector;
//   m_type = stateVectorType;
//}

//namespace test
// {
//
//StateVector2::StateVector2(const CartesianStateVector& cartesianStateVector) :
//m_type(StateVectorType::Cartesian),
//m_state(cartesianStateVector)
//{
//
//}
//
//StateVector2::StateVector2(const OrbitalElements& orbitalElements) :
//m_type(StateVectorType::Orbital),
//m_state(orbitalElements)
//{
//
//}
//
//const CartesianStateVector& StateVector2::GetCartesianStateVector() const
//{
//   OTL_ASSERT(m_type == StateVectorType::Cartesian);
//   return boost::get<CartesianStateVector>(m_state);
//}
//
//} // namespace test

} // namespace otl