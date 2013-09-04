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
inline Vector3<T>::Vector3() :
x(0),
y(0),
z(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>::Vector3(T X, T Y, T Z) :
x(X),
y(Y),
z(Z)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Vector3<T>::Vector3(const Vector3<U>& vector) :
x(static_cast<T>(vector.x)),
y(static_cast<T>(vector.y)),
z(static_cast<T>(vector.z))
{
}

////////////////////////////////////////////////////////////
template <typename T>
inline T Vector3<T>::operator[](int index) const
{

}

////////////////////////////////////////////////////////////
template <typename T>
inline T Vector3<T>::Magnitude() const
{
   return sqrt(x*x + y*y + z*z);
}

////////////////////////////////////////////////////////////
template <typename T>
inline T Vector3<T>::Dot(const Vector3<T>& vector)
{
   return x*vector.x + y*vector.y + z*vector.z;
}

////////////////////////////////////////////////////////////
template <typename T>
inline T Vector3<T>::Dot(const Vector3<T>& left, const Vector3<T>& right)
{
   return left.x*right.x + left.y*right.y + left.z*right.z;
}

////////////////////////////////////////////////////////////
template <typename T>
inline void Vector3<T>::Dot(const Vector3<T>& left, const Vector3<T>& right, T& result)
{
   result = left.x*right.x + left.y*right.y + left.z*right.z;
}

////////////////////////////////////////////////////////////
template <typename T>
inline void Vector3<T>::Cross(const Vector3<T>& vector)
{
   x = (y * vector.z) - (z * vector.y);
   y = (z * vector.x) - (x * vector.z);
   z = (x * vector.y) - (y * vector.x);
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& left, const Vector3<T> right)
{
   return Vector3<T>((left.y * right.z) - (left.z * right.y),
                     (left.z * right.x) - (left.x * right.z),
                     (left.x * right.y) - (left.y * right.x));
}

////////////////////////////////////////////////////////////
template <typename T>
inline void Vector3<T>::Cross(const Vector3<T>& left, const Vector3<T> right, Vector3<T>& result)
{
   result.x = (left.y * right.z) - (left.z * right.y);
   result.y = (left.z * right.x) - (left.x * right.z);
   result.z = (left.x * right.y) - (left.y * right.x);
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator -(const Vector3<T>& left)
{
    return Vector3<T>(-left.x, -left.y, -left.z);
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>& operator +=(Vector3<T>& left, const Vector3<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>& operator -=(Vector3<T>& left, const Vector3<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator *(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.x * right, left.y * right, left.z * right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator *(T left, const Vector3<T>& right)
{
    return Vector3<T>(right.x * left, right.y * left, right.z * left);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>& operator *=(Vector3<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> operator /(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.x / right, left.y / right, left.z / right);
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T>& operator /=(Vector3<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}
