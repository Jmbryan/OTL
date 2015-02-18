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
#include <OTL/Core/Logger.h>

namespace otl
{ namespace test2 {

template<typename T>
class Vector3
{
public:
   Vector3() :
   m_x(T()), m_y(T()), m_z(T())
   {

   }

   Vector3(const T& x, const T& y, const T& z) :
   m_x(x), m_y(y), m_z(z)
   {

   }

   Vector3(const Vector3& other) :
   m_x(other.X()), m_y(other.Y()), m_z(other.Z())
   {

   }

   Vector3(const Vector3&& other) :
   m_x(std::move(other.X())), m_y(std::move(other.Y())), m_z(std::move(other.Z()))
   {

   }

   template<typename U>
   Vector3(const Vector3<U>& other) :
   m_x(static_cast<T>(other.X())), m_y(static_cast<T>(other.Y())), m_z(static_cast<T>(other.Z()))
   {

   }

   template<typename U>
   Vector3(const Vector3<U>&& other) :
   m_x(std::move(static_cast<T>(other.X()))), m_y(static_cast<T>(std::move(other.Y()))), m_z(static_cast<T>(std::move(other.Z())))
   {

   }

   ~Vector3()
   {

   }

   Vector3& operator =(const Vector3& other)
   {
      if (this != &other)
      {
         m_x = other.X();
         m_y = other.Y();
         m_z = other.Z();
      }
      return *this;
   }

   Vector3& operator =(const Vector3&& other)
   {
      if (this != &other)
      {
         m_x = std::move(other.X());
         m_y = std::move(other.Y());
         m_z = std::move(other.Z());
      }
      return *this;
   }

   T x() const
   {
      return m_x;
   }

   T& x()
   {
      return m_x;
   }

   T y() const
   {
      return m_y;
   }

   T& y()
   {
      return m_y;
   }

   T z() const
   {
      return m_z;
   }

   T& z()
   {
      return m_z;
   }

   T X() const
   {
      return m_x;
   }

   T& X()
   {
      return m_x;
   }

   T Y() const
   {
      return m_y;
   }

   T& Y()
   {
      return m_y;
   }

   T Z() const
   {
      return m_z;
   }

   T& Z()
   {
      return m_z;
   }

   T operator[](int index) const
   {
      OTL_ASSERT(index >= 0 && index < 3, "Index must be between 0, 1, or 2");
      return (index == 0 ? m_x : index == 1 ? m_y : m_z);
   }

   T& operator[](int index)
   {
      OTL_ASSERT(index >= 0 && index < 3, "Index must be between 0, 1, or 2");
      return (index == 0 ? m_x : index == 1 ? m_y : m_z);
   }

   T squaredNorm() const
   {
      return SQR(m_x) + SQR(m_y) + SQR(m_z);
   }

   T norm() const
   {
      return sqrt(squaredNorm());
   }

   void normalize()
   {
      T n = norm();
      m_x /= n;
      m_y /= n;
      m_z /= n;
   }

   Vector3 normalized() const
   {
      T n = norm();
      return Vector3(
         m_x / n,
         m_y / n,
         m_z / n);
   }

   Vector3& normalizeInPlace()
   {
      normalize();
      return *this;
   }

   T dot(const Vector3& other) const
   {
      return (m_x * other.X() + 
              m_y * other.Y() +
              m_z * other.Z());
   }

   Vector3 cross(const Vector3& other) const
   {
      return Vector3(
         m_y * other.Z() - m_z * other.Y(),
         m_z * other.X() - m_x * other.Z(),
         m_x * other.Y() - m_y * other.X());
   }

   bool isApprox(const Vector3& other, const T& tolerance) const
   {
      return (
         IsApprox(m_x, other.X(), tolerance) &&
         IsApprox(m_y, other.Y(), tolerance) &&
         IsApprox(m_z, other.Z(), tolerance));
   }

   bool isZero() const
   {
      return (
         IsApprox(m_x, 0.0) &&
         IsApprox(m_y, 0.0) &&
         IsApprox(m_z, 0.0));
   }

   void Zero()
   {
      m_x = m_y = m_z = static_cast<T>(0);
   }

   static Vector3 Zeros()
   {
      return Vector3(0, 0, 0);
   }

   static Vector3 Ones()
   {
      return Vector3(1, 1, 1);
   }

   static Vector3 UnitX()
   {
      return Vector3(1, 0, 0);
   }

   static Vector3 UnitY()
   {
      return Vector3(0, 1, 0);
   }

   static Vector3 UnitZ()
   {
      return Vector3(0, 0, 1);
   }

private:
   T m_x;
   T m_y;
   T m_z;
};

template<typename T>
Vector3<T> operator -(const Vector3<T>& left)
{
   return Vector3<T>(-left.X(), -left.Y(), -left.Z());
}

template<typename T>
Vector3<T> operator+=(Vector3<T>& left, const Vector3<T>& right)
{
   left.X() += right.X();
   left.Y() += right.Y();
   left.Z() += right.Z();
   return left;
}

template<typename T>
Vector3<T> operator-=(Vector3<T>& left, const Vector3<T>& right)
{
   left.X() -= right.X();
   left.Y() -= right.Y();
   left.Z() -= right.Z();
   return left;
}

template<typename T>
Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right)
{
   return Vector3<T>(
      left.X() + right.X(),
      left.Y() + right.Y(),
      left.Z() + right.Z());
}

template<typename T>
Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right)
{
   return Vector3<T>(
      left.X() - right.X(),
      left.Y() - right.Y(),
      left.Z() - right.Z());
}

template<typename T>
Vector3<T> operator *(const Vector3<T>& left, const T& scalar)
{
   return Vector3<T>(
      left.X() * scalar,
      left.Y() * scalar,
      left.Z() * scalar);
}

template<typename T>
Vector3<T> operator *(const T& scalar, const Vector3<T>& left)
{
   return Vector3<T>(
      scalar * left.X(),
      scalar * left.Y(),
      scalar * left.Z());
}

template<typename T>
Vector3<T>& operator*=(Vector3<T>& left, const T& scalar)
{
   left.X() *= scalar;
   left.Y() *= scalar;
   left.Z() *= scalar;
   return left;
}

template<typename T>
Vector3<T> operator /(const Vector3<T>& left, const T& scalar)
{
   return Vector3<T>(
      left.X() / scalar,
      left.Y() / scalar,
      left.Z() / scalar);
}

template<typename T>
Vector3<T>& operator/=(Vector3<T>& left, const T& scalar)
{
   left.X() /= scalar;
   left.Y() /= scalar;
   left.Z() /= scalar;
   return left;
}

template<typename T>
bool operator==(const Vector3<T>& left, const Vector3<T>& right)
{
   return (
      IsApprox(left.X(), right.X()) &&
      IsApprox(left.Y(), right.Y()) &&
      IsApprox(left.Z(), right.Z()));
}

template<typename T>
bool operator!=(const Vector3<T>& left, const Vector3<T>& right)
{
   return (
      !IsApprox(left.X(), right.X()) ||
      !IsApprox(left.Y(), right.Y()) ||
      !IsApprox(left.Z(), right.Z()));
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

template<typename T>
class Vector6
{
public:
   Vector6() :
      m_x(T()), m_y(T()), m_z(T()), m_vx(T()), m_vy(T()), m_vz(T())
   {

   }

   Vector6(const T& x, const T& y, const T& z, const T& vx, const T& vy, const T& vz) :
      m_x(x), m_y(y), m_z(z), m_vx(vx), m_vy(vy), m_vz(vz)
   {

   }

   Vector6(const Vector6& other) :
      m_x(other.X()), m_y(other.Y()), m_z(other.Z()), m_vx(other.VX()), m_vy(other.VY()), m_vz(other.VZ())
   {

   }

   Vector6(const Vector6&& other) :
      m_x(std::move(other.X())), m_y(std::move(other.Y())), m_z(std::move(other.Z())), m_vx(std::move(other.VX())), m_vy(std::move(other.VY())), m_vz(std::move(other.VZ()))
   {

   }

   template<typename U>
   Vector6(const Vector6<U>& other) :
      m_x(static_cast<T>(other.X())), m_y(static_cast<T>(other.Y())), m_z(static_cast<T>(other.Z())), m_vx(static_cast<T>(other.VX())), m_vy(static_cast<T>(other.VY())), m_vz(static_cast<T>(other.VZ()))
   {

   }

   template<typename U>
   Vector6(const Vector6<U>&& other) :
      m_x(std::move(static_cast<T>(other.X()))), m_y(static_cast<T>(std::move(other.Y()))), m_z(static_cast<T>(std::move(other.Z()))), m_vx(std::move(static_cast<T>(other.VX()))), m_vy(static_cast<T>(std::move(other.VY()))), m_vz(static_cast<T>(std::move(other.VZ())))
   {

   }

   ~Vector6()
   {

   }

   Vector6& operator =(const Vector6& other)
   {
      if (this != &other)
      {
         m_x = other.X();
         m_y = other.Y();
         m_z = other.Z();
         m_vx = other.VX();
         m_vy = other.VY();
         m_vz = other.VZ();
      }
      return *this;
   }

   Vector6& operator =(const Vector6&& other)
   {
      if (this != &other)
      {
         m_x = std::move(other.X());
         m_y = std::move(other.Y());
         m_z = std::move(other.Z());
         m_vx = std::move(other.VX());
         m_vy = std::move(other.VY());
         m_vz = std::move(other.VZ());
      }
      return *this;
   }

   T x() const
   {
      return m_x;
   }

   T& x()
   {
      return m_x;
   }

   T y() const
   {
      return m_y;
   }

   T& y()
   {
      return m_y;
   }

   T z() const
   {
      return m_z;
   }

   T& z()
   {
      return m_z;
   }

   T vx() const
   {
      return m_vx;
   }
   
   T& vx()
   {
      return m_vx;
   }

   T vy() const
   {
      return m_vy;
   }

   T& vy()
   {
      return m_vy;
   }

   T vz() const
   {
      return m_vz;
   }

   T& vz()
   {
      return m_vz;
   }

   T X() const
   {
      return m_x;
   }

   T& X()
   {
      return m_x;
   }

   T Y() const
   {
      return m_y;
   }

   T& Y()
   {
      return m_y;
   }

   T Z() const
   {
      return m_z;
   }

   T& Z()
   {
      return m_z;
   }

   T VX() const
   {
      return m_vx;
   }

   T& VX()
   {
      return m_vx;
   }

   T VY() const
   {
      return m_vy;
   }

   T& VY()
   {
      return m_vy;
   }

   T VZ() const
   {
      return m_vz;
   }

   T& VZ()
   {
      return m_vz;
   }

   T operator[](int index) const
   {
      OTL_ASSERT(index >= 0 && index < 6, "Index must be between 0..6");
      return (index == 0 ? m_x : index == 1 ? m_y : index == 2 ? m_z : index == 3 ? m_vx : index == 4 ? m_vy : m_vz);
   }

   T& operator[](int index)
   {
      OTL_ASSERT(index >= 0 && index < 6, "Index must be between 0..6");
      return (index == 0 ? m_x : index == 1 ? m_y : index == 2 ? m_z : index == 3 ? m_vx : index == 4 ? m_vy : m_vz);
   }

   T squaredNorm() const
   {
      return SQR(m_x) + SQR(m_y) + SQR(m_z) + SQR(m_vx) + SQR(m_vy) + SQR(m_vz);
   }

   T norm() const
   {
      return sqrt(squaredNorm());
   }

   Vector6 normalized() const
   {
      T norm = Norm();
      return Vector6(
         m_x / norm,
         m_y / norm,
         m_z / norm,
         m_vx / norm,
         m_vy / norm,
         m_vz / norm);
   }

   Vector6& normalizeInPlace()
   {
      T norm = Norm();
      m_x /= norm;
      m_y /= norm;
      m_z /= norm;
      m_vx /= norm;
      m_vy /= norm;
      m_vz /= norm;
   }

   T dot(const Vector6& other)
   {
      return (
         m_x * other.X() +
         m_y * other.Y() +
         m_z * other.Z() +
         m_vx * other.VX() +
         m_vy * other.VY() +
         m_vz * other.VZ());
   }

   static Vector6 Zeros()
   {
      return Vector6(0, 0, 0, 0, 0, 0, 0);
   }

   static Vector6 Ones()
   {
      return Vector6(1, 1, 1, 1, 1, 1);
   }

private:
   T m_x;
   T m_y;
   T m_z;
   T m_vx;
   T m_vy;
   T m_vz;
};

template<typename T>
Vector6<T> operator -(const Vector6<T>& left)
{
   return Vector6<T>(-left.X(), -left.Y(), -left.Z(), -left.VX(), -left.VY(), -left.VZ());
}

template<typename T>
Vector6<T> operator+=(Vector6<T>& left, const Vector6<T>& right)
{
   left.X() += right.X();
   left.Y() += right.Y();
   left.Z() += right.Z();
   left.VX() += right.VX();
   left.VY() += right.VY();
   left.VZ() += right.VZ();
   return left;
}

template<typename T>
Vector6<T> operator-=(Vector6<T>& left, const Vector6<T>& right)
{
   left.X() -= right.X();
   left.Y() -= right.Y();
   left.Z() -= right.Z();
   left.VX() -= right.VX();
   left.VY() -= right.VY();
   left.VZ() -= right.VZ();
   return left;
}

template<typename T>
Vector6<T> operator +(const Vector6<T>& left, const Vector6<T>& right)
{
   return Vector6<T>(
      left.X() + right.X(),
      left.Y() + right.Y(),
      left.Z() + right.Z(),
      left.VX() + right.VX(),
      left.VY() + right.VY(),
      left.VZ() + right.VZ());
}

template<typename T>
Vector6<T> operator -(const Vector6<T>& left, const Vector6<T>& right)
{
   return Vector6<T>(
      left.X() - right.X(),
      left.Y() - right.Y(),
      left.Z() - right.Z(),
      left.VX() - right.VX(),
      left.VY() - right.VY(),
      left.VZ() - right.VZ());
}

template<typename T>
Vector6<T> operator *(const Vector6<T>& left, const T& scalar)
{
   return Vector6<T>(
      left.X() * scalar,
      left.Y() * scalar,
      left.Z() * scalar,
      left.VX() * scalar,
      left.VY() * scalar,
      left.VZ() * scalar);
}

template<typename T>
Vector6<T> operator *(const T& scalar, const Vector6<T>& left)
{
   return Vector6<T>(
      scalar * left.X(),
      scalar * left.Y(),
      scalar * left.Z(),
      scalar * left.VX(),
      scalar * left.VY(),
      scalar * left.VZ());
}

template<typename T>
Vector6<T>& operator*=(Vector6<T>& left, const T& scalar)
{
   left.X() *= scalar;
   left.Y() *= scalar;
   left.Z() *= scalar;
   left.VX() *= scalar;
   left.VY() *= scalar;
   left.VZ() *= scalar;
   return left;
}

template<typename T>
Vector6<T> operator /(const Vector6<T>& left, const T& scalar)
{
   return Vector6<T>(
      left.X() / scalar,
      left.Y() / scalar,
      left.Z() / scalar,
      left.VX() / scalar,
      left.VY() / scalar,
      left.VZ() / scalar);
}

template<typename T>
Vector6<T>& operator/=(Vector6<T>& left, const T& scalar)
{
   left.X() /= scalar;
   left.Y() /= scalar;
   left.Z() /= scalar;
   left.VX() /= scalar;
   left.VY() /= scalar;
   left.VZ() /= scalar;
   return left;
}

template<typename T>
bool operator==(const Vector6<T>& left, const Vector6<T>& right)
{
   return (
      IsApprox(left.X(), right.X()) &&
      IsApprox(left.Y(), right.Y()) &&
      IsApprox(left.Z(), right.Z()) &&
      IsApprox(left.VX(), right.VX()) &&
      IsApprox(left.VY(), right.VY()) &&
      IsApprox(left.VZ(), right.VZ()));
}

template<typename T>
bool operator!=(const Vector6<T>& left, const Vector6<T>& right)
{
   return (
      !IsApprox(left.X(), right.X()) ||
      !IsApprox(left.Y(), right.Y()) ||
      !IsApprox(left.Z(), right.Z()) ||
      !IsApprox(left.VX(), right.VX()) ||
      !IsApprox(left.VY(), right.VY()) ||
      !IsApprox(left.VZ(), right.VZ()));
}

} // namespace test
} // namespace otl