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
#include <OTL/Core/EphemerisBody.h>

namespace otl
{

// Forward declarations
class MpcorbEphemeris;
typedef std::shared_ptr<MpcorbEphemeris> MpcorbEphemerisPointer;

class OTL_CORE_API MpcorbBody : public IEphemerisBody
{
public:
   MpcorbBody();
   explicit MpcorbBody(const std::string& name,
                       const Epoch& epoch = Epoch::MJD2000(0.0));
   MpcorbBody(const std::string& name,
              const MpcorbEphemerisPointer& ephemeris,
              const Epoch& epoch = Epoch::MJD2000(0.0));

   void SetEphemeris(const MpcorbEphemerisPointer& ephemeris);

protected:
   virtual void VInitialize() override;
   virtual test::StateVector VQueryStateVectorr(const Epoch& epoch) override;

private:
   MpcorbEphemerisPointer m_ephemeris;
   Epoch m_referenceEpoch;
   test::StateVector m_referenceStateVector;
};

} // namespace otl