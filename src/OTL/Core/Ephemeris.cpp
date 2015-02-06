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
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Logger.h>

#include <OTL/Core/OrbitalBody.h> // for PhysicalProperties

namespace otl
{

////////////////////////////////////////////////////////////
IEphemeris::IEphemeris() :
m_initialized(false)
{

}

////////////////////////////////////////////////////////////
IEphemeris::~IEphemeris()
{

}

////////////////////////////////////////////////////////////
void IEphemeris::GetPosition(const std::string& name, const Epoch& epoch, Vector3d& position)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (VIsValidName(name))
   {
      if (VIsValidEpoch(epoch))
      {
         VGetPosition(name, epoch, position);
      }
      else
      {
         OTL_ERROR() << "Epoch " << Bracket(epoch) << " is outside the accepted range";
      }
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";
   }
}

////////////////////////////////////////////////////////////
void IEphemeris::GetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (VIsValidName(name))
   {
      if (VIsValidEpoch(epoch))
      {
         VGetVelocity(name, epoch, velocity);
      }
      else
      {
         OTL_ERROR() << "Epoch " << Bracket(epoch) << " is outside the accepted range";
      }
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";
   }
}

////////////////////////////////////////////////////////////
void IEphemeris::GetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (VIsValidName(name))
   {
      if (VIsValidEpoch(epoch))
      {
         VGetStateVector(name, epoch, stateVector);
      }
      else
      {
          OTL_ERROR() << "Epoch " << Bracket(epoch) << " is outside the accepted range";
      }
   }
   else
   {
      OTL_ERROR() << "Name " << Bracket(name) << " not found";
   }
}

////////////////////////////////////////////////////////////
void IEphemeris::GetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (VIsValidName(name))
   {
      if (VIsValidEpoch(epoch))
      {
         VGetOrbitalElements(name, epoch, orbitalElements);
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
}

//////////////////////////////////////////////////////////////
//PhysicalProperties IEphemeris::GetPhysicalProperties(const std::string& name)
//{
//   std::lock_guard<std::mutex> lock(m_mutex);
//
//   if (!m_initialized)
//   {
//      Initialize();
//   }
//
//   if (VIsValidName(name))
//   {
//      return VGetPhysicalProperties(name);
//   }
//   else
//   {
//      OTL_ERROR() << "Name " << Bracket(name) << " not found";
//      return PhysicalProperties();
//   }
//}
//
//PhysicalProperties IEphemeris::VGetPhysicalProperties(const std::string& name)
//{
//   return PhysicalProperties();
//}

////////////////////////////////////////////////////////////
test::StateVector IEphemeris::GetStateVector(const std::string& name, const Epoch& epoch)
{
   test::StateVector stateVector;
   GetStateVector(name, epoch, stateVector);
   return stateVector;
}

////////////////////////////////////////////////////////////
void IEphemeris::GetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector)
{
   std::lock_guard<std::mutex> lock(m_mutex);

   if (!m_initialized)
   {
      Initialize();
   }

   if (VIsValidName(name))
   {
      if (VIsValidEpoch(epoch))
      {
         VGetStateVector(name, epoch, stateVector);
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
}

////////////////////////////////////////////////////////////
void IEphemeris::Initialize()
{
    VLoad();
    VInitialize();
    m_initialized = true;
}

} // namespace otl