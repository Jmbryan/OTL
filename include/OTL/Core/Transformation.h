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
#include <OTL/Core/Base.h>

namespace otl
{

////////////////////////////////////////////////////////////
/// \brief Transform a 3D vector from perifocal to inertial reference frames
/// \ingroup otl
///
/// \reference H. Curtis. Orbital Mechanics for Engineering Students 1st Edition
/// Algorthm ?? (X.X)XX, 2007
///
/// \param perifocalVector Vector in perifocal coordinates
/// \param inclinaiton Inclination (radians)
/// \param argOfPericenter Arguement of Perigee (radians)
/// \param lonOfAscendingNode Right Ascension of the Ascending Node (radians)
/// \param [out] inertialVector Transformed vector in inertial coordinates.
///
////////////////////////////////////////////////////////////
void TransformPerifocal2Inertial(const Vector3d& perifocalVector,
                                 double inclination,
                                 double argOfPericenter,
                                 double lonOfAscendingNode,
                                 Vector3d& inertialVector);

} // namespace otl
