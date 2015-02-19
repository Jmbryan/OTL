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
#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Conversion.h>
#include <OTL/Core/Logger.h>

namespace otl
{

// Static globals
typedef std::unique_ptr<MpcorbEphemerisIO> EphemerisDatabasePointer;
static EphemerisDatabasePointer g_ephemerisDatabase;

////////////////////////////////////////////////////////////
MpcorbEphemeris::MpcorbEphemeris(const std::string& dataFilename) :
IEphemeris(dataFilename),
m_propagator(new keplerian::KeplerianPropagator())
{

}

////////////////////////////////////////////////////////////
MpcorbEphemeris::~MpcorbEphemeris()
{

}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::SetPropagator(const PropagatorPointer& propagator)
{
   m_propagator = propagator;
}

////////////////////////////////////////////////////////////
StateVector MpcorbEphemeris::GetReferenceStateVector(const std::string& name)
{
   try
   {    
      return g_ephemerisDatabase->GetStateVector(name);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while retrieving reference state vector for " << Bracket(name);
      return StateVector();
   }
}

////////////////////////////////////////////////////////////
Epoch MpcorbEphemeris::GetReferenceEpoch(const std::string& name)
{
   try
   {
      return g_ephemerisDatabase->GetEpoch(name);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while retrieving reference epoch for " << Bracket(name);
      return Epoch();
   }
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VLoad()
{
   // make_unique not supported in c++11
   g_ephemerisDatabase = std::unique_ptr<MpcorbEphemerisIO>(new MpcorbEphemerisIO(GetDataFilename()));
   try
   {
      g_ephemerisDatabase->Initialize();
   }
   catch (std::exception ex)
   {
      OTL_FATAL() << "Exception caught while trying to load ephemeris datafile " <<
         Bracket(GetDataFilename()) << ": " << Bracket(ex.what());
   }
}

////////////////////////////////////////////////////////////
void MpcorbEphemeris::VInitialize()
{
   
}

////////////////////////////////////////////////////////////
bool MpcorbEphemeris::VIsValidName(const std::string& name)
{
   return g_ephemerisDatabase->IsValidName(name);
}

////////////////////////////////////////////////////////////
bool MpcorbEphemeris::VIsValidEpoch(const Epoch& epoch)
{
   return g_ephemerisDatabase->IsValidEpoch(epoch);
}

////////////////////////////////////////////////////////////
PhysicalProperties MpcorbEphemeris::VGetPhysicalProperties(const std::string& name)
{
   try
   {
      return g_ephemerisDatabase->GetPhysicalProperties(name);
   }
   catch (std::exception ex)
   {
      OTL_ERROR() << "Exception caught while retrieving physical properties for " << Bracket(name);
   }

   return PhysicalProperties();
}

////////////////////////////////////////////////////////////
double MpcorbEphemeris::VGetGravitationalParameterCentralBody(const std::string& name)
{
   return ASTRO_MU_SUN;
}

////////////////////////////////////////////////////////////
StateVector MpcorbEphemeris::VGetStateVector(const std::string& name, const Epoch& epoch)
{
   // Get the reference epoch and state vector
   m_referenceEpoch = GetReferenceEpoch(name);
   m_referenceStateVector = GetReferenceStateVector(name);

   // Time since the reference epoch
   auto timeDelta = Time::Days(epoch.GetJD() - m_referenceEpoch.GetJD());

   // Propagate the state vector to the desired epoch
   if (m_propagator)
   {
      return m_propagator->Propagate(m_referenceStateVector, timeDelta, ASTRO_MU_SUN);
   }
   else
   {
      OTL_ERROR() << "Invalid propagator pointer";
   }

   return StateVector();
}

} // namespace otl