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
#include <Eigen/src/Core/MatrixBase.h>

namespace otl
{

namespace temp {

template<typename T, int NumRows, int NumCols>
class Matrix
{
public:
   // Aliases
                                                            using MatrixImpl = Eigen::Matrix<T, NumRows, NumCols>;
                                                            using TransposeReturnType = Eigen::Transpose<MatrixImpl>;
   template<typename DerivedType>                           using MatrixBase = Eigen::MatrixBase<DerivedType>;
                                                            using CrossProductReturnType = typename MatrixBase<MatrixImpl>::template cross_product_return_type<MatrixImpl>::type;
   template<typename OtherType>                             using DotProductReturnType = typename Eigen::internal::scalar_product_traits<typename Eigen::internal::traits<MatrixImpl>::Scalar, typename Eigen::internal::traits<OtherType>::Scalar>::ReturnType;
   template<typename T, typename LhsType>                   using UnaryOpScalarProduct = Eigen::CwiseUnaryOp<Eigen::internal::scalar_multiple_op<T>, const LhsType>;
   template<typename T, typename LhsType>                   using UnaryOpScalarQuotient = Eigen::CwiseUnaryOp<Eigen::internal::scalar_quotient1_op<T>, const LhsType>;
   template<typename T, typename LhsType, typename RhsType> using BinaryOpSum = Eigen::CwiseBinaryOp<Eigen::internal::scalar_sum_op<T>, const LhsType, const RhsType>;
   template<typename T, typename LhsType, typename RhsType> using BinaryOpDifference = Eigen::CwiseBinaryOp<Eigen::internal::scalar_difference_op<T>, const LhsType, const RhsType>;
   template<typename T, typename LhsType, typename RhsType> using ProductReturnType = typename Eigen::ProductReturnType<LhsType, RhsType>::Type;
   
   using RowReturnValue = typename Eigen::MatrixBase<MatrixImpl>::RowXpr;
   using ColumnReturnValue = typename Eigen::MatrixBase<MatrixImpl>::ColXpr;

   ////////////////////////////////////////////////////////////
   // Creation
   ////////////////////////////////////////////////////////////

   Matrix();
   explicit Matrix(unsigned int numElements);
   Matrix(unsigned int numRows, unsigned int numCols);
   Matrix(const Matrix& other);
   Matrix(const MatrixImpl& matrix);
   Matrix& operator =(const Matrix& other);
   Matrix& operator =(const MatrixImpl&  other);
   ~Matrix();

   ////////////////////////////////////////////////////////////
   // Access
   ////////////////////////////////////////////////////////////

   MatrixImpl& operator()();
   const MatrixImpl& operator()() const;

   T& operator()(unsigned int index);
   const T& operator()(unsigned int index) const;

   T& operator()(unsigned int row, unsigned int col);
   const T& operator()(unsigned int row, unsigned int col) const;

   T& operator[](unsigned int index);
   const T& operator[](unsigned int index) const;

   RowReturnValue Row(unsigned int row);
   RowReturnValue Row(unsigned int row) const;
   ColumnReturnValue Col(unsigned int col);
   ColumnReturnValue Col(unsigned int col) const;

   ////////////////////////////////////////////////////////////
   // Utility
   ////////////////////////////////////////////////////////////

   void Resize(unsigned int numRows, unsigned int numCols);
   template<typename ScalarType>
   void Fill(const ScalarType& value);

   T GetNorm() const;
   T GetSquaredNorm() const;
   void NormalizeInPlace();
   void TransposeInPlace();
   TransposeReturnType Transpose();

   DotProductReturnType<MatrixImpl> Dot(const Matrix& other) const;
   template<typename OtherType>
   DotProductReturnType<OtherType> Dot(const MatrixBase<OtherType>& other) const;

   CrossProductReturnType Cross(const Matrix& other) const;
   template<typename OtherType>
   CrossProductReturnType Cross(const MatrixBase<OtherType>& other) const;

   ////////////////////////////////////////////////////////////
   // Static methods
   ////////////////////////////////////////////////////////////

   static Matrix Zero();
   static Matrix Identity();
   template<typename ScalarType>
   static Matrix Constant(const ScalarType& value);

   ////////////////////////////////////////////////////////////
   // Operator overloads
   ////////////////////////////////////////////////////////////

   friend const BinaryOpSum<T, MatrixImpl, MatrixImpl> operator +(const Matrix& left, const Matrix& right) { return left.m_matrix + right.m_matrix; }
   template<typename OtherType>
   friend const BinaryOpSum<T, MatrixImpl, OtherType> operator +(const Matrix& left, const MatrixBase<OtherType>& right) { return left.m_matrix + right; }
   template<typename OtherType>
   friend const BinaryOpSum<T, OtherType, MatrixImpl> operator +(const MatrixBase<OtherType>& left, const Matrix& right) { return left + right.m_matrix; }

   friend const BinaryOpDifference<T, MatrixImpl, MatrixImpl> operator -(const Matrix& left, const Matrix& right) { return left.m_matrix - right.m_matrix; }
   template<typename OtherType>
   friend const BinaryOpDifference<T, MatrixImpl, OtherType> operator -(const Matrix& left, const MatrixBase<OtherType>& right) { return left.m_matrix - right; }
   template<typename OtherType>
   friend const BinaryOpDifference<T, OtherType, MatrixImpl> operator -(const MatrixBase<OtherType>& left, const Matrix& right) { return left - right.m_matrix; }

   friend const ProductReturnType<T, MatrixImpl, MatrixImpl> operator *(const Matrix& left, const Matrix& right) { return left.m_matrix * right.m_matrix; }
   template<typename OtherType>
   friend const ProductReturnType<T, MatrixImpl, OtherType> operator *(const Matrix& left, const MatrixBase<OtherType>& right) { return left.m_matrix * right; }
   template<typename OtherType>
   friend const ProductReturnType<T, OtherType, MatrixImpl> operator *(const MatrixBase<OtherType>& left, const Matrix& right) { return left * right.m_matrix; }

   friend Matrix& operator+=(Matrix& left, const Matrix<T, NumRows, NumCols>& right) { left.m_matrix += right.m_matrix; return left; }
   template<typename OtherType>
   friend Matrix& operator+=(Matrix& left, const MatrixBase<OtherType>& right) { left.m_matrix += right; return left; }
   template<typename OtherType>
   friend OtherType& operator+=(const MatrixBase<OtherType>& left, Matrix& right) { left += right.m_matrix; return left; }
   friend Matrix& operator-=(Matrix& left, const Matrix& right) { left.m_matrix -= right.m_matrix; return left; }
   template<typename OtherType>
   friend Matrix& operator-=(Matrix& left, const MatrixBase<OtherType>& right) { left.m_matrix -= right; return left; }
   template<typename OtherType>
   friend OtherType& operator-=(const MatrixBase<OtherType>& left, Matrix& right) { left -= right.m_matrix; return left; }
   friend Matrix& operator*=(Matrix& left, const Matrix& right) { left.m_matrix *= right.m_matrix; return left; }
   template<typename OtherType>
   friend Matrix& operator*=(Matrix& left, const MatrixBase<OtherType>& right) { left.m_matrix *= right; return left; }
   template<typename OtherType>
   friend OtherType& operator*=(const MatrixBase<OtherType>& left, Matrix& right) { left *= right.m_matrix; return left; }

   // Boolean
   friend bool operator==(Matrix& left, const Matrix& right) { return left.m_matrix == right.m_matrix; }
   template<typename OtherType>
   friend bool operator==(Matrix& left, const MatrixBase<OtherType>& right) { return left.m_matrix == right; }
   template<typename OtherType>
   friend bool operator==(MatrixBase<OtherType>& left, const Matrix& right) { return left == right.m_matrix; }
   friend bool operator!=(Matrix& left, const Matrix& right) { return left.m_matrix != right.m_matrix; }
   template<typename OtherType>
   friend bool operator!=(Matrix& left, const MatrixBase<OtherType>& right) { return left.m_matrix != right; }
   template<typename OtherType>
   friend bool operator!=(MatrixBase<OtherType>& left, const Matrix& right) { return left != right.m_matrix; }

   // Scalar
   template<typename ScalarType>
   friend const UnaryOpScalarProduct<T, MatrixImpl> operator *(const Matrix& left, const ScalarType& right) { return left.m_matrix * static_cast<T>(right); }
   template<typename ScalarType>
   friend const UnaryOpScalarProduct<T, MatrixImpl> operator *(const ScalarType& left, const Matrix& right) { return static_cast<T>(left)* right.m_matrix; }
   template<typename ScalarType>
   friend const UnaryOpScalarQuotient<T, MatrixImpl> operator /(const Matrix& left, const ScalarType& right) { return left.m_matrix / static_cast<T>(right); }
   template<typename ScalarType>
   friend Matrix& operator*=(Matrix& left, const ScalarType& right) { left.m_matrix *= static_cast<T>(right); return left; }
   template<typename ScalarType>
   friend Matrix& operator/=(Matrix& left, const ScalarType& right) { left.m_matrix /= static_cast<T>(right); return left; }

private:
   MatrixImpl m_matrix;
};

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