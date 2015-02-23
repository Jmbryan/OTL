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

//namespace test {

StateVector::StateVector() :
m_type(StateVectorType::Invalid)//,
//m_cartesianStateVector(),
//m_orbitalElements()
//m_state()
//m_state(6, 0.0)
{
   //m_state = new double[6];
   //for (int i = 0; i < 6; ++i)
   //{
   //   m_state[i] = 0.0;
   //}  
   //m_state.fill(0.0);

   //std::fill(m_state, m_state + 6, 0.0);
   std::fill(m_state.begin(), m_state.end(), 0.0);
}

StateVector::StateVector(const StateVector& other)// :
//m_type(other.GetType())
//m_cartesianStateVector(other.GetCartesianStateVector()),
//m_orbitalElements(other.GetOrbitalElements())
//m_state(other.GetState())
{
   *this = other;
}

StateVector::StateVector(const StateVector&& other)// :
//m_type(std::move(other.GetType()))
//m_state(std::move(other.GetState()))
{
   *this = other;
}

StateVector::StateVector(double x1, double x2, double x3, double x4, double x5, double x6, const StateVectorType& type)// :
//m_type(type)
//m_state(x1, x2, x3, x4, x5, x6)
{
   Set(x1, x2, x3, x4, x5, x6, type);
}

StateVector::StateVector(const Vector3d& state1, const Vector3d& state2, const StateVectorType& type)// :
//m_type(type)
//m_state(state1, state2)
{
   Set(state1, state2, type);
}

StateVector::StateVector(const Vector6d& state, const StateVectorType& type)// :
//m_type(type)
//m_state(state)
{
   Set(state, type);
}

StateVector::StateVector(const std::vector<double>& state, const StateVectorType& type)// :
//m_type(type)
//m_state(state.data())
{
   Set(state, type);
}

StateVector::StateVector(double* state, const StateVectorType& type)// :
//m_type(type)
//m_state(state)
{
   Set(state, type);
}

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
      //memcpy(m_state, other.GetState(), 6 * sizeof(double));
      std::copy(other.GetState().begin(), other.GetState().end(), m_state.begin());
      //m_state = other.GetState();

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
      //memcpy(m_state, other.GetState(), 6 * sizeof(double));
      m_state = std::move(other.GetState());

      //m_state = std::move(other.GetGenericStateVector());
      //m_state = std::move(other.GetState());
      //double* state = other.GetState();
      //m_state = state;
      //state = nullptr;

   }
   return *this;
}

//StateVector& StateVector::operator =(const Vector6d& state)
//{
//   m_type = StateVectorType::Generic;
//   m_state = genericStateVector;
//   return *this;
//}

StateVector& StateVector::operator =(const CartesianStateVector& cartesianStateVector)
{
   m_type = StateVectorType::Cartesian;
   //memcpy(m_state, cartesianStateVector.position.data(), 3 * sizeof(double));
   //memcpy(m_state + 3, cartesianStateVector.velocity.data(), 3 * sizeof(double));
   std::copy(cartesianStateVector.position.data(), cartesianStateVector.position.data() + 3, m_state.begin());
   std::copy(cartesianStateVector.velocity.data(), cartesianStateVector.velocity.data() + 3, m_state.begin() + 3);
   //m_state.segment(0, 3) = cartesianStateVector.position;
   //m_state.segment(3, 3) = cartesianStateVector.velocity;
   
   //m_state[0] = cartesianStateVector.position.x();
   //m_state[1] = cartesianStateVector.position.y();
   //m_state[2] = cartesianStateVector.position.z();
   //m_state[3] = cartesianStateVector.velocity.x();
   //m_state[4] = cartesianStateVector.velocity.y();
   //m_state[5] = cartesianStateVector.velocity.z();
   //m_state = cartesianStateVector;
   return *this;
}

StateVector& StateVector::operator =(const OrbitalElements& orbitalElements)
{
   m_type = StateVectorType::Orbital;
   //if (!m_state)
   //   m_state = std::make_shared<Vector6d>();
      //orbitalElements.semiMajorAxis,
      //orbitalElements.eccentricity,
      //orbitalElements.trueAnomaly,
      //orbitalElements.inclination,
      //orbitalElements.argOfPericenter,
      //orbitalElements.lonOfAscendingNode);
   //memcpy(m_state, &orbitalElements, 6 * sizeof(double));
   //std::copy(&orbitalElements, &orbitalElements + 6, m_state.begin());
   m_state[0] = orbitalElements.semiMajorAxis;
   m_state[1] = orbitalElements.eccentricity;
   m_state[2] = orbitalElements.trueAnomaly;
   m_state[3] = orbitalElements.inclination;
   m_state[4] = orbitalElements.argOfPericenter;
   m_state[5] = orbitalElements.lonOfAscendingNode;
   //m_state = orbitalElements;
   return *this;
}

bool StateVector::IsCartesian() const
{
   return m_type == StateVectorType::Cartesian;
}

bool StateVector::IsOrbital() const
{
   return m_type == StateVectorType::Orbital;
}

bool StateVector::IsType(const StateVectorType& type) const
{
   return m_type == type;
}

StateVectorType StateVector::GetType() const
{
   return m_type;
}

const StateVector::StateVectorData& StateVector::GetState() const
{
   return m_state;
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

//const Vector6d& StateVector::GetState() const
//{
//   return m_state;
//}

//const StateVector::State& StateVector::GetState() const
//{
//   return m_state;
//}

//const StateVector::StatePtr& StateVector::GetState() const
//{
//   return m_state;
//}

CartesianStateVector StateVector::GetCartesianStateVector() const
{
   OTL_ASSERT(m_type == StateVectorType::Cartesian, "Invalid state vector type");
   return CartesianStateVector(m_state[0], m_state[1], m_state[2], m_state[3], m_state[4], m_state[5]);
   //return CartesianStateVector(m_state.segment(0, 3), m_state.segment(3, 3));
   //return CartesianStateVector(Vector3d(m_state[0], m_state[1], m_state[2]), Vector3d(m_state[3], m_state[4], m_state[5]));
   //return boost::get<CartesianStateVector>(m_state);
}

OrbitalElements StateVector::GetOrbitalElements() const
{
   OTL_ASSERT(m_type == StateVectorType::Orbital, "Invalid state vector type");
   return OrbitalElements(m_state[0], m_state[1], m_state[2], m_state[3], m_state[4], m_state[5]);
   //return boost::get<OrbitalElements>(m_state);
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

//double StateVector::GetX() const
//{
//   OTL_ASSERT(m_type == StateVectorType::Cartesian);
//   return m_state[0];
//}
//
//double StateVector::GetSemimajorAxis() const
//{
//   OTL_ASSERT(m_type == StateVectorType::Orbital);
//   return m_state[0];
//}

void StateVector::Set(double x1, double x2, double x3, double x4, double x5, double x6, const StateVectorType& type)
{
   //m_state << x1, x2, x3, x4, x5, x6;
   m_type = type;
   m_state[0] = x1;
   m_state[1] = x2;
   m_state[2] = x3;
   m_state[3] = x4;
   m_state[4] = x5;
   m_state[5] = x6;
}

void StateVector::Set(const Vector3d& a, const Vector3d& b, const StateVectorType& type)
{
   m_type = type;
   //memcpy(m_state, a.data(), 3 * sizeof(double));
   //memcpy(m_state + 3, b.data(), 3 * sizeof(double));
   std::copy(a.data(), a.data() + 3, m_state.begin());
   std::copy(b.data(), b.data() + 3, m_state.begin() + 3);
}

void StateVector::Set(const Vector6d& state, const StateVectorType& type)
{
   //m_state = state;
   m_type = type;
   //memcpy(m_state, state.data(), 6 * sizeof(double));
   std::copy(state.data(), state.data() + 6, m_state.begin());
}

void StateVector::Set(const std::vector<double>& state, const StateVectorType& type)
{
   m_type = type;
   //memcpy(m_state, state.data(), 6 * sizeof(double));
   std::copy(state.data(), state.data() + 6, m_state.begin());
}

void StateVector::Set(double* state, const StateVectorType& type)
{
   //OTL_ASSERT(size == 6, "State vector must be 6 dimensional");
   //m_state << state[0], state[1], state[2], state[3], state[4], state[5], state[6];
   //for (int i = 0; i < size; ++i)
   //{ 
   //   m_state[i] = state[i];
   //}
   m_type = type;
   //memcpy(m_state, state, 6 * sizeof(double));
   std::copy(state, state + 6, m_state.begin());
}

void StateVector::ConvertTo(const StateVectorType& type, double mu)
{
   OTL_ASSERT(type == StateVectorType::Cartesian || m_type == StateVectorType::Orbital, "Invalid state vector type");

   if (m_type != type)
   {
      if (type == StateVectorType::Cartesian)
      {
         *this = ToCartesianStateVector(mu);
      }
      else if (type == StateVectorType::Orbital)
      {
         *this = ToOrbitalElements(mu);
      }
   }
}

//}

namespace test
{

StateVector::StateVector() :
m_type(StateVectorType::Invalid),
m_mu(1.0)
{

}

StateVector::StateVector(const StateVector& other)
{
   *this = other;
}

StateVector::StateVector(const OrbitalElements& orbitalElements, double mu)
{
   Set(orbitalElements, mu);
}

StateVector::StateVector(const CartesianStateVector& cartesianStateVector, double mu)
{
   Set(cartesianStateVector, mu);
}

StateVector::StateVector(double* state, double mu, const StateVectorType& type)
{
   Set(state, mu, type);
}

StateVector& StateVector::operator =(const StateVector& other)
{
   if (this != &other)
   {
      m_type = other.GetType();
      m_mu = other.GetMu();
      if (m_type == StateVectorType::Cartesian || m_type == StateVectorType::CartesianAndOrbital)
      {
         memcpy(m_cartesian.position.data(), other.GetCartesianStateVector().position.data(), 3 * sizeof(double));
         memcpy(m_cartesian.velocity.data(), other.GetCartesianStateVector().velocity.data(), 3 * sizeof(double));
      }
      if (m_type == StateVectorType::Orbital || m_type == StateVectorType::CartesianAndOrbital)
      {
         memcpy(&m_orbital, &other.GetOrbitalElements(), 6 * sizeof(double));
      }
   }
   return *this;
}

void StateVector::Set(const CartesianStateVector& cartesianStateVector, double mu)
{
   m_type = StateVectorType::Cartesian;
   m_mu = mu;
   memcpy(m_cartesian.position.data(), cartesianStateVector.position.data(), 3 * sizeof(double));
   memcpy(m_cartesian.velocity.data(), cartesianStateVector.velocity.data(), 3 * sizeof(double));
}

void StateVector::Set(const OrbitalElements& orbitalElements, double mu)
{
   m_type = StateVectorType::Orbital;
   m_mu = mu;
   memcpy(&m_orbital, &orbitalElements, 6 * sizeof(double));
}

void StateVector::Set(double* state, double mu, const StateVectorType& type)
{
   m_type = type;
   m_mu = mu;
   if (m_type == StateVectorType::Cartesian)
   {
      memcpy(m_cartesian.position.data(), state, 3 * sizeof(double));
      memcpy(m_cartesian.velocity.data(), state, 3 * sizeof(double));
   }
   else if (m_type == StateVectorType::Orbital)
   {
      memcpy(&m_orbital, state, 6 * sizeof(double));
   }
}

void StateVector::ConvertTo(const StateVectorType& type) const
{  
   if (m_type != type && m_type == StateVectorType::Cartesian || m_type == StateVectorType::Orbital)
   {
      OTL_ASSERT(m_type == StateVectorType::Cartesian || m_type == StateVectorType::Orbital, "Invalid state vector type");
      OTL_ASSERT(type == StateVectorType::Cartesian || m_type == StateVectorType::Orbital, "Invalid state vector type");

      m_type = StateVectorType::CartesianAndOrbital;
      if (type == StateVectorType::Cartesian)
      {        
         m_cartesian = ConvertOrbitalElements2CartesianStateVector(m_orbital, m_mu);
      }
      else if (type == StateVectorType::Orbital)
      {
         m_orbital = ConvertCartesianStateVector2OrbitalElements(m_cartesian, m_mu);
      }
   }
}

bool StateVector::IsCartesian() const
{
   return m_type == StateVectorType::Cartesian;
}

bool StateVector::IsOrbital() const
{
   return m_type == StateVectorType::Orbital;
}

bool StateVector::IsType(const StateVectorType& type) const
{
   return m_type == type;
}

StateVectorType StateVector::GetType() const
{
   return m_type;
}

double StateVector::GetMu() const
{
   return m_mu;
}

const CartesianStateVector& StateVector::GetCartesianStateVector() const
{
   if (m_type == StateVectorType::Cartesian || m_type == StateVectorType::CartesianAndOrbital)
   {
      return m_cartesian;
   }
   else if (m_type == StateVectorType::Orbital)
   {
      m_type = StateVectorType::CartesianAndOrbital;
      m_cartesian = ConvertOrbitalElements2CartesianStateVector(m_orbital, m_mu);
      OTL_WARN() << "Implict covnersion from OrbitalElements to CartesianStateVector";
   }
   else
   {
      OTL_ERROR() << "Invalid state vector type";
   }
   return m_cartesian;
}

const OrbitalElements& StateVector::GetOrbitalElements() const
{
   if (m_type == StateVectorType::Orbital || m_type == StateVectorType::CartesianAndOrbital)
   {
      return m_orbital;
   }
   else if (m_type == StateVectorType::Cartesian)
   {
      m_type = StateVectorType::CartesianAndOrbital;
      m_orbital = ConvertCartesianStateVector2OrbitalElements(m_cartesian, m_mu);
      OTL_WARN() << "Implict covnersion from CartesianStateVector to OrbitalElements";
   }
   else
   {
      OTL_ERROR() << "Invalid state vector type";
   }
   return m_orbital;
}

} // namespace test

} // namespace otl