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

#include <OTL/Core/JplApproximateEphemeris.h>
#include <OTL/Core/Jpl/JplApproximateEphemerisIO.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Logger.h>

#include <OTL/Core/OrbitalBody.h> // for PhysicalProperties

namespace otl
{

// Static globals
typedef std::unique_ptr<JplApproximateEphemerisIO> EphemerisDatabasePointer;
static EphemerisDatabasePointer g_ephemerisDatabase;

////////////////////////////////////////////////////////////
JplApproximateEphemeris::JplApproximateEphemeris() :
IEphemeris()
{

}

////////////////////////////////////////////////////////////
JplApproximateEphemeris::JplApproximateEphemeris(const std::string& dataFilename) :
IEphemeris(),
m_dataFilename(dataFilename)
{

}

////////////////////////////////////////////////////////////
JplApproximateEphemeris::~JplApproximateEphemeris()
{

}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::SetDataFile(const std::string& dataFilename)
{
   m_dataFilename = dataFilename;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::LoadDataFile(const std::string& dataFilename)
{
   SetDataFile(dataFilename);
   VLoad();
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VLoad()
{
   // make_unique not supported in c++11
   g_ephemerisDatabase = std::unique_ptr<JplApproximateEphemerisIO>(new JplApproximateEphemerisIO(m_dataFilename));
   try
   {
      g_ephemerisDatabase->Initialize();
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught while trying to load ephemeris datafile " <<
         Bracket(m_dataFilename) << ": " << Bracket(ex.what());
   } 
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VInitialize()
{
   
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemeris::VIsValidName(const std::string& name)
{
   return g_ephemerisDatabase->IsValidName(name);
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemeris::VIsValidEpoch(const Epoch& epoch)
{
   return g_ephemerisDatabase->IsValidEpoch(epoch);
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VGetStateVector(name, epoch, stateVector);

   // Return the position vector
   position = stateVector.position;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VGetStateVector(name, epoch, stateVector);

   // Return the velocity vector
   velocity = stateVector.velocity;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
    // Get the orbital elements at the given epoch
    OrbitalElements orbitalElements;
    VGetOrbitalElements(name, epoch, orbitalElements);

    // Convert orbital elements to state vector
    stateVector = ConvertOrbitalElements2StateVector(orbitalElements, ASTRO_MU_SUN);
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   try
   {
      g_ephemerisDatabase->GetOrbitalElements(name, epoch, orbitalElements);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve orbital elements for " << Bracket(name) <<
         " at epoch " << Bracket(epoch) << ": " << Bracket(ex.what());
   }
}

////////////////////////////////////////////////////////////
//PhysicalProperties JplApproximateEphemeris::VGetPhysicalProperties(const std::string& name)
//{
//   try
//   {
//      return g_ephemerisDatabase->GetPhysicalProperties(name);
//   }
//   catch (std::exception ex)
//   {
//      OTL_ERROR() << "Exception caught while trying to retrieve physical properties for "
//         << Bracket(name) << ": " << Bracket(ex.what());
//      return PhysicalProperties();
//   }
//}

void JplApproximateEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector)
{
   try
   {
      g_ephemerisDatabase->GetStateVector(name, epoch, stateVector);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve state vector for " << Bracket(name) <<
         " at epoch " << Bracket(epoch) << ": " << Bracket(ex.what());
   }
}

} // namespace otl