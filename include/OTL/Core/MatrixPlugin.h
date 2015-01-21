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

inline Scalar& x()
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](0);
}

inline const Scalar& x() const
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](0);
}

inline Scalar& y()
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](1);
}

inline const Scalar& y() const
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](1);
}

inline Scalar& z()
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](2);
}

inline const Scalar& z() const
{
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(MatrixBase, 3)
   return this->operator[](2);
}