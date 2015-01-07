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

#include <OTL/Core/MpcorbEphemerisIO.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>
#include <OTL/Core/Base.h>
#include <fstream>

namespace otl
{

typedef std::map<std::string, OrbitalElements> MpcorbDatabase;
MpcorbDatabase g_database;

////////////////////////////////////////////////////////////
MpcorbEphemerisIO::MpcorbEphemerisIO(const std::string& dataFilename) :
   m_dataFilename(dataFilename)
{

}

////////////////////////////////////////////////////////////
void MpcorbEphemerisIO::Initialize()
{
   std::ifstream ifs(m_dataFilename);
   if (!ifs)
   {
      OTL_FATAL() << "Error: unable to open mpcorb database input file '" << m_dataFilename;
      return;
   }

   int index;
   int recordsWritten = 0;
   while (ifs >> index)
   {
      char tempChar;
      int tempInt;
      std::string tempString;

      // Absolute magnitude, slope parameter, epoch (packed form), mean anomaly (degrees)
      double H, G, M;
      std::string epoch;
      ifs >> H >> G >> epoch >> M;

      // Argument of perihelion (degrees), longitude of the ascending node (degrees)
      double peri, node;
      ifs >> peri >> node;

      // Inclination to the ecliptic (degrees), orbital eccentricity
      double incl, e;
      ifs >> incl >> e;

      // Mean daily motion (deg/day), semimajor axis (AU)
      double n, a;
      ifs >> n >> a;

      // ?   
      ifs >> tempInt;

      // Reference, number of observations, number of oppositions
      std::string ref;
      int obs, opp;
      ifs >> ref >> obs >> opp;

      // Arc length (days), r.m.s residual, coarse indicator of perturbers
      int firstObsYear, lastObsYear;
      double rms;
      std::string perts;
      ifs >> firstObsYear >> tempChar >> lastObsYear >> rms >> perts;

      // ?, computer, ?
      std::string computer;
      ifs >> tempString >> computer >> tempInt;

      // Identifier, name     
      int id;
      std::string name;
      ifs >> tempChar >> id >> tempChar >> name;

      // Date of last observation included in orbit solution (YYYYMMDD format)
      int lastObsDate;
      ifs >> lastObsDate;

      // Convert mean anomaly to eccentric anomaly (radians)
      double E;
      if (e < 1.0)
      {
         keplerian::KeplersEquationElliptical kepler;
         E = kepler.Evaluate(e, M * MATH_DEG_TO_RAD);
      }
      else
      {
         keplerian::KeplersEquationHyperbolic kepler;
         E = kepler.Evaluate(e, M * MATH_DEG_TO_RAD);
      }

      // Convert eccentric anomaly to true anomaly (radians)
      double ta = ConvertEccentricAnomaly2TrueAnomaly(E, e);

      // Package the orbital elements after converting to standard units (km, rad)
      OrbitalElements orbitalElements;
      orbitalElements.semiMajorAxis       = a      * ASTRO_AU_TO_KM;
      orbitalElements.eccentricity        = e;
      orbitalElements.inclination         = incl   * MATH_DEG_TO_RAD;
      orbitalElements.argOfPericenter     = peri   * MATH_DEG_TO_RAD;
      orbitalElements.lonOfAscendingNode  = node   * MATH_DEG_TO_RAD;
      orbitalElements.trueAnomaly         = ta;

      // Add to the database
      g_database[name] = orbitalElements;

      recordsWritten++;
   }

   OTL_INFO() << "Successfully loaded [" << recordsWritten << "] records from mpcorb database file [" << m_dataFilename << "].";
}

////////////////////////////////////////////////////////////
void MpcorbEphemerisIO::GetOrbitalElements(const std::string& name, OrbitalElements& orbitalElements)
{
   if (!m_initialized)
   {
      Initialize();
   }

   if (IsNameValid(name))
   {
      orbitalElements = g_database[name];
   }
   else
   {
      OTL_ERROR() << "Name [" << name << "] not found";
   }
}

bool MpcorbEphemerisIO::IsNameValid(const std::string& name) const
{
   auto it = g_database.find(name);
   return (it != g_database.end());
}

} // namespace otl