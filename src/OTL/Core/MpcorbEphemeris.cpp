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
#include <OTL/Core/Mpcorb/MpcorbEphemerisIO.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>
#include <OTL/Core/Exceptions.h>

#include <OTL/Core/PropagateLagrangian.h>

namespace otl
{

// Static globals
typedef std::unique_ptr<MpcorbEphemerisIO> EphemerisDatabasePointer;
static EphemerisDatabasePointer g_ephemerisDatabase;

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
void MpcorbEphemeris::SetPropagator(const keplerian::PropagatorPointer& propagator)
{
   m_propagator = propagator;
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VLoad()
{
   try
   {
      g_ephemerisDatabase.reset(new MpcorbEphemerisIO(m_dataFile));
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught when trying to create ephemeris database using datafile [" << m_dataFile << "]: [" << ex.what() << "].";
   }
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VInitialize()
{
   m_propagator.reset(new keplerian::PropagateLagrangian());
}

////////////////////////////////////////////////////////////
bool MpcorbEphemeris::VIsNameValid(const std::string& name)
{
   return g_ephemerisDatabase->IsNameValid(name);
}

////////////////////////////////////////////////////////////
bool MpcorbEphemeris::VIsEpochValid(const Epoch& epoch)
{
   return g_ephemerisDatabase->IsEpochValid(epoch);
}


////////////////////////////////////////////////////////////
void MpcorbEphemeris::GetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VQueryDatabase(name, epoch, stateVector);

   // Return the position vector
   position = stateVector.position;
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::GetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   // Get the state vector at the given epoch
   StateVector stateVector;
   VQueryDatabase(name, epoch, stateVector);

   // Return the velocity vector
   velocity = stateVector.velocity;
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   // Get the reference epoch and state vector
   try
   {
      g_ephemerisDatabase->GetEpoch(name, m_referenceEpoch);
      g_ephemerisDatabase->GetStateVector(name, m_referenceStateVector);
   }
   catch (Exception ex)
   {
      OTL_ERROR() << "Exception caught while retrieving reference epoch and state vector for " << Bracket(name);
      return;
   }

   // Time since the reference epoch
   auto timeDelta = Time::Days(epoch.GetJD() - m_referenceEpoch.GetJD());
   
   // Propagate the state vector to the desired epoch
   if (m_propagator)
   {
      m_propagator->Propagate(m_referenceStateVector, ASTRO_MU_SUN, timeDelta, stateVector);
   }
   else
   {
      OTL_ERROR() << "Invalid propagator pointer";
   }
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   // Get the reference epoch and orbital elements
   try
   {
      g_ephemerisDatabase->GetEpoch(name, m_referenceEpoch);
      g_ephemerisDatabase->GetOrbitalElements(name, m_referenceOrbitalElements);
   }
   catch (Exception ex)
   {
      OTL_ERROR() << "Exception caught while retrieving reference epoch and orbital elements for " << Bracket(name);
      return;
   }

   // Time since the reference epoch
   auto timeDelta = Time::Days(epoch.GetJD() - m_referenceEpoch.GetJD());

   // Propagate the state vector to the desired epoch
   if (m_propagator)
   {
      m_propagator->Propagate(m_referenceOrbitalElements, ASTRO_MU_SUN, timeDelta, orbitalElements);
   }
   else
   {
      OTL_ERROR() << "Invalid propagator pointer";
   }
}

} // namespace otl