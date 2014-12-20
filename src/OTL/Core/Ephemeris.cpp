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

namespace otl
{

IEphemeris::IEphemeris() :
m_initialized(false)
{

}

IEphemeris::~IEphemeris()
{

}

void IEphemeris::QueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (IsNameValid(name))
   {
      VQueryDatabase(name, epoch, stateVector);
   }
   else
   {
      //throw InvalidArgumentException("Name not found");
   }
}

void IEphemeris::QueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (IsNameValid(name))
   {
      VQueryDatabase(name, epoch, orbitalElements);
   }
   else
   {
      //throw InvalidArgumentException("Name not found");
   }
}

EphemerisPointer IEphemeris::GetInstance()
{
   return instance;
}

void IEphemeris::Initialize()
{
    VInitialize();
    m_initialized = true;
}

bool IEphemeris::IsNameValid(const std::string& name)
{
   return VIsNameValid(name);
}

} // namespace otl