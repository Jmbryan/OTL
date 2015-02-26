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
#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>
#include <fstream>
#include <vector>

namespace otl
{

typedef std::map<std::string, std::vector<double>> JplApproxDatabase;
static JplApproxDatabase g_database;

////////////////////////////////////////////////////////////
JplApproximateEphemerisIO::JplApproximateEphemerisIO()
{

}

////////////////////////////////////////////////////////////
JplApproximateEphemerisIO::JplApproximateEphemerisIO(const std::string& dataFilename) :
m_dataFilename(dataFilename)
{

}

////////////////////////////////////////////////////////////
OrbitalElements JplApproximateEphemerisIO::GetOrbitalElements(const std::string& name, const Epoch& epoch)
{
   // Check if the data is already cached, if not then cache it
   if (name != m_cache.first)
   {
      m_cache = std::make_pair(name, g_database[name]);
   }

   // Number of centuries since J2000
   double T = (epoch.GetJD() - 2451545.0) / 36525.0;

   // Compute the orbital elements at the given epoch
   // The formula is element = element0 + elementRate * T
   // a has units AU
   // e has no units
   // all other elemnets have units of degrees
   const auto& data = m_cache.second;
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

   // Return the state vector in Orbital format after converting to standard units (km, rad)
   return OrbitalElements(
      a         * ASTRO_AU_TO_KM,
      e,
      M         * MATH_DEG_TO_RAD,
      I         * MATH_DEG_TO_RAD,
      longNode  * MATH_DEG_TO_RAD,
      w         * MATH_DEG_TO_RAD);
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemerisIO::IsValidName(const std::string& name) const
{
   const auto it = g_database.find(name);
   return (it != g_database.end());
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemerisIO::IsValidEpoch(const Epoch& epoch) const
{
   return (epoch >= m_startEpoch && epoch <= m_endEpoch);
}

////////////////////////////////////////////////////////////
void JplApproximateEphemerisIO::Initialize()
{
   if (!m_dataFilename.empty())
   {
      Load();
      return;
   }

   OTL_DEBUG() << "No JPL approximate ephemeris data file was specified: Using default values";

   // Default ephemeris is valid between 3000 BC and 3000 AD
   m_startEpoch = Epoch::Gregorian(GregorianDateTime(-3000, 1, 1));
   m_endEpoch = Epoch::Gregorian(GregorianDateTime(3000, 1, 1));

   // Keplerian elements and rates provided by JPL.
   // The first six entries are the keplerian elements
   // The next six entries are the rates expressed in terms of centuries [/cy].
   // The last four entries are the additional terms (b, c, s, f) required
   // to compute the mean anomaly for Jupiter through Pluto.
   // The six orbital elements are listed in order below:
   //   Semi-major axis [au]
   //   Eccentricity  []
   //   Inclination [deg]
   //   Mean longitude [deg]
   //   Longitude of perihelion [deg]
   //   Longitude of the ascending node [deg]
   g_database["Mercury"] = std::vector<double> {
      0.38709843, 0.20563661, 7.00559432, 252.25166724, 77.45771895, 48.33961819,
      0.00000000, 0.00002123, -0.00590158, 149472.67486623, 0.15940013, -0.12214182,
      0.0, 0.0, 0.0, 0.0};
   g_database["Venus"] = std::vector<double> {
      0.72332102, 0.00676399, 3.39777545, 181.97970850, 131.76755713, 76.67261496,
      -0.00000026, -0.00005107, 0.00043494, 58517.81560260, 0.05679648, -0.27274174,
      0.0, 0.0, 0.0, 0.0};
   g_database["Earth"] = std::vector<double> {
      1.00000018, 0.01673163, -0.00054346, 100.46691572, 102.93005885, -5.11260389,
      -0.00000003, -0.00003661, -0.01337178, 35999.37306329, 0.31795260, -0.24123856,
      0.0, 0.0, 0.0, 0.0};
   g_database["Mars"] = std::vector<double> {
      1.52371243, 0.09336511, 1.85181869, -4.56813164, -23.91744784, 49.71320984,
      0.00000097, 0.00009149, -0.00724757, 19140.29934243, 0.45223625, -0.26852431,
      0.0, 0.0, 0.0, 0.0};
   g_database["Jupiter"] = std::vector<double> {
      5.20248019, 0.04853590, 1.29861416, 34.33479152, 14.27495244, 100.29282654,
      -0.00002864, 0.00018026, -0.00322699, 3034.90371757, 0.18199196, 0.13024619,
      -0.00012452, 0.06064060, -0.35635438, 38.35125000};
   g_database["Saturn"] = std::vector<double> {
      9.54149883, 0.05550825, 2.49424102, 50.07571329, 92.86136063, 113.63998702,
      -0.00003065, -0.00032044, 0.00451969, 1222.11494724, 0.54179478, -0.25015002,
      0.00025899, -0.13434469, 0.87320147, 38.35125000};
   g_database["Uranus"] = std::vector<double> {
      19.18797948, 0.04685740, 0.77298127, 314.20276625, 172.43404441, 73.96250215,
      -0.00020455, -0.00001550, -0.00180155, 428.49512595, 0.09266985, 0.05739699,
      0.00058331, -0.97731848, 0.17689245, 7.67025000};
   g_database["Neptune"] = std::vector<double> {
      30.06952752, 0.00895439, 1.77005520, 304.22289287, 46.68158724, 131.78635853,
         0.00006447, 0.00000818, 0.00022400, 218.46515314, 0.01009938, -0.00606302,
         -0.00041348, 0.68346318, -0.10162547, 7.67025000};
   g_database["Pluto"] = std::vector<double> {
      39.48686035, 0.24885238, 17.14104260, 238.96535011, 224.09702598, 110.30167986,
      0.00449751, 0.00006016, 0.00000501, 145.18042903, -0.00968827, -0.00809981,
      -0.01262724, 0.0, 0.0, 0.0};
}

////////////////////////////////////////////////////////////
void JplApproximateEphemerisIO::Load()
{
   std::ifstream ifs(m_dataFilename);
   if (!ifs)
   {
      OTL_FATAL() << "Failed to open JPL approximate ephemeris data file " << Bracket(m_dataFilename);
      return;
   }

   // Start and end year
   int startYear, endYear;
   ifs >> startYear >> endYear;
   m_startEpoch = Epoch::Gregorian(GregorianDateTime(startYear, 1, 1));
   m_endEpoch = Epoch::Gregorian(GregorianDateTime(endYear, 1, 1));

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
   unsigned int numEphemerisValues = 12;
   unsigned int numAdditionalTerms = 4;

   // Ephemeris data and additional terms for each planet
   std::vector<std::vector<double>> ephemeris(numPlanets);
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      for (unsigned int i = 0; i < numEphemerisValues; ++i)
      {
         double value;
         ifs >> value;
         ephemeris[p].push_back(value);
      }
   }
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      for (unsigned int i = 0; i < numAdditionalTerms; ++i)
      {
         double value;
         ifs >> value;
         ephemeris[p].push_back(value);
      }
   }

   // Save the ephemeris data for each planet to the database
   g_database.clear();
   for (unsigned int p = 0; p < numPlanets; ++p)
   {
      g_database[planetNames[p]] = ephemeris[p];
   }

   OTL_INFO() << "Successfully loaded JPL approximate ephemeris data file " << Bracket(m_dataFilename);
}

} // namespace otl