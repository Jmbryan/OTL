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

#include <OTL/Core/OrbitalElements.h>
#include <OTL/Core/Base.h>
#include <sstream>

namespace otl
{

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements() :
semiMajorAxis(),
eccentricity(),
trueAnomaly(),
inclination(),
argOfPericenter(),
lonOfAscendingNode()
{
}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(const OrbitalElements& other) :
semiMajorAxis(other.semiMajorAxis),
eccentricity(other.eccentricity),
trueAnomaly(other.trueAnomaly),
inclination(other.inclination),
argOfPericenter(other.argOfPericenter),
lonOfAscendingNode(other.lonOfAscendingNode)
{

}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(const OrbitalElements&& other) :
semiMajorAxis(std::move(other.semiMajorAxis)),
eccentricity(std::move(other.eccentricity)),
trueAnomaly(std::move(other.trueAnomaly)),
inclination(std::move(other.inclination)),
argOfPericenter(std::move(other.argOfPericenter)),
lonOfAscendingNode(std::move(other.lonOfAscendingNode))
{

}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(double _semiMajorAxis, double _eccentricity, double _trueAnomaly,
                                 double _inclination, double _argOfPericenter, double _lonOfAscendingNode) :
semiMajorAxis(_semiMajorAxis),
eccentricity(_eccentricity),
trueAnomaly(_trueAnomaly),
inclination(_inclination),
argOfPericenter(_argOfPericenter),
lonOfAscendingNode(_lonOfAscendingNode)
{

}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(std::initializer_list<double> list)
{
   auto size = list.size();
   auto it = list.begin();
   semiMajorAxis      = (size > 0) ? *(it++) : 0.0;
   eccentricity       = (size > 1) ? *(it++) : 0.0;
   trueAnomaly        = (size > 2) ? *(it++) : 0.0;
   inclination        = (size > 3) ? *(it++) : 0.0;
   argOfPericenter    = (size > 4) ? *(it++) : 0.0;
   lonOfAscendingNode = (size > 5) ? *(it++) : 0.0;
}

////////////////////////////////////////////////////////////
OrbitalElements& OrbitalElements::operator =(const OrbitalElements& other)
{
   if (this != &other)
   {
      semiMajorAxis = other.semiMajorAxis;
      eccentricity = other.eccentricity;
      trueAnomaly = other.trueAnomaly;
      inclination = other.inclination;
      argOfPericenter = other.argOfPericenter;
      lonOfAscendingNode = other.lonOfAscendingNode;
   }
   return *this;
}

////////////////////////////////////////////////////////////
OrbitalElements& OrbitalElements::operator =(const OrbitalElements&& other)
{
   if (this != &other)
   {
      semiMajorAxis = std::move(other.semiMajorAxis);
      eccentricity = std::move(other.eccentricity);
      trueAnomaly = std::move(other.trueAnomaly);
      inclination = std::move(other.inclination);
      argOfPericenter = std::move(other.argOfPericenter);
      lonOfAscendingNode = std::move(other.lonOfAscendingNode);
   }
   return *this;
}

////////////////////////////////////////////////////////////
std::string HumanReadable(const OrbitalElements& orbitalElements)
{
   const double rad2deg = MATH_RAD_TO_DEG;
   std::ostringstream ss;
   ss << "Orbital Elements:" << std::endl;
   ss << "   Semimajor Axis:              " << orbitalElements.semiMajorAxis << std::endl;
   ss << "   Eccentricity:                " << orbitalElements.eccentricity << std::endl;
   ss << "   True Anomaly:                " << orbitalElements.trueAnomaly * rad2deg << " deg" << std::endl;
   ss << "   Inclination:                 " << orbitalElements.inclination * rad2deg << " deg" << std::endl;
   ss << "   Arguement of Pericenter:     " << orbitalElements.argOfPericenter * rad2deg << " deg" << std::endl;
   ss << "   Longitude of Ascending Node: " << orbitalElements.lonOfAscendingNode * rad2deg << " deg" << std::endl;
   return ss.str();
}

////////////////////////////////////////////////////////////
bool operator==(const OrbitalElements& lhs, const OrbitalElements& rhs)
{
   return (IsApprox(lhs.semiMajorAxis, rhs.semiMajorAxis) &&
           IsApprox(lhs.eccentricity, rhs.eccentricity) &&
           IsApprox(lhs.trueAnomaly, rhs.trueAnomaly) &&
           IsApprox(lhs.inclination, rhs.inclination) &&
           IsApprox(lhs.argOfPericenter, rhs.argOfPericenter) &&
           IsApprox(lhs.lonOfAscendingNode, rhs.lonOfAscendingNode));
}

////////////////////////////////////////////////////////////
bool operator!=(const OrbitalElements& lhs, const OrbitalElements& rhs)
{
   return !(lhs == rhs);
}

} // namespace otl