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
#include <OTL/Core/Epoch.h>

namespace otl
{

// Forward declarations
namespace keplerian {
class IPropagateAlgorithm;
typedef std::shared_ptr<IPropagateAlgorithm> PropagatorPointer;
}

class MpcorbEphemeris : public IEphemeris
{
public:
   MpcorbEphemeris(const std::string& dataFilename);
   virtual ~MpcorbEphemeris();
   void LoadDataFile(const std::string& filename);
   void SetDataFile(const std::string& dataFile);
   void SetPropagator(const keplerian::PropagatorPointer& propagator);

protected:
   virtual void VLoad();
   virtual void VInitialize();
   virtual bool VIsNameValid(const std::string& name);
   virtual bool VIsEpochValid(const Epoch& epoch);
   virtual void VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position);
   virtual void VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity);
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector);
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements);

private:
   std::string m_dataFilename;
   keplerian::PropagatorPointer m_propagator;
   Epoch m_referenceEpoch;                      ///< Temporary variable for retrieving reference epoch
   StateVector m_referenceStateVector;          ///< Temporary variable for retrieving reference state vector
   OrbitalElements m_referenceOrbitalElements;  ///< Temporary variable for retrieving reference orbital elements
};

} // namespace otl