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

   template<typename DerivedImpl> class MatBase;

#define OTL_DECLARE_MATRIX_EXPRESSION(ClassName, ...) \
class ClassName : public MatBase<__VA_ARGS__> \
{ \
public:  \
typedef MatBase<__VA_ARGS__> Base; \
ClassName() : Base() {} \
template<typename OtherImpl> ClassName(const Eigen::MatrixBase<OtherImpl>& other) : Base(other) {} \
template<typename OtherImpl> ClassName& operator =(const Eigen::MatrixBase<OtherImpl>& other) { m_impl = other; return *this; } \
};

#define OTL_DECLARE_SCALAR_EXPRESSION(ClassName, ...) \
class ClassName \
{ \
public: \
typedef __VA_ARGS__ Type; \
ClassName() : m_value() {} \
template<typename Scalar> ClassName(const Scalar& scalar) : m_value(scalar) {} \
template<typename Scalar> ClassName& operator =(const Scalar& scalar) { m_value = scalar; return *this; } \
private: \
Type m_value; \
};

//template<typename Derived>
//OTL_DECLARE_MATRIX_EXPRESSION(RowExpression, typename Eigen::MatrixBase<Derived>::RowXpr)
template<typename Derived>
OTL_DECLARE_MATRIX_EXPRESSION(CrossProductExpression, typename Eigen::MatrixBase<Derived>::template cross_product_return_type<Derived>::type)
template<typename Derived, typename OtherDerived>
OTL_DECLARE_SCALAR_EXPRESSION(DotProductExpression, typename Eigen::internal::scalar_product_traits<typename Eigen::internal::traits<Derived>::Scalar, typename Eigen::internal::traits<OtherDerived>::Scalar>::ReturnType)

template<typename Derived>
class RowExpression : public MatBase<typename Eigen::MatrixBase<Derived>::RowXpr>
{
public:
   typedef MatBase<typename Eigen::MatrixBase<Derived>::RowXpr> Base;
   RowExpression() : Base()
   {
      double d = 1.0;
   }
   template<typename OtherImpl>
   RowExpression(const Eigen::MatrixBase<OtherImpl>& other) : Base(other)
   {
      double d = 1.0;
   }
   template<typename OtherImpl>
   RowExpression& operator =(const Eigen::MatrixBase<OtherImpl>& other)
   {
      m_impl = other; return *this;
   }
};

//template<typename Derived, typename OtherDerived>
//class DotProductReturnType
//{
//public:
//   typedef typename Eigen::internal::scalar_product_traits<typename Eigen::internal::traits<Derived>::Scalar, typename Eigen::internal::traits<OtherDerived>::Scalar>::ReturnType Type;
//   DotProductReturnType() : m_value() {}
//   template<typename Scalar>
//   DotProductReturnType(const Scalar& other) : m_value(other) {}
//   template<typename Scalar>
//   DotProductReturnType& operator =(const Scalar& other) { m_value = other; return *this; }
//private:
//   Type m_value;
//};

template<typename DerivedImpl>
class MatBase
{
public:
   typedef Eigen::MatrixBase<DerivedImpl> Impl;
   MatBase() : m_impl(DerivedImpl()) {}
   template<typename OtherImpl>
   MatBase(const Eigen::MatrixBase<OtherImpl>& other) :
   m_impl(other)
   {
   }
   template<typename OtherImpl>
   MatBase& operator =(const Eigen::MatrixBase<OtherImpl>& other) { m_impl = other; }

   Impl& GetImpl() { return m_impl; }
   const Impl& GetImpl() const { return m_impl; }

   RowExpression<DerivedImpl> Row(unsigned int index)
   {
      return m_impl.row(index);
   }

   template<typename OtherDerivedImpl>
   CrossProductExpression<DerivedImpl> Cross(const MatBase<OtherDerivedImpl>& other) const
   {
      return m_impl.cross(other.GetImpl());
   }

   template<typename OtherDerivedImpl>
   DotProductExpression<DerivedImpl, OtherDerivedImpl> Dot(const MatBase<OtherDerivedImpl>& other) const
   {
      return m_impl.dot(other.GetImpl());
   }

protected:
   Impl m_impl;
};

template<
   typename _Scalar,
   int _Rows,
   int _Cols,
   int _Options = Eigen::AutoAlign | ((_Rows == 1 && _Cols != 1) ? Eigen::RowMajor
                                    : (_Cols == 1 && _Rows != 1) ? Eigen::ColMajor
                                    : Eigen::EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION),
   int _MaxRows = _Rows,
   int _MaxCols = _Cols>
class Mat;

template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
class Mat : public MatBase<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>>
{
public:
   typedef MatBase<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>> Base;
   Mat() : Base() {}
private:
};

typedef Mat<double, 3, 3> Mat3d;
typedef Mat<double, 3, 1> Vec3d;

} // namespace otl