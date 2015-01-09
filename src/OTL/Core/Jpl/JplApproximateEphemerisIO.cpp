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

#include <OTL/Core/Jpl/JplApproximateEphemerisIO.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Logger.h>
#include <fstream>
#include <vector>

namespace otl
{

typedef std::map<std::string, std::vector<double>> JplApproxDatabase;
static JplApproxDatabase g_database;

JplApproximateEphemerisIO::JplApproximateEphemerisIO(const std::string& dataFilename) :
m_dataFilename(dataFilename),
m_initialized(false)
{

}

void JplApproximateEphemerisIO::GetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{

}

void JplApproximateEphemerisIO::GetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   // Number of centuries since J2000
   double T = (epoch.GetJD() - 2451545.0) / 36525.0;

   // Compute the orbital elements at the given epoch
   // The formula is element = element0 + elementRate * T
   // a has units AU
   // e has no units
   // all other elemnets have units of degrees
   const auto& data = g_database[name];
   double a        = data[0] + data[6]  * T;
   double e        = data[1] + data[7]  * T;
   double I        = data[2] + data[8]  * T;
   double L        = data[3] + data[9]  * T;
   double longPeri = data[4] + data[10] * T;
   double longNode = data[5] + data[11] * T;

   // Extract the additional terms used for computing the
   // mean anomaly
   double b = data[12];
   double c = data[13];
   double s = data[14];
   double f = data[15];

   // Compute argument of pericenter (degrees)
   double w = longPeri - longNode;

   // Compute mean anomaly (degrees)
   double M = L - longPeri +
   b * SQR(T) +
   c * cos(f * T) +
   s * sin(f * T);

   // Compute modulo of the mean anomaly and then limit to -180..180 degrees
   M = Modulo(M, 360.0);
   if (M > 180.0)
   {
   M -= 360.0;
   }

   // Convert mean anomaly to eccentric anomaly (radians)
   double E = m_keplersEquation->Evaluate(e, M * MATH_DEG_TO_RAD);

   // Convert eccentric anomaly to true anomaly (radians)
   double ta = ConvertEccentricAnomaly2TrueAnomaly(E, e);

   // Package the orbital elements after converting to standard units (km, rad)
   orbitalElements.semiMajorAxis       = a         * ASTRO_AU_TO_KM;
   orbitalElements.eccentricity        = e;
   orbitalElements.inclination         = I         * MATH_DEG_TO_RAD;
   orbitalElements.lonOfAscendingNode  = longNode  * MATH_DEG_TO_RAD;
   orbitalElements.argOfPericenter     = w         * MATH_DEG_TO_RAD;
   orbitalElements.trueAnomaly         = ta;
}

bool JplApproximateEphemerisIO::IsNameValid(const std::string& name) const
{
   return true;
}

bool JplApproximateEphemerisIO::IsEpochValid(const Epoch& epoch) const
{
   return true;
}

void JplApproximateEphemerisIO::Initialize()
{
   std::ifstream ifs(m_dataFilename);
   if (!ifs)
   {
      OTL_FATAL() << "Failed to open jpl approximate ephemeris database input file " << Bracket(m_dataFilename);
      return;
   }

   // Start and end year
   int startYear, endYear;
   ifs >> startYear >> endYear;

   unsigned int numPlanets;
   ifs >> numPlanets;

   std::vector<std::string> planetNames(numPlanets, "");
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      std::string name;
      ifs >> name;
      planetNames[p] = name;
   }

   // Number of ephemeris data values and additional terms
   unsigned int numEphemerisValues, numAdditionalTerms;
   ifs >> numEphemerisValues >> numAdditionalTerms;

   // Ephemeris data and additional terms for each planet
   std::vector<std::vector<double>> ephemeris(numPlanets);
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      for (int i = 0; i < numEphemerisValues; ++i)
      {
         double value;
         ifs >> value;
         ephemeris[p].push_back(value);
      }
   }
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      for (int i = 0; i < numAdditionalTerms; ++i)
      {
         double value;
         ifs >> value;
         ephemeris[p].push_back(value);
      }
   }

   // Store in the database
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      g_database[planetNames[p]] = ephemeris[p];
   }
}

} // namespace otl