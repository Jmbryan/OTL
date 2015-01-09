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
#include <OTL/Core/Exceptions.h>

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
void JplApproximateEphemeris::LoadDataFile(const std::string& filename)
{
   SetDataFile(filename);
   g_ephemerisDatabase->SetDataFile(m_dataFilename);
   g_ephemerisDatabase->Load();
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::SetDataFile(const std::string& filename)
{
   m_dataFilename = filename;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VLoad()
{
   g_ephemerisDatabase.reset(new JplApproximateEphemerisIO(m_dataFilename));
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VInitialize()
{
   g_ephemerisDatabase->Initialize();
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemeris::VIsNameValid(const std::string& name)
{
   return g_ephemerisDatabase->IsNameValid(name);
}

////////////////////////////////////////////////////////////
bool JplApproximateEphemeris::VIsEpochValid(const Epoch& epoch)
{
   return g_ephemerisDatabase->IsEpochValid(epoch);
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VQueryDatabase(name, epoch, stateVector);

   // Return the position vector
   position = stateVector.position;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VQueryDatabase(name, epoch, stateVector);

   // Return the velocity vector
   velocity = stateVector.velocity;
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
    // Get the orbital elements at the given epoch
    OrbitalElements orbitalElements;
    VQueryDatabase(name, epoch, orbitalElements);

    // Convert orbital elements to state vector
    ConvertOrbitalElements2StateVector(orbitalElements, stateVector, ASTRO_MU_SUN);
}

////////////////////////////////////////////////////////////
void JplApproximateEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   try
   {
      g_ephemerisDatabase->GetOrbitalElements(name, epoch, orbitalElements);
   }
   catch (Exception ex)
   {
      OTL_ERROR() << "Exception caught while trying to retrieve orbital elements for " << Bracket(name) <<
         " at epoch " << Bracket(epoch) << ": " << Bracket(ex.what());
   }
}

} // namespace otl