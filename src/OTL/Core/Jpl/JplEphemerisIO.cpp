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

#include <OTL/Core/Jpl/JplEphemerisIO.h>
#include <OTL/Core/Logger.h>

namespace otl
{

int JplEphemerisIO::NUM_COEFFICIENTS = 13;
int JplEphemerisIO::NUM_DAYS_PER_RECORD = 32;
int JplEphemerisIO::HEADER_SIZE = (sizeof(int) * NUM_COEFFICIENTS * NUM_COEFFICIENTS) +
                                  (sizeof(int)) +
                                  (sizeof(double) * 2);

JplEphemerisIO::JplEphemerisIO(const std::string& dataFilename) :
   m_dataFilename(dataFilename),
   m_dataStream(),
   m_initialized(false)
{
   
}

void JplEphemerisIO::GetInterpolationInfo(double julianDay,
                                          int entity,
                                          std::vector<double>& coefficients,
                                          double& setsPerDay,
                                          double& chebyshevTime)
{
   // Lazy initialization
   if (!m_initialized)
      Initialize();

   // Sanity check on inputs
   if (julianDay < m_startJulianDay)
   {
      OTL_ERROR() << "Interpolation data request before earliest available date";
   }
   if (julianDay > m_endJulianDay)
   {
      OTL_ERROR() << "Interpolation data request after latest available date";
   }
   if (m_coefficientOffsets[entity] == -1)
   {
      OTL_ERROR() << "Interpolation data not available for requested entity";
   }

   // Number of coefficients for this entity
   int numCoefficients = m_numCoefficients[entity];

   //
   double numDaysPerRecord = static_cast<double>(NUM_DAYS_PER_RECORD);
   setsPerDay = m_numCoefficientSets[entity] / numDaysPerRecord;
   double daysPerSet = 1.0 / setsPerDay;

   // Determine which record we're in
   int recordNum = static_cast<int>((julianDay - m_startJulianDay) / numDaysPerRecord);
   double recordStartDay = recordNum * numDaysPerRecord + m_startJulianDay;

   // Determine which subdivision we're in
   int subRecordNum = static_cast<int>((julianDay - recordStartDay) * setsPerDay);

   // Jump to the offset
   long offset = HEADER_SIZE +
                 (recordNum * m_numCoefficientsPerRecord * sizeof(double)) +
                 (m_coefficientOffsets[entity] * sizeof(double)) +
                 (subRecordNum * numCoefficients * 3 * sizeof(double));

   m_dataStream.seekg(offset, std::ios_base::beg);
   if (!m_dataStream)
   {
      OTL_ERROR() << "Fatal error while seeking file offset [" << offset << "]";
   }

   int i = 0;
   union BufD
   {
      char cdata[sizeof(double)];
      double dblVal;
   } bufD;
   coefficients.clear();
   while (m_dataStream.read(bufD.cdata, sizeof(double)) && i < (numCoefficients * 3))
   {
      coefficients.push_back(bufD.dblVal);
      i++;
   }

   // Compute chebyshev time
   chebyshevTime = 2.0 * ((julianDay - (recordStartDay + subRecordNum * daysPerSet)) / daysPerSet) - 1;
   if (chebyshevTime < -1.0 || chebyshevTime > 1.0)
   {
      OTL_ERROR() << "Chebyshev time must be between -1..1 [" << chebyshevTime << "]";
   }
}

void JplEphemerisIO::Initialize()
{
   // Open the file
   m_dataStream.open(m_dataFilename, std::ios::binary);
   if (!m_dataStream)
   {
      OTL_FATAL() << "Failed to open data file [" << m_dataFilename << "]";
   }

   // Load the layout info from the header
   LoadLayoutInfo<int>(m_coefficientOffsets, NUM_COEFFICIENTS);
   LoadLayoutInfo<int>(m_numCoefficients, NUM_COEFFICIENTS);
   LoadLayoutInfo<int>(m_numCoefficientSets, NUM_COEFFICIENTS);
   LoadLayoutInfo<int>(m_numCoefficientsPerRecord);
   LoadLayoutInfo<double>(m_startJulianDay);
   LoadLayoutInfo<double>(m_endJulianDay);

   m_initialized = true;
}

template<typename T>
void JplEphemerisIO::LoadLayoutInfo(std::vector<T>& layoutInfo, int size)
{
   union BufT
   {
      char cdata[sizeof(T)];
      T value;
   } buffer;

   layoutInfo.clear();
   layoutInfo.reserve(size);
   for (int index = 0; index < size; ++index)
   {
      if (!m_dataStream.read(buffer.cdata, sizeof(int)))
      {
         OTL_ERROR() << "Failed to read layout info";
      }
      layoutInfo.push_back(buffer.value);
   }
}

template<typename T>
void LoadLayoutInfo(T& layoutInfo)
{
   union BufT
   {
      char cdata[sizeof(T)];
      T value;
   } buffer;

   if (!m_dataStream.read(buffer.cdata, sizeof(T)))
   {
      OTL_ERROR() << "Failed to load layout info";
   }
   layoutInfo = buffer.value;
}

} // namespace otl