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
#include <string>
#include <vector>
#include <fstream>

namespace otl
{

class JplEphemerisIO
{
public:
   JplEphemerisIO(const std::string& dataFilename);
   void GetInterpolationInfo(double julianDay,
                             int entity,
                             std::vector<double>& coefficients,
                             double& setsPerDay,
                             double& chebyshevTime);

private:
   void Initialize();

   template<typename T>
   void LoadLayoutInfo(std::vector<T>& layoutInfo, int size);

   template<typename T>
   void LoadLayoutInfo(T& layoutInfo);

private:
   std::string m_dataFilename;
   std::ifstream m_dataStream;
   std::vector<int> m_coefficientOffsets;
   std::vector<int> m_numCoefficients;
   std::vector<int> m_numCoefficientSets;
   int m_numCoefficientsPerRecord;
   double m_startJulianDay;
   double m_endJulianDay;
   bool m_initialized;

   static int NUM_COEFFICIENTS;
   static int NUM_DAYS_PER_RECORD;
   static int HEADER_SIZE;
};

} // namespace otl