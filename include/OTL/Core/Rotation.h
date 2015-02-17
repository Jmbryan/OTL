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

enum class RotationType
{
   Invalid = -1,
   Generic,
   Quaternion,
   Matrix,
   Euler,
   Count
};

namespace test
{

struct AngularStateVector
{
   Quaterniond orientation;
   Quaterniond velocity;
};

struct EulerAngles
{
   Vector3d angles;
   int a1;
   int a2;
   int a3;

   EulerAngles(const Vector3d& angles, int a1, int a2, int a3);
};

class Rotation
{
public:

   Rotation(const Rotation& other);
   Rotation(const Quaterniond& quaternion);
   Rotation(const Matrix3d& rotationMatrix);
   Rotation(const EulerAngles& eulerAngles);

   Rotation& operator =(const Rotation& other);
   Rotation& operator =(const Quaterniond& quaternion);
   Rotation& operator =(const EulerAngles& eulerAngles);
   Rotation& operator =(const Matrix3d& rotationMatrix);

   Quaterniond GetQuaternion() const;
   Matrix3d GetRotationMatrix() const;
   EulerAngles GetEulerAngles() const;
   const Matrix3d& GetRawState() const;

private:
   RotationType m_type;
   Matrix3d m_matrix;
};

} // namespace test

} // namespace otl
