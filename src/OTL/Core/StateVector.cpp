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

#include <OTL/Core/StateVector.h>
#include <OTL/Core/Base.h>

namespace otl
{

////////////////////////////////////////////////////////////
StateVector::StateVector() :
position({ 0.0, 0.0, 0.0 }), velocity({ 0.0, 0.0, 0.0 })
{

}

////////////////////////////////////////////////////////////
StateVector::StateVector(const StateVector& other) :
position(other.position),
velocity(other.velocity)
{

}

////////////////////////////////////////////////////////////
StateVector::StateVector(const StateVector&& other) :
position(std::move(other.position)),
velocity(std::move(other.velocity))
{

}

////////////////////////////////////////////////////////////
StateVector::StateVector(const Vector3d& _position, const Vector3d& _velocity) :
position(_position),
velocity(_velocity)
{

}

////////////////////////////////////////////////////////////
StateVector::StateVector(double x, double y, double z, double vx, double vy, double vz) :
position({ x, y, z }),
velocity({ vx, vy, vz })
{

}

////////////////////////////////////////////////////////////
StateVector::StateVector(std::initializer_list<double> list)
{
   auto size = list.size();
   auto it = list.begin();
   int index = 0;
   for (; it != list.end() && index < 3; ++it, ++index)
   {
      position[index] = static_cast<double>(*it);
   }
   for (; index < 3; ++index)
   {
      position[index] = 0.0;
   }

   index = 0;
   for (; it != list.end() && index < 3; ++it, ++index)
   {
      velocity[index] = static_cast<double>(*it);
   } 
   for (; index < 3; ++index)
   {
      velocity[index] = 0.0;
   }
}

////////////////////////////////////////////////////////////
StateVector& StateVector::operator =(const StateVector& other)
{
   if (this != &other)
   {
      position = other.position;
      velocity = other.velocity;
   }
   return *this;
}

////////////////////////////////////////////////////////////
StateVector& StateVector::operator =(const StateVector&& other)
{
   if (this != &other)
   {
      position = std::move(other.position);
      velocity = std::move(other.velocity);
   }
   return *this;
}

////////////////////////////////////////////////////////////
std::string StateVector::ToString() const
{
   std::ostringstream os;
   os << "x=" << position.x() << " "
      << "y=" << position.y() << " "
      << "z=" << position.z() << " "
      << "vx=" << velocity.x() << " "
      << "vy=" << velocity.y() << " "
      << "vz=" << velocity.z();

   return os.str();
}

////////////////////////////////////////////////////////////
std::string StateVector::ToDetailedString() const
{
   std::ostringstream os;
   os << "State Vector:" << std::endl;
   os << "   Position:" << std::endl;
   os << "      X: " << std::setprecision(6) << std::fixed << position.x() << std::endl;
   os << "      Y: " << std::setprecision(6) << std::fixed << position.y() << std::endl;
   os << "      Z: " << std::setprecision(6) << std::fixed << position.z() << std::endl;
   os << "   Velocity:" << std::endl;
   os << "      X: " << std::setprecision(6) << std::fixed << velocity.x() << std::endl;
   os << "      Y: " << std::setprecision(6) << std::fixed << velocity.y() << std::endl;
   os << "      Z: " << std::setprecision(6) << std::fixed << velocity.z() << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
bool operator==(const StateVector& lhs, const StateVector& rhs)
{
   return (lhs.position.isApprox(rhs.position, 2.0 * MATH_EPSILON) &&
           lhs.velocity.isApprox(rhs.velocity, 2.0 * MATH_EPSILON));
}

////////////////////////////////////////////////////////////
bool operator!=(const StateVector& lhs, const StateVector& rhs)
{
   return !(lhs == rhs);
}

} // namespace otl