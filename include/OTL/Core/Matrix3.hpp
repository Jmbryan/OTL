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
#include <OTL/Core/Vector3.hpp>

namespace otl
{

template <typename T>
class Matrix3
{
public:

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ///
   /// Creates a Matrix3(Vector3(0), Vector3(0), Vector3(0)).
   ///
   ////////////////////////////////////////////////////////////
   Matrix3();

   ////////////////////////////////////////////////////////////
   /// \brief Construct the matrix with an initial value
   ///
   /// \param Val Initial value for all elements of the matrix
   ///
   ////////////////////////////////////////////////////////////
   Matrix3(T Val);

   ////////////////////////////////////////////////////////////
   /// \brief Construct the matrix from its rows
   ///
   /// \param Row1 First row
   /// \param Row2 Second row
   /// \param Row3 Third row
   ///
   ////////////////////////////////////////////////////////////
   Matrix3(const Vector3<T>& Row1, const Vector3<T>& Row2, const Vector3<T>& Row3);

   ////////////////////////////////////////////////////////////
   /// \brief Construct the matrix from another type of matrix
   ///
   /// This constructor doesn't replace the copy constructor,
   /// it's called only when U != T.
   /// A call to this constructor will fail to compile if U
   /// is not convertible to T.
   ///
   /// \param matrix Matrix to convert
   ///
   ////////////////////////////////////////////////////////////
   template <typename U>
   Matrix3(const Matrix3<U>& matrix);

   ////////////////////////////////////////////////////////////
   // \brief Access each row of the matrix
   //
   // Convenience function for accessing each row of the matrix
   // using the natural [] syntax. However, it is more efficient
   // to directly access the rows through the public members.
   //
   // \param row Row to access
   // 
   ////////////////////////////////////////////////////////////
   Vector3<T>& operator[](int row);

   ////////////////////////////////////////////////////////////
   // \brief Take the transpose of the matrix
   ////////////////////////////////////////////////////////////
   Matrix3<T>& Transpose();

   ////////////////////////////////////////////////////////////
   // \brief Construct an identity matrix
   //
   // An identity matrix consist of all zeros except for its
   // main diagonal which are all ones.
   //
   ////////////////////////////////////////////////////////////
   static Matrix3<T> Identity();

   ////////////////////////////////////////////////////////////
   // Member data
   ////////////////////////////////////////////////////////////
   Vector3<T> row1;  ///< First row of the matrix
   Vector3<T> row2;  ///< Second row of the matrix
   Vector3<T> row3;  ///< Third row of the matrix
};

// Matrix/Matrix operations

template <typename T>
Matrix3<T>& operator +=(Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
Matrix3<T>& operator -=(Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
Matrix3<T>& operator *=(Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
Matrix3<T> operator +(const Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
Matrix3<T> operator -(const Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
Matrix3<T> operator *(const Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
bool operator ==(const Matrix3<T>& left, const Matrix3<T>& right);

template <typename T>
bool operator !=(const Matrix3<T>& left, const Matrix3<T>& right);

// Matrix/Vector operations

template <typename T>
Vector3<T> operator *(const Matrix3<T>& left, const Vector3<T>& right);

// Matrix/Scalar operations

template <typename T>
Matrix3<T>& operator *=(Matrix3<T>& left, T right);

template <typename T>
Matrix3<T>& operator /=(Matrix3<T>& left, T right);

template <typename T>
Matrix3<T>& operator +=(Matrix3<T>& left, T right);

template <typename T>
Matrix3<T>& operator -=(Matrix3<T>& left, T right);

template <typename T>
Matrix3<T> operator *(const Matrix3<T>& left, T right);

template <typename T>
Matrix3<T> operator /(const Matrix3<T>& left, T right);

template <typename T>
Matrix3<T> operator +(const Matrix3<T>& left, T right);

template <typename T>
Matrix3<T> operator -(const Matrix3<T>& left, T right);

#include <OTL/Core/Matrix3.inl>

// Define the most common types
typedef Matrix3<int>    Matrix3i;
typedef Matrix3<float>  Matrix3f;
typedef Matrix3<double> Matrix3d;

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Matrix3
/// \ingroup core
///
/// otl::Matrix3 is a simple class that defines a mathematical
/// matrix with three rows and three columns (3x3 matrix).
///
/// Internally, each row is represented by a simple 3-dimensional
/// vector class (Vector<T>). Each row is a public member and can
/// be accessed directly (i.e matrix.row1) or indirectly through the
//// convenient operator[] function which allows for more natural matrix
/// indexing (i.e matrix[0]).
///
/// The template parameter T is the type of the values. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// You generally don't have to care about the templated form (otl::Matrix3<T>),
/// the most common specializations have special typedefs:
/// \li otl::Matrix3<int> is otl::Matrix3i
/// \li otl::Matrix3<float> is otl::Matrix3f
/// \li otl::Matrix3<double> is otl::Matrix3d
///
/// Usage example:
/// \code
/// otl::Matrix3f m1(1.0f);
/// m1.row1.x = 2.0f; // Most efficient way to access each row
/// m1[1].y = 3.0f; // Convenient (but less efficient) way to access each row
///
/// otl::Matrix3f m2(m1);
/// otl::Matrix3f m3 = m1 * m2;
/// otl::Vector3f v1 = Vector3f(1.0f, 2.0f, 3.0f);
/// otl::Vector3f v2 = m3 * v1;
///
/// bool different = (m2 != m3);
/// \endcode
///
////////////////////////////////////////////////////////////
