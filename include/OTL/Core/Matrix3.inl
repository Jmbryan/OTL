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


////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>::Matrix3() :
row1(Vector3<T>()),
row2(Vector3<T>()),
row3(Vector3<T>())
{
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>::Matrix3(T Val) :
row1(Vector3<T>(Val, Val, Val)),
row2(Vector3<T>(Val, Val, Val)),
row3(Vector3<T>(Val, Val, Val))
{
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>::Matrix3(const Vector3<T>& Row1, const Vector3<T>& Row2, const Vector3<T>& Row3) :
row1(Row1),
row2(Row2),
row3(Row3)
{
}

////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Matrix3<T>::Matrix3(const Matrix3<U>& matrix) :
row1(matrix.row1),
row2(matrix.row2),
row3(matrix.row3)
{
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>& Matrix3<T>::operator[](int row)
{
   assert(row >= 0 && row < 3);
   if (row == 0) return row1;
   if (row == 1) return row2;
   return row3;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& Matrix3<T>::Transpose()
{
   // Save the lower triangle values.
   T row2x = row2.x;
   T row3x = row3.x;
   T row3y = row3.y;

   // Copy the upper triable values to the lower triangle.
   row2.x = row1.y;
   row3.x = row1.z;
   row3.y = row2.z;

   // Copy the saved lower triangle values to the upper triangle.
   row1.y = row2x;
   row1.z = row3x;
   row2.z = row3y;

   return *this;
}

////////////////////////////////////////////////////////////
template <typename T>
Matrix3<T> Matrix3<T>::Identity()
{
   Matrix3<T> matrix(0);
   matrix.row1.x = static_cast<T>(1);
   matrix.row2.y = static_cast<T>(1);
   matrix.row3.z = static_cast<T>(1);

   return matrix;
}

// Matrix/Matrix operations

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator +=(Matrix3<T>& left, const Matrix3<T>& right)
{
   left.row1 += right.row1;
   left.row2 += right.row2;
   left.row3 += right.row3;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator -=(Matrix3<T>& left, const Matrix3<T>& right)
{
   left.row1 -= right.row1;
   left.row2 -= right.row2;
   left.row3 -= right.row3;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator *=(Matrix3<T>& left, const Matrix3<T>& right)
{
   left.row1.x =
      left.row1.x * right.row1.x +
      left.row1.y * right.row2.x +
      left.row1.z * right.row3.x;
   left.row1.y =
      left.row1.x * right.row1.y +
      left.row1.y * right.row2.y +
      left.row1.z * right.row3.y;
   left.row1.z =
      left.row1.x * right.row1.z +
      left.row1.y * right.row2.z +
      left.row1.z * right.row3.z;

   left.row2.x =
      left.row2.x * right.row1.x +
      left.row2.y * right.row2.x +
      left.row2.z * right.row3.x;
   left.row2.y =
      left.row2.x * right.row1.y +
      left.row2.y * right.row2.y +
      left.row2.z * right.row3.y;
   left.row2.z =
      left.row2.x * right.row1.z +
      left.row2.y * right.row2.z +
      left.row2.z * right.row3.z;

   left.row3.x =
      left.row3.x * right.row1.x +
      left.row3.y * right.row2.x +
      left.row3.z * right.row3.x;
   left.row3.y =
      left.row3.x * right.row1.y +
      left.row3.y * right.row2.y +
      left.row3.z * right.row3.y;
   left.row3.z =
      left.row3.x * right.row1.z +
      left.row3.y * right.row2.z +
      left.row3.z * right.row3.z;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator +(const Matrix3<T>& left, const Matrix3<T>& right)
{
   return Matrix3<T>(left) += right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator -(const Matrix3<T>& left, const Matrix3<T>& right)
{
   return Matrix3<T>(left) -= right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator *(const Matrix3<T>& left, const Matrix3<T>& right)
{
   return Matrix3<T>(left) *= right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Matrix3<T>& left, const Matrix3<T>& right)
{
   return (left.row1 == right.row1) && (left.row2 == right.row2) && (left.row3 == right.row3);
}

////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Matrix3<T>& left, const Matrix3<T>& right)
{
   return (left.row1 != right.row1) || (left.row2 != right.row2) || (left.row3 != right.row3);
}

// Matrix/Vector operations

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator *(const Matrix3<T>& left, const Vector3<T>& right)
{
   return Vector3<T>(left.row1.x * right.x + left.row1.y * right.y + left.row1.z * right.z,
                         left.row2.x * right.x + left.row2.y * right.y + left.row2.z * right.z,
                         left.row3.x * right.x + left.row3.y * right.y + left.row3.z * right.z);
}

// Matrix/Scalar operations

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator *=(Matrix3<T>& left, T right)
{
   left.row1 *= right;
   left.row2 *= right;
   left.row3 *= right;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator /=(Matrix3<T>& left, T right)
{
   left.row1 /= right;
   left.row2 /= right;
   left.row3 /= right;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator +=(Matrix3<T>& left, T right)
{
   left.row1 += right;
   left.row2 += right;
   left.row3 += right;

   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T>& operator -=(Matrix3<T>& left, T right)
{
   left.row1 -= right;
   left.row2 -= right;
   left.row3 -= right;
   
   return left;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator *(const Matrix3<T>& left, T right)
{
   return Matrix3<T>(left) *= right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator /(const Matrix3<T>& left, T right)
{
   return Matrix3<T>(left) /= right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator +(const Matrix3<T>& left, T right)
{
   return Matrix3<T>(left) += right;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Matrix3<T> operator -(const Matrix3<T>& left, T right)
{
   return Matrix3<T>(left) -= right;
}