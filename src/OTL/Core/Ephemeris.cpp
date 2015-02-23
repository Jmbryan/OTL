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

#include <OTL/Core/Ephemeris.h>
#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/StateVector.h>
#include <OTL/Core/Logger.h>

namespace otl
{

////////////////////////////////////////////////////////////
IEphemeris::IEphemeris() :
m_initialized(false),
m_dataFilename("")
{

}

////////////////////////////////////////////////////////////
IEphemeris::IEphemeris(const std::string& dataFilename) :
m_initialized(false),
m_dataFilename(dataFilename)
{

}

////////////////////////////////////////////////////////////
IEphemeris::~IEphemeris()
{

}

////////////////////////////////////////////////////////////
void IEphemeris::LoadDataFile(const std::string& dataFilename)
{
   m_dataFilename = dataFilename;
   Initialize();
}

////////////////////////////////////////////////////////////
void IEphemeris::SetDataFilename(const std::string& dataFilename)
{
   m_dataFilename = dataFilename;
   m_initialized = false;
}

////////////////////////////////////////////////////////////
const std::string& IEphemeris::GetDataFilename() const
{
   return m_dataFilename;
}

////////////////////////////////////////////////////////////
PhysicalProperties IEphemeris::GetPhysicalProperties(const std::string& name)
{
   if (!m_initialized)
   {
      Initialize();
   }

   if (IsValidName(name))
   {
      return VGetPhysicalProperties(name);
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";    
   }

   return PhysicalProperties();
}

////////////////////////////////////////////////////////////
double IEphemeris::GetGravitationalParameterCentralBody(const std::string& name)
{
   if (!m_initialized)
   {
      Initialize();
   }

   if (IsValidName(name))
   {
      return VGetGravitationalParameterCentralBody(name);
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";     
   }

   return 0.0;
}

////////////////////////////////////////////////////////////
StateVector IEphemeris::GetStateVector(const std::string& name, const Epoch& epoch)
{
   if (!m_initialized)
   {
      Initialize();
   }

   if (IsValidName(name))
   {
      if (IsValidEpoch(epoch))
      {
         return VGetStateVector(name, epoch);
      }
      else
      {
         OTL_ERROR() << "Epoch is outside the accepted range";
      }
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";
   }
   return StateVector();
}

////////////////////////////////////////////////////////////
bool IEphemeris::IsValidName(const std::string& name)
{
   if (name == m_cachedName)
   {
      return true;
   }
   else if (VIsValidName(name))
   {
      m_cachedName = name;
      return true;
   }
   return false;
}

////////////////////////////////////////////////////////////
bool IEphemeris::IsValidEpoch(const Epoch& epoch)
{
   if (epoch == m_cachedEpoch)
   {
      return true;
   }
   else if (VIsValidEpoch(epoch))
   {
      m_cachedEpoch = epoch;
      return true;
   }
   return false;
}

////////////////////////////////////////////////////////////
void IEphemeris::Initialize()
{
    VLoad();
    VInitialize();
    m_initialized = true;
}

} // namespace otl