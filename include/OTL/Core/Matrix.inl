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

#include <OTL/Core/Matrix.h>
#include <OTL/Core/Logger.h>

namespace otl
{

#define FIXED_VECTOR_THREE_ONLY "THIS_FUNCTION_IS_ONLY_FOR_FIXED_SIZE_VECTORS_OF_SIZE_THREE"

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix() :
m_matrix()
{

}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix(unsigned int numElements) :
m_matrix(numElements)
{

}
////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix(unsigned int numRows, unsigned int numCols) :
m_matrix(numRows, numCols)
{

}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix(T x, T y, T z)
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   m_matrix(0) = x; m_matrix(1) = y; m_matrix(2) = z;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix(const Matrix<T, NumRows, NumCols>& other)
{
   m_matrix = other();
}

////////////////////////////////////////////////////////////
//template<typename T, int NumRows, int NumCols>
//inline Matrix<T, NumRows, NumCols>::Matrix(const Eigen::Matrix<T, NumRows, NumCols>& matrix)
//{
//   m_matrix = matrix;
//}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>&
Matrix<T, NumRows, NumCols>::operator =(const Matrix<T, NumRows, NumCols>& other)
{
   if (this != &other)
   {
      m_matrix = other();
   }
   return *this;
}

////////////////////////////////////////////////////////////
//template<typename T, int NumRows, int NumCols>
//inline Matrix<T, NumRows, NumCols>&
//Matrix<T, NumRows, NumCols>::operator =(const Eigen::Matrix<T, NumRows, NumCols>&  other)
//{
//   if (&m_matrix != &other)
//   {
//      m_matrix = other;
//   }
//   return *this;
//}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::~Matrix()
{

}

// Access

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::MatrixImpl&
Matrix<T, NumRows, NumCols>::operator()()
{
   return m_matrix;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const typename Matrix<T, NumRows, NumCols>::MatrixImpl&
Matrix<T, NumRows, NumCols>::operator()() const
{
   return m_matrix;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::operator()(unsigned int index)
{
   return m_matrix(index);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::operator()(unsigned int index) const
{
   return m_matrix(index);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::operator()(unsigned int row, unsigned int col)
{
   return m_matrix(row, col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::operator()(unsigned int row, unsigned int col) const
{
   return m_matrix(row, col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::operator[](unsigned int index)
{
   return m_matrix[index];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::operator[](unsigned int index) const
{
   return m_matrix[index];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::RowReturnValue
Matrix<T, NumRows, NumCols>::Row(unsigned int row)
{
   return m_matrix.row(row);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::RowReturnValue
Matrix<T, NumRows, NumCols>::Row(unsigned int row) const
{
   return m_matrix.row(row);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::ColumnReturnValue
Matrix<T, NumRows, NumCols>::Col(unsigned int col)
{
   return m_matrix.col(col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::ColumnReturnValue
Matrix<T, NumRows, NumCols>::Col(unsigned int col) const
{
   return m_matrix.col(col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::X()
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[0];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::X() const
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[0];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::Y()
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[1];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::Y() const
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[1];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T&
Matrix<T, NumRows, NumCols>::Z()
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[2];
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T&
Matrix<T, NumRows, NumCols>::Z() const
{
   static_assert((NumRows == 3 || NumCols == 3) && (NumRows == 1 || NumCols == 1), FIXED_VECTOR_THREE_ONLY);
   return m_matrix[2];
}

// Utility

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline void
Matrix<T, NumRows, NumCols>::Resize(unsigned int numRows, unsigned int numCols)
{
   if ((NumRows == Eigen::Dynamic || NumRows == numRows) &&
      (NumCols == Eigen::Dynamic || NumCols == numCols))
   {
      m_matrix.resize(numRows, numCols);
   }
   else
   {
      OTL_ERROR() << "Failed to resize matrix. Each matrix axis must have dynamic size or remain constant";
   }
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
//template<typename ScalarType>
inline void
Matrix<T, NumRows, NumCols>::Fill(const ScalarType& value)
{
   m_matrix.fill(static_cast<T>(value));
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T
Matrix<T, NumRows, NumCols>::GetNorm() const
{
   return m_matrix.norm();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T
Matrix<T, NumRows, NumCols>::GetSquaredNorm() const
{
   return m_matrix.squaredNorm();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline void
Matrix<T, NumRows, NumCols>::NormalizeInPlace()
{
   m_matrix /= GetNorm();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline void
Matrix<T, NumRows, NumCols>::TransposeInPlace()
{
   m_matrix.transposeInPlace();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::TransposeReturnType
Matrix<T, NumRows, NumCols>::Transpose()
{
   return m_matrix.transpose();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::ConstTransposeReturnType
Matrix<T, NumRows, NumCols>::Transpose() const
{
   return m_matrix.transpose();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::DotProductReturnType<typename Matrix<T, NumRows, NumCols>::MatrixImpl>
Matrix<T, NumRows, NumCols>::Dot(const Matrix& other) const
{
   return m_matrix.dot(other());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
template<typename OtherType>
inline typename Matrix<T, NumRows, NumCols>::DotProductReturnType<OtherType>
Matrix<T, NumRows, NumCols>::Dot(const MatrixBase<OtherType>& other) const
{
   return m_matrix.dot(other);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline typename Matrix<T, NumRows, NumCols>::CrossProductReturnType
Matrix<T, NumRows, NumCols>::Cross(const Matrix& other) const
{
   return m_matrix.cross(other());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
template<typename OtherType>
inline typename Matrix<T, NumRows, NumCols>::CrossProductReturnType
Matrix<T, NumRows, NumCols>::Cross(const MatrixBase<OtherType>& other) const
{
   return m_matrix.cross(other);
}

// Static methods

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>
Matrix<T, NumRows, NumCols>::Zero()
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Zero());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols>
Matrix<T, NumRows, NumCols>::Identity()
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Identity());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
//template<typename ScalarType>
Matrix<T, NumRows, NumCols>
Matrix<T, NumRows, NumCols>::Constant(const ScalarType& value)
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Constant(static_cast<T>(value)));
}

} // namespace otl