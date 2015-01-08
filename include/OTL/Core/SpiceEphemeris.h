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

#pragma once
#include <OTL/Core/Ephemeris.h>

namespace otl
{

class SpiceEphemeris : public IEphemeris
{
public:
   SpiceEphemeris(const std::string& dataFile);
   virtual ~SpiceEphemeris();
   void SetDataFile(const std::string& dataFile);
   void SetReferenceFrame(const std::string& referenceFrame);
   void SetAbberationCorrections(const std::string& abberationCorrections);
   void SetObserverBody(const std::string& observerBody);

protected:
   virtual void VLoad();
   virtual void VInitialize();
   virtual bool VIsNameValid(const std::string& name);
   virtual bool VIsEpochValid(const Epoch& epoch);
   virtual void GetPosition(const std::string& name, const Epoch& epoch, Vector3d& position);
   virtual void GetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity);
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector);
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements);

private:
   double CalculateEphemerisTime(const Epoch& epoch) const;

private:
   std::string m_dataFile;
   std::string m_referenceFrame;
   std::string m_abberationCorrections;
   std::string m_observerBody;
};

} // namespace otl