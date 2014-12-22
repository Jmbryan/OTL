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

#include <OTL/Core/Transformation.h>

////////////////////////////////////////////////////////////
namespace otl
{

void TransformPerifocal2Inertial(const Vector3d& perifocalVector,
                                 double inclination,
                                 double argOfPericenter,
                                 double lonOfAscendingNode,
                                 Vector3d& inertialVector)
{
   // Precalculate common trig functions.
   double cosIncl = cos(inclination);
   double sinIncl = sin(inclination);
   double cosAop = cos(argOfPericenter);
   double sinAop = sin(argOfPericenter);
   double cosLan = cos(lonOfAscendingNode);
   double sinLan = sin(lonOfAscendingNode);
   
   // Build the rotation matrix.
   Matrix3d matrix;
   matrix.row1.x =  (cosLan  * cosAop) - (sinLan * sinAop  * cosIncl);
   matrix.row1.y = -(cosLan  * sinAop) - (sinLan * cosIncl * cosAop);
   matrix.row1.z =  (sinLan  * sinIncl);
   matrix.row2.x =  (sinLan  * cosAop) + (cosLan * cosIncl * sinAop);
   matrix.row2.y = -(sinLan  * sinAop) + (cosLan * cosIncl * cosAop);
   matrix.row2.z = -(cosLan  * sinIncl);
   matrix.row3.x =  (sinIncl * sinAop);
   matrix.row3.y =  (sinIncl * cosAop);
   matrix.row3.z =  (cosIncl);

   // Rotate the perifocal vector to inertial coordinates.
   inertialVector = matrix * perifocalVector;
}

} // namespace otl