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

/** \brief Constructs an initialized 6D vector with given coefficients */
EIGEN_STRONG_INLINE Matrix(const Scalar& x1, const Scalar& x2, const Scalar& x3, const Scalar& x4, const Scalar& x5, const Scalar& x6)
{
   Base::_check_template_params();
   EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Matrix, 6)
   m_storage.data()[0] = x1;
   m_storage.data()[1] = x2;
   m_storage.data()[2] = x3;
   m_storage.data()[3] = x4;
   m_storage.data()[4] = x5;
   m_storage.data()[5] = x6;
}