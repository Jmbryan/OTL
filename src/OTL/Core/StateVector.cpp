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
bool StateVector::operator==(const StateVector& other)
{
   return (position == other.position && velocity == other.velocity);
}

////////////////////////////////////////////////////////////
bool StateVector::operator!=(const StateVector& other)
{
   return (position != other.position || velocity != other.velocity);
}

////////////////////////////////////////////////////////////
std::string HumanReadable(const StateVector& stateVector)
{
   std::stringstream ss;
   ss << "State Vector:" << std::endl;
   ss << "   Position:" << std::endl;
   ss << "      X: " << stateVector.position.x() << std::endl;
   ss << "      Y: " << stateVector.position.y() << std::endl;
   ss << "      Z: " << stateVector.position.z() << std::endl;
   ss << "   Velocity:" << std::endl;
   ss << "      X: " << stateVector.velocity.x() << std::endl;
   ss << "      Y: " << stateVector.velocity.y() << std::endl;
   ss << "      Z: " << stateVector.velocity.z();
   return ss.str();
}

} // namespace otl