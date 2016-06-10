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
#include <OTL/Core/Constants.h>
#include <OTL/Core/Logger.h>
#include <numeric>
#include <iterator>

namespace otl
{

namespace test
{

#define OTL_STATIC_ASSERT(condition, message) static_assert(condition, #message)
#define OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(type) \
OTL_STATIC_ASSERT(type::IsVector && type::Size == 3, THIS_METHOD_IS_ONLY_FOR_VECTORS_OF_SIZE_THREE)
#define OTL_STATIC_ASSERT_VECTOR_ONLY(type) \
OTL_STATIC_ASSERT(type::IsVector, THIS_METHOD_IS_ONLY_FOR_VECTORS)

template<typename T, int NumRows, int NumCols>
class Matrix
{
public:
   enum
   {
      Rows = NumRows,
      Columns = NumCols,
      Size = NumRows * NumCols,
      IsVector = (NumRows == 1 || NumCols == 1)
   };

   Matrix() {}

   explicit Matrix(const T& fillValue)
   {
      fill(fillValue);
   }

   Matrix(const Matrix& other)
   {
      *this = other;
   }

   Matrix(const Matrix& other, const T& scalar)
   {
      *this = other;
      scale(scalar);
   }

   Matrix(const T& _x, const T& _y, const T& _z)
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      x() = _x;
      y() = _y;
      z() = _z;
   }

   Matrix& operator =(const Matrix& other)
   {
      if (this != &other)
      {
         memcpy(m_data, other.GetRawData(), sizeof(T)*GetSize());
      }
      return *this;
   }

   T* GetRawData()
   {
      return m_data;
   }

   const T* GetRawData() const
   {
      return m_data;
   }

   T* data()
   {
      return m_data;
   }

   const T* data() const
   {
      return m_data;
   }

   int GetNumRows() const
   {
      return Rows;
   }

   int GetNumCols() const
   {
      return Columns;
   }

   int GetSize() const
   {
      return Size;
   }

   T operator()(int row, int column) const
   {
      OTL_ASSERT(row >= 0 && row < Rows, "Row index out of bounds");
      OTL_ASSERT(column >= 0 && column < Columns, "Column index out of bounds");
      return m_data[row + column * NumRows];
   }

   T& operator()(int row, int column)
   {
      OTL_ASSERT(row >= 0 && row < Rows, "Row index out of bounds");
      OTL_ASSERT(column >= 0 && column < Columns, "Column index out of bounds");
      return m_data[row + column * NumRows];
   }

   T operator[](int index) const
   {
      OTL_ASSERT(index >= 0 && index < GetSize(), "Index out of bounds");
      return m_data[index];
   }

   T& operator[](int index)
   {
      OTL_ASSERT(index >= 0 && index < GetSize(), "Index out of bounds");
      return m_data[index];
   }

   T x() const
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[0];
   }

   T& x()
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[0];
   }

   T y() const
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[1];
   }

   T& y()
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[1];
   }

   T z() const
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[2];
   }

   T& z()
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return m_data[2];
   }

   void fill(const T& fillValue)
   {
      std::fill(m_data, m_data + Size, fillValue);
   }

   void scale(const T& scalar)
   {
      std::for_each(m_data, m_data + Size,
         [&scalar](T& value){value *= scalar;}
      );
   }

   T squaredNorm() const
   {
      OTL_STATIC_ASSERT_VECTOR_ONLY(Matrix);
      return std::accumulate(m_data, m_data + Size, T(0),
         [](const T& sum, const T& value){return sum + value*value;}
      );
   }

   T norm() const
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return sqrt(squaredNorm());
   }

   void normalize()
   {
      OTL_STATIC_ASSERT_VECTOR_ONLY(Matrix);
      T n = norm();
      std::for_each(m_data, m_data + GetSize(),
         [n](T& value){value /= n;}
      );
   }

   Matrix normalized() const
   {
      OTL_STATIC_ASSERT_VECTOR_ONLY(Matrix);
      return Matrix(*this).normalizeInPlace(); // [TODO] Remove extra temporary
   }

   Matrix& normalizeInPlace()
   {
      OTL_STATIC_ASSERT_VECTOR_ONLY(Matrix);
      normalize();
      return *this;
   }

   T dot(const Matrix& other) const
   {
      OTL_STATIC_ASSERT_VECTOR_ONLY(Matrix);
      return (
         x() * other.x() +
         y() * other.y() +
         z() * other.z());
   }

   Matrix cross(const Matrix& other) const
   {
      OTL_STATIC_ASSERT_VECTOR_SIZE_THREE(Matrix);
      return Matrix(
         y() * other.z() - z() * other.y(),
         z() * other.x() - x() * other.z(),
         x() * other.y() - y() * other.x());
   }

   bool isApprox(const Matrix& other, const T& epsilon = 2.0 * MATH_EPSILON) const
   {
#ifdef OTL_COMPILER_MSVC
      return std::equal(m_data, m_data + Size, stdext::checked_array_iterator<const T*>(other.GetRawData(), other.GetSize()),
         [&epsilon](const T& left, const T& right) -> bool {return IsApprox(left, right, epsilon);}
#else
      return std::equal(m_data, m_data + Size,
         [&epsilon](const T& left, const T& right) -> bool {return IsApprox(left, right, epsilon); }
#endif
      );
   }

   bool isZero() const
   {
      return true;
   }

   void Zero()
   {
      std::fill(m_data, m_data + Size, T(0));
   }

   static Matrix Zeros()
   {
      return Matrix(0);
   }

   static Matrix Ones()
   {
      return Matrix(1);
   }

   static Matrix<T, 3, 1> UnitX()
   {
      return Matrix<T, 3, 1>(1, 0, 0);
   }

   static Matrix<T, 3, 1> UnitY()
   {
      return Matrix<T, 3, 1>(0, 1, 0);
   }

   static Matrix<T, 3, 1> UnitZ()
   {
      return Matrix<T, 3, 1>(0, 0, 1);
   }

private:
   T m_data[NumRows * NumCols];
};

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator -(const Matrix<T, NumRows, NumCols>& left)
{
   return Matrix<T, NumRows, NumCols>(left, -1.0);
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator+=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   auto size = left.GetSize();
   auto pLeft = left.GetRawData();
   auto pRight = right.GetRawData();
   for (int i = 0; i < size; ++i, ++pLeft, ++pRight)
   {
      *pLeft += *pRight;
   }
   return left;
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator-=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   auto size = left.GetSize();
   auto pLeft = left.GetRawData();
   auto pRight = right.GetRawData();
   for (int i = 0; i < size; ++i, ++pLeft, ++pRight)
   {
      *pLeft -= *pRight;
   }
   return left;
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator +(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return Matrix<T, NumRows, NumCols>(left) += right;
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator -(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return Matrix<T, NumRows, NumCols>(left) -= right;
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, NumCols>& left, const T& scalar)
{
   return Matrix<T, NumRows, NumCols>(left, scalar);
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator *(const T& scalar, const Matrix<T, NumRows, NumCols>& right)
{
   return Matrix<T, NumRows, NumCols>(right, scalar);
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator*=(Matrix<T, NumRows, NumCols>& left, const T& scalar)
{
   auto size = left.GetSize();
   auto pLeft = left.GetRawData();
   for (int i = 0; i < size; ++i, ++pLeft)
   {
      *pLeft *= scalar;
   }
   return left;
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> operator /(const Matrix<T, NumRows, NumCols>& left, const T& scalar)
{
   OTL_ASSERT(fabs(scalar) > MATH_EPSILON, "Divide by zero");
   return Matrix<T, NumRows, NumCols>(left, T(1) / scalar);
}

template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>& operator/=(Matrix<T, NumRows, NumCols>& left, const T& scalar)
{
   auto size = left.GetSize();
   auto pLeft = left.GetRawData();
   for (int i = 0; i < size; ++i, ++pLeft)
   {
      *pLeft /= scalar;
   }
   return left;
}

template<typename T, int NumRows, int NumCols>
bool operator==(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return left.isApprox(right);
}

template<typename T, int NumRows, int NumCols>
bool operator!=(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return !left.isApprox(right);
}

template<typename T, int NumRows, int InnerSize, int NumCols>
Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, InnerSize>& left, const Matrix<T, InnerSize, NumCols>& right)
{
   Matrix<T, NumRows, NumCols> result;
   for (int row = 0; row < NumRows; ++row)
   {
      for (int col = 0; col < NumCols; ++col)
      {
         result[row + col * NumRows] = static_cast<T>(0);
         for (int k = 0; k < InnerSize; ++k)
         {
            result[row + col * NumRows] += left[row + k * NumRows] * right[k + col * InnerSize];
         }
      }
   }
   return result;
}

} // namespace test
} // namespace otl