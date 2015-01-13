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

namespace temp {

// Class methods

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix() :
m_matrix()
{
   //m_matrix.setConstant(defaultValue);
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
inline Matrix<T, NumRows, NumCols>::Matrix(const Matrix<T, NumRows, NumCols>& other)
{
   m_matrix = other.GetImpl();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>::Matrix(const Eigen::Matrix<T, NumRows, NumCols>& matrix)
{
   m_matrix = matrix;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::operator =(const Matrix<T, NumRows, NumCols>& other)
{
   if (this != &other)
   {
      m_matrix = other.GetImpl();
   }
   return *this;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::operator =(const Eigen::Matrix<T, NumRows, NumCols>&  other)
{
   if (&m_matrix != &other)
   {
      m_matrix = other;
   }
   return *this;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T& Matrix<T, NumRows, NumCols>::operator()(unsigned int row, unsigned int col)
{
   return m_matrix(row, col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const T& Matrix<T, NumRows, NumCols>::operator()(unsigned int row, unsigned int col) const
{
   return m_matrix(row, col);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
void Matrix<T, NumRows, NumCols>::Resize(unsigned int numRows, unsigned int numCols)
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
void Matrix<T, NumRows, NumCols>::Fill(T fillValue)
{
   m_matrix.fill(fillValue);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T Matrix<T, NumRows, NumCols>::GetMagnitude() const
{
   T sum = m_matrix.sum();
   return (sum / (NumRows * NumCols));
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline void Matrix<T, NumRows, NumCols>::Transpose()
{
   m_matrix.transposeInPlace();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline void Matrix<T, NumRows, NumCols>::Normalize()
{
   m_matrix /= GetMagnitude();
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline T Matrix<T, NumRows, NumCols>::Dot(const Matrix& other) const
{
   return m_matrix.dot(other.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols> Matrix<T, NumRows, NumCols>::Cross(const Matrix& other) const
{
   return Matrix<T, NumRows, NumCols>(m_matrix.cross(other.GetImpl()));
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::GetImpl()
{
   return m_matrix;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline const Eigen::Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::GetImpl() const
{
   return m_matrix;
}

// Free functions

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols> Cross(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   Matrix<T, NumRows, NumCols> result;
   return result;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Transpose(const Matrix<T, NumRows, NumCols>& other)
{
   return Matrix<T, NumRows, NumCols>(other.GetImpl().transpose());
}

// Static methods

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Matrix<T, NumRows, NumCols>::Zero()
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Zero());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Matrix<T, NumRows, NumCols>::Identity()
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Identity());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
Matrix<T, NumRows, NumCols> Matrix<T, NumRows, NumCols>::Constant(const T& value)
{
   return Matrix(Eigen::Matrix<T, NumRows, NumCols>::Constant(T));
}

// Operator overloads

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols> operator +(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return (left.GetImpl() + right.GetImpl());
   //return Matrix<T, NumRows, NumCols>(left.GetImpl() + right.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols> operator -(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return (left.GetImpl() - right.GetImpl());
   //return Matrix<T, NumRows, NumCols>(left.GetImpl() - right.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return (left.GetImpl() * right.GetImpl());
   //return Matrix<T, NumRows, NumCols>(left.GetImpl() * right.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>& operator+=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   left.GetImpl() += right.GetImpl();
   return left;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>& operator-=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   left.GetImpl() -= right.GetImpl();
   return left;
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline Matrix<T, NumRows, NumCols>& operator*=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   left.GetImpl() *= right.GetImpl();
   return left;
}

////////////////////////////////////////////////////////////
template<typename S, typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols> operator *(const Matrix<T, NumRows, NumCols>& left, const S& right)
{
   return (left.GetImpl() * right);
   //return Matrix<T, NumRows, NumCols>(left.GetImpl() * right);
}

////////////////////////////////////////////////////////////
template<typename S, typename T, int NumRows, int NumCols>
inline Eigen::CwiseUnaryOp<Eigen::internal::scalar_multiple_op<T>, const Eigen::Matrix<T, NumRows, NumCols>> operator *(const S& left, const Matrix<T, NumRows, NumCols>& right)
//template<typename S, typename T, int NumRows, int NumCols>
//inline Eigen::Matrix<T, NumRows, NumCols> operator *(const S& left, const Matrix<T, NumRows, NumCols>& right)
{
   const Eigen::Matrix<T, NumRows, NumCols>& mat = right.GetImpl();
   return Eigen::CwiseUnaryOp<Eigen::internal::scalar_multiple_op<T>, const Eigen::Matrix<T, NumRows, NumCols>>(mat, Eigen::internal::scalar_multiple_op<T>(static_cast<T>(left)));
   //return (static_cast<T>(left) * right.GetImpl());
   //return Matrix<T, NumRows, NumCols>(static_cast<T>(left) * right.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename S, typename T, int NumRows, int NumCols>
inline Eigen::Matrix<T, NumRows, NumCols> operator /(const Matrix<T, NumRows, NumCols>& left, const S& right)
{
   return (left.GetImpl() / right);
   //return Matrix<T, NumRows, NumCols>(left.GetImpl() / right);
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline bool operator==(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return (left.GetImpl() == right.GetImpl());
}

////////////////////////////////////////////////////////////
template<typename T, int NumRows, int NumCols>
inline bool operator!=(Matrix<T, NumRows, NumCols>& left, const Matrix<T, NumRows, NumCols>& right)
{
   return (left.GetImpl() != right.GetImpl());
}

//#define OTL_EXPLICIT_DEF(Type, Size)                           \
//template class Matrix<Type, Size, Size>;                       \
//template class Matrix<Type, Size, 1>;                          \
//template class Matrix<Type, 1, Size>;                          \
//template class Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;
//
//#define OTL_EXPLICIT_FUNC_DEF(Type, Size)   \
//template Matrix<Type, Size, Size> operator +(const Matrix<Type, Size, Size>&, const Matrix<Type, Size, Size>&);   \
//template Matrix<Type, Size, Size> operator -(const Matrix<Type, Size, Size>&, const Matrix<Type, Size, Size>&);   \
//
//#define OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, RowSize, ColSize)   \
//template Matrix<Type, RowSize, ColSize> Cross(const Matrix<Type, RowSize, ColSize>&, const Matrix<Type, RowSize, ColSize>&);  \
//
//#define OTL_EXPLICIT_DEF_ALL_SIZES(Type)  \
//OTL_EXPLICIT_DEF(Type, 2)                 \
//OTL_EXPLICIT_DEF(Type, 3)                 \
//OTL_EXPLICIT_DEF(Type, 6)
//
//#define OTL_EXPLICIT_FUNC_DEF_ALL_SIZES(Type)   \
//OTL_EXPLICIT_FUNC_DEF(Type, 2)                  \
//OTL_EXPLICIT_FUNC_DEF(Type, 3)                  \
//OTL_EXPLICIT_FUNC_DEF(Type, 6)                  \
//OTL_EXPLICIT_FUNC_DEF(Type, Eigen::Dynamic)
//
//#define OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY_ALL_SIZES(Type)  \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 2, 1)              \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 3, 1)              \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 6, 1)              \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, Eigen::Dynamic, 1) \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 1, 2)              \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 1, 3)              \
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY(Type, 1, Eigen::Dynamic)
//
//// Explicit template definitions for common types
//OTL_EXPLICIT_DEF_ALL_SIZES(int)
//OTL_EXPLICIT_DEF_ALL_SIZES(float)
//OTL_EXPLICIT_DEF_ALL_SIZES(double)
//OTL_EXPLICIT_FUNC_DEF_VECTOR_ONLY_ALL_SIZES(double)
//OTL_EXPLICIT_FUNC_DEF_ALL_SIZES(double)

} // namespace temp

} // namespace otl