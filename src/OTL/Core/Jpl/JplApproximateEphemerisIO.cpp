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

#include <OTL/Core/OrbitalBody.h> // for PhysicalProperties

namespace otl
{

typedef std::map<std::string, std::vector<double>> JplApproxDatabase;
static JplApproxDatabase g_database;

////////////////////////////////////////////////////////////
JplApproximateEphemerisIO::JplApproximateEphemerisIO() :
m_initialized(false),
m_startYear(0),
m_endYear(0),
m_keplersEquation(new keplerian::KeplersEquationElliptical())
{

}

////////////////////////////////////////////////////////////
JplApproximateEphemerisIO::JplApproximateEphemerisIO(const std::string& dataFilename) :
m_dataFilename(dataFilename),
m_initialized(false),
m_startYear(0),
m_endYear(0),
m_keplersEquation(new keplerian::KeplersEquationElliptical())

{

}

////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
//PhysicalProperties JplApproximateEphemerisIO::GetPhysicalProperties(const std::string& name)
//{
//   PhysicalProperties properties(1.0, 1.0);
//   return properties;
//}

////////////////////////////////////////////////////////////
bool JplApproximateEphemerisIO::IsValidName(const std::string& name) const
{
   const auto it = g_database.find(name);
   return (it != g_database.end());
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemerisIO::IsValidEpoch(const Epoch& epoch) const
{
   auto year = epoch.GetGregorian().year;
   return (year >= m_startYear && year <= m_endYear);
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

   //data["Mercury"] = std::vector<double>{
   //    0.38709927, 0.20563593, 7.00497902, 252.25032350, 77.45779628, 48.33076593,
   //    0.00000037, 0.00001906, -0.00594749, 149472.67411175, 0.16047689, -0.12534081};
   //data["Venus"] = std::vector < double > {
   //    0.72333566, 0.00677672, 3.39467605, 181.97909950, 131.60246718, 76.67984255,
   //    0.00000390, -0.00004107, -0.00078890, 58517.81538729, 0.00268329, -0.27769418};
   //data["Earth"] = std::vector < double > {
   //    1.00000261, 0.01671123, -0.00001531, 100.46457166, 102.93768193, 0.0,
   //    0.00000562, -0.00004392, -0.01294668, 35999.37244981, 0.32327364, 0.0};
   //data["Mars"] = std::vector < double > {
   //    1.52371034, 0.09339410, 1.84969142, -4.55343205, -23.94362959, 49.55953891,
   //    0.00001847, 0.00007882, -0.00813131, 19140.30268499, 0.44441088, -0.29257343};
   //data["Jupiter"] = std::vector < double > {
   //    5.20288700, 0.04838624, 1.30439695, 34.39644051, 14.72847983, 100.47390909,
   //    -0.00011607, -0.00013253, -0.00183714, 3034.74612775, 0.21252668, 0.20469106};
   //data["Saturn"] = std::vector < double > {
   //    9.53667594, 0.05386179, 2.48599187, 49.95424423, 92.59887831, 113.66242448,
   //    -0.00125060, -0.00050991, 0.00193609, 1222.49362201, -0.41897216, -0.28867794};
   //data["Uranus"] = std::vector < double > {
   //    19.18916464, 0.04725744, 0.77263783, 313.23810451, 170.95427630, 74.01692503,
   //    -0.00196176, -0.00004397, -0.00242939, 428.48202785, 0.40805281, 0.04240589};
   //data["Neptune"] = std::vector < double > {
   //    30.06992276, 0.00859048, 1.77004347, -55.12002969, 44.96476227, 131.78422574,
   //    0.00026291, 0.00005105, 0.00035372, 218.45945325, -0.32241464, -0.00508664};
   //data["Pluto"] = std::vector < double > {
   //    39.48211675, 0.24882730, 17.14001206, 238.92903833, 224.06891629, 110.30393684,
   //    -0.00031596, 0.00005170, 0.00004818, 145.20780515, -0.04062942, -0.01183482};

   // Ephemeris is valid between 3000 BC and 3000 AD
   m_startYear = -3000;
   m_endYear = 3000;
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
   ifs >> m_startYear >> m_endYear;

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