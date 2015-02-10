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

#include <OTL/Core/Rotation.h>
#include <OTL/Core/Logger.h>

namespace otl
{

namespace test
{

EulerAngles::EulerAngles(const Vector3d& angles, int a1, int a2, int a3) :
angles(angles),
a1(a1),
a2(a2),
a3(a3)
{
   OTL_ASSERT(a1 >= 1 && a1 <= 3, "Euler axis must be 1, 2, or 3");
   OTL_ASSERT(a2 >= 1 && a2 <= 3, "Euler axis must be 1, 2, or 3");
   OTL_ASSERT(a3 >= 1 && a3 <= 3, "Euler axis must be 1, 2, or 3");
}


Rotation::Rotation(const Rotation& other)
{
   *this = other;
}

Rotation::Rotation(const Quaterniond& quaternion)
{
   *this = quaternion;
}

Rotation::Rotation(const Matrix3d& rotationMatrix)
{
   *this = rotationMatrix;
}

Rotation::Rotation(const EulerAngles& eulerAngles)
{
   *this = eulerAngles;
}

Rotation& Rotation::operator =(const Rotation& other)
{
   if (this != &other)
   {
      m_matrix = other.GetRawState();
   }
   return *this;
}

Rotation& Rotation::operator =(const Quaterniond& quaternion)
{
   m_matrix = quaternion.toRotationMatrix();
   return *this;
}

Rotation& Rotation::operator =(const EulerAngles& eulerAngles)
{
   m_matrix = Eigen::AngleAxisd(eulerAngles.angles.x(), eulerAngles.a1 == 1 ? Vector3d::UnitX() : eulerAngles.a1 == 2 ? Vector3d::UnitY() : Vector3d::UnitZ()) *
      Eigen::AngleAxisd(eulerAngles.angles.y(), eulerAngles.a2 == 1 ? Vector3d::UnitX() : eulerAngles.a2 == 2 ? Vector3d::UnitY() : Vector3d::UnitZ()) *
      Eigen::AngleAxisd(eulerAngles.angles.z(), eulerAngles.a3 == 1 ? Vector3d::UnitX() : eulerAngles.a3 == 2 ? Vector3d::UnitY() : Vector3d::UnitZ());
   return *this;
}

Rotation& Rotation::operator =(const Matrix3d& rotationMatrix)
{
   m_matrix = rotationMatrix;
   return *this;
}

Quaterniond Rotation::GetQuaternion() const
{
   OTL_ASSERT(m_type == RotationType::Quaternion, "Invalid rotation type");
   return Quaterniond(m_matrix);
}

Matrix3d Rotation::GetRotationMatrix() const
{
   OTL_ASSERT(m_type == RotationType::Matrix, "Invalid rotation type");
   return m_matrix;
}

EulerAngles Rotation::GetEulerAngles() const
{
   OTL_ASSERT(m_type == RotationType::Euler, "Invalid rotation type");
   return EulerAngles(m_matrix.eulerAngles(1, 2, 3), 1, 2, 3);
}

const Matrix3d& Rotation::GetRawState() const
{
   return m_matrix;
}

} // namespace test

} // namespace otl