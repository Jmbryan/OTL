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
#include <OTL/Core/OrbitalBody.h>

namespace otl
{

class IEphemerisBody : public OrbitalBody
{
public:
   IEphemerisBody();
   IEphemerisBody(const std::string& name,
                  const PhysicalProperties& physicalProperties,
                  double gravitationalParameterCentralBody,
                  const test::StateVector& stateVector,
                  const Epoch& epoch = Epoch::MJD2000(0.0));

   virtual const PhysicalProperties& GetPhysicalProperties() const override;
   virtual const keplerian::Orbit& GetOrbit() const override;

   //const PhysicalProperties& QueryPhysicalProperties();
   //double QueryGravitationalParameterCentralBody();
   const test::StateVector& QueryStateVector(const Epoch& epoch);

   void ForceInitialize();

protected:
   virtual void VPropagate(const Time& timeDelta) override;
   virtual void VInitialize() = 0;
   virtual test::StateVector VQueryStateVectorr(const Epoch& epoch) = 0;

private:
   mutable bool m_initialized;   ///< TRUE if the ephemeris data has been initialized
   Time m_maxPropagationTime;    ///< Max propagation time before next ephemeris update
};

} // namespace otl