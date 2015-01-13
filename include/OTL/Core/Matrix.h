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
#include <Eigen/Dense>
#include <Eigen/src/Core/CwiseBinaryOp.h>
//#include <Eigen/src/Plugins/CommonCwiseUnaryOps.h>

namespace otl
{

namespace temp {

template<typename T, int NumRows, int NumCols>
class Matrix
{
public:
   Matrix();
   explicit Matrix(unsigned int numElements);
   Matrix(unsigned int numRows, unsigned int numCols);
   Matrix(const Matrix& other);
   Matrix(const Eigen::Matrix<T, NumRows, NumCols>& matrix);
   Matrix& operator =(const Matrix& other);
   Matrix& operator =(const Eigen::Matrix<T, NumRows, NumCols>&  other);

   T& operator()(unsigned int row, unsigned int col);
   const T& operator()(unsigned int row, unsigned int col) const;

   void Resize(unsigned int numRows, unsigned int numCols);
   void Fill(T fillValue);

   T GetMagnitude() const;
   void Transpose();
   void Normalize();

   T Dot(const Matrix& other) const;
   Matrix Cross(const Matrix& other) const;

   Eigen::Matrix<T, NumRows, NumCols>& GetImpl();
   const Eigen::Matrix<T, NumRows, NumCols>& GetImpl() const;

   static Matrix Zero();
   static Matrix Identity();
   static Matrix Constant(const T& value);

private:
   Eigen::Matrix<T, NumRows, NumCols> m_matrix;
};

// Free functions

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Cross(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Transpose(const Matrix<T, NumRows, NumCols>& other);

// Operator overloads

template<typename T, int NumRows, int NumCols>
Eigen::Matrix<T, NumRows, NumCols> operator +(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Eigen::Matrix<T, NumRows, NumCols> operator -(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Eigen::Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator+=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator-=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator*=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

// Scalar
template<typename S, typename T, int NumRows, int NumCols>
Eigen::Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, NumCols>& left, const S& right);

template<typename S, typename T, int NumRows, int NumCols>
Eigen::CwiseUnaryOp<Eigen::internal::scalar_multiple_op<T>, const Eigen::Matrix<T, NumRows, NumCols>> operator *(const S& left, const Matrix<T, NumRows, NumCols>& right);
//template<typename S, typename T, int NumRows, int NumCols>
//Eigen::Matrix<T, NumRows, NumCols> operator *(const S& left, const Matrix<T, NumRows, NumCols>& right);

template<typename S, typename T, int NumRows, int NumCols>
Eigen::Matrix<T, NumRows, NumCols> operator /(const Matrix<T, NumRows, NumCols>& left, const S& right);

// Boolean
template<typename T, int NumRows, int NumCols>
bool operator==(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);

template<typename T, int NumRows, int NumCols>
bool operator!=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right);


#define OTL_MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix)        \
typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix;     \
typedef Matrix<Type, Size, 1>    Vector##SizeSuffix##TypeSuffix;     \
typedef Matrix<Type, 1, Size>    RowVector##SizeSuffix##TypeSuffix;

#define OTL_MAKE_DYNAMIC_TYPEDEF(Type, TypeSuffix)                            \
typedef Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> Matrix##TypeSuffix;      \
typedef Matrix<Type, Eigen::Dynamic, 1>              Vector##TypeSuffix;      \
typedef Matrix<Type, 1, Eigen::Dynamic>              RowVector##TypeSuffix;

#define OTL_MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
OTL_MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2)             \
OTL_MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3)             \
OTL_MAKE_TYPEDEFS(Type, TypeSuffix, 6, 6)             \
OTL_MAKE_DYNAMIC_TYPEDEF(Type, TypeSuffix)

OTL_MAKE_TYPEDEFS_ALL_SIZES(int,    i)
OTL_MAKE_TYPEDEFS_ALL_SIZES(float,  f)
OTL_MAKE_TYPEDEFS_ALL_SIZES(double, d)

} // namespace temp

} // namespace otl

#include <OTL/Core/Matrix.inl>