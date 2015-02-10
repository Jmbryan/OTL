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

#include <OTL/Core/SpiceBody.h>
#include <OTL/Core/SpiceEphemeris.h>

namespace otl
{

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody() :
OrbitalBody(),
m_spiceEphemeris()
{

}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& name,
                     const std::string& dataFilename,
                     const Epoch& epoch) :
OrbitalBody(name, PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_spiceEphemeris(std::make_shared<SpiceEphemeris>(dataFilename))
{
   Initialize();
}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& name,
                     const SpiceEphemerisPointer& spiceEphemeris,
                     const Epoch& epoch) :
OrbitalBody(name, PhysicalProperties(), 1.0, test::StateVector(), epoch),
m_spiceEphemeris(spiceEphemeris)
{
   Initialize();
}

////////////////////////////////////////////////////////////
void SpiceBody::Initialize()
{
   // Initialize ephemeris
   SetEphemeris(m_spiceEphemeris);

   // Lazily query the body data from the ephemeris database
   LazyQueryPhysicalProperties();
   LazyQueryCentralBodyMu();
   LazyQueryStateVector(GetEpoch());
}

} // namespace otl