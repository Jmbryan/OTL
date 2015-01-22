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
inclination(),
argOfPericenter(),
lonOfAscendingNode(),
trueAnomaly()
{
}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(const OrbitalElements& other) :
semiMajorAxis(other.semiMajorAxis),
eccentricity(other.eccentricity),
inclination(other.inclination),
argOfPericenter(other.argOfPericenter),
lonOfAscendingNode(other.lonOfAscendingNode),
trueAnomaly(other.trueAnomaly)
{

}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(const OrbitalElements&& other) :
semiMajorAxis(std::move(other.semiMajorAxis)),
eccentricity(std::move(other.eccentricity)),
inclination(std::move(other.inclination)),
argOfPericenter(std::move(other.argOfPericenter)),
lonOfAscendingNode(std::move(other.lonOfAscendingNode)),
trueAnomaly(std::move(other.trueAnomaly))
{

}

////////////////////////////////////////////////////////////
OrbitalElements::OrbitalElements(double _semiMajorAxis, double _eccentricity, double _inclination,
                                 double _argOfPericenter, double _lonOfAscendingNode, double _trueAnomaly) :
semiMajorAxis(_semiMajorAxis),
eccentricity(_eccentricity),
inclination(_inclination),
argOfPericenter(_argOfPericenter),
lonOfAscendingNode(_lonOfAscendingNode),
trueAnomaly(_trueAnomaly)
{

}

////////////////////////////////////////////////////////////
std::string HumanReadable(const OrbitalElements& orbitalElements)
{
   std::stringstream ss;
   ss << "Orbital Elements:" << std::endl;
   ss << "   Semimajor Axis:              " << orbitalElements.semiMajorAxis << std::endl;
   ss << "   Eccentricity:                " << orbitalElements.eccentricity << std::endl;
   ss << "   Inclination:                 " << orbitalElements.inclination << std::endl;
   ss << "   Arguement of Pericenter:     " << orbitalElements.argOfPericenter << std::endl;
   ss << "   Longitude of Ascending Node: " << orbitalElements.lonOfAscendingNode << std::endl;
   ss << "   True Anomaly:                " << orbitalElements.trueAnomaly;
   return ss.str();
}

////////////////////////////////////////////////////////////
bool operator==(const OrbitalElements& lhs, const OrbitalElements& rhs)
{
   return (ApproxEqual(lhs.semiMajorAxis, rhs.semiMajorAxis) &&
            ApproxEqual(lhs.eccentricity, rhs.eccentricity) &&
            ApproxEqual(lhs.inclination, rhs.inclination) &&
            ApproxEqual(lhs.argOfPericenter, rhs.argOfPericenter) &&
            ApproxEqual(lhs.lonOfAscendingNode, rhs.lonOfAscendingNode) &&
            ApproxEqual(lhs.trueAnomaly, rhs.trueAnomaly));
}

////////////////////////////////////////////////////////////
bool operator!=(const OrbitalElements& lhs, const OrbitalElements& rhs)
{
   return !(lhs == rhs);
}

} // namespace otl