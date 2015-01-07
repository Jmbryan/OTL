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

#include <OTL/Core/MpcorbEphemeris.h>
#include <OTL/Core/MpcorbEphemerisIO.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>

namespace otl
{

   // Static globals
   //typedef std::unique_ptr<DE405Ephemeris> EphemerisDatabasePointer;
   //static EphemerisDatabasePointer g_ephemerisDatabase;

   //typedef std::map<std::string, DE405Ephemeris::AstroEntity> AstroEntityDictionary;
   //static AstroEntityDictionary g_entityDictionary;

   ////////////////////////////////////////////////////////////
   MpcorbEphemeris::MpcorbEphemeris(const std::string& dataFile) :
      IEphemeris(),
      m_dataFile(dataFile)
   {

   }

   ////////////////////////////////////////////////////////////
   MpcorbEphemeris::~MpcorbEphemeris()
   {

   }

   ////////////////////////////////////////////////////////////
   void MpcorbEphemeris::SetDataFile(const std::string& dataFile)
   {
      m_dataFile = dataFile;
   }

   ////////////////////////////////////////////////////////////
   void MpcorbEphemeris::VLoad()
   {
      try
      {
         //g_ephemerisDatabase.reset(new DE405Ephemeris(m_dataFile));
      }
      catch (std::exception ex)
      {
         OTL_FATAL() << "Exception caught when trying to create ephemeris database using datafile [" << m_dataFile << "]: [" << ex.what() << "].";
      }
   }

   ////////////////////////////////////////////////////////////
   void MpcorbEphemeris::VInitialize()
   {

   }

   ////////////////////////////////////////////////////////////
   bool MpcorbEphemeris::VIsNameValid(const std::string& name)
   {
      return true;
      //AstroEntityDictionary::const_iterator it = g_entityDictionary.find(name);
      //return (it != g_entityDictionary.end());
   }

   ////////////////////////////////////////////////////////////
   bool MpcorbEphemeris::VIsEpochValid(const Epoch& epoch)
   {
      int year = epoch.GetGregorian().year;
      return (year >= 1800 && year <= 2200);
   }

   ////////////////////////////////////////////////////////////
   void MpcorbEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
   {
      //DE405Ephemeris::AstroEntity entity = g_entityDictionary[name];

      double pos[3], vel[3];
      //g_ephemerisDatabase->getPosVel(epoch.GetJD(), entity, pos, vel);

      for (int i = 0; i < 3; ++i)
      {
         stateVector.position[i] = pos[i];
         stateVector.velocity[i] = vel[i] / MATH_DAY_TO_SEC;
      }
   }

   ////////////////////////////////////////////////////////////
   void MpcorbEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
   {
      // Query the state vector at the given epoch
      StateVector stateVector;
      VQueryDatabase(name, epoch, stateVector);

      // Convert state vector to orbital elements
      ConvertStateVector2OrbitalElements(stateVector, orbitalElements, ASTRO_MU_SUN);
   }

} // namespace otl