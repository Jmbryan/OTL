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
#include <OTL/Core/Base.h>
#include <OTL/Core/KeplersEquations.h>
#include <OTL/Core/Conversion.h>

#include <iostream>
#include <sstream>
#include <map>

namespace otl
{

std::map<std::string, OrbitalElements> g_database;

MpcorbEphemerisIO::MpcorbEphemerisIO(const std::string& dataFilename) :
   m_dataFilename(dataFilename)
{

}

void MpcorbEphemerisIO::Initialize()
{
   //m_dataStream.open(m_dataFilename);

   //m_dataStream.read()

   //std::string outputFilename(currentDirectory + "\\..\\data\\mpcorb\\mpcorb.dat");
   //std::ofstream ofs(outputFilename, std::ios::binary | std::ios::trunc);
   //if (!ofs)
   //{
   //   std::cerr << "Error: unable to open output file '" << outputFilename << "'" << std::endl;
   //   return;
   //}

   std::ifstream ifs(m_dataFilename);
   if (!ifs)
   {
      std::cerr << "Error: unable to open input file '" << m_dataFilename << "'" << std::endl;
      return;
   }

   int index;
   double bufD;
   int recordsWritten = 0;
   while (ifs >> index)
   {
      double H, G, M, peri, node, incl, e, n, a;
      std::string epoch;
      ifs >> H >> G >> epoch >> M >> peri >> node >> incl >> e >> n >> a;

      int ref, opp, arc;
      std::string obs, rms;
      double perts;
      ifs >> ref >> obs >> opp >> arc >> rms >> perts;

      int tempInt;
      std::string tempString;
      ifs >> tempString >> tempString >> tempString >> tempInt;

      char tempChar;
      int id;
      ifs >> tempChar >> id >> tempChar;

      std::string name;
      ifs >> name;

      ifs >> tempInt;

      // Convert mean anomaly to eccentric anomaly (radians)
      keplerian::KeplersEquationElliptical kepler;
      double E = kepler.Evaluate(e, M * MATH_DEG_TO_RAD);

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

   std::cout << "Wrote " << recordsWritten << " records from " << m_dataFilename << std::endl;
}


} // namespace otl