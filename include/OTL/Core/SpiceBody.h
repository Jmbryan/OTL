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
#include <OTL/Core/SpiceEphemeris.h>

namespace otl
{

//// Foreward declarations
//class SpiceEphemeris;
//typedef std::shared_ptr<SpiceEphemeris> SpiceEphemerisPointer;

class OTL_CORE_API SpiceBody : public OrbitalBody
{
public:
   SpiceBody();
   explicit SpiceBody(const std::string& observerBodyName,
                      const Epoch& epoch = Epoch(),
                      const std::string& targetBodyName = "SUN",
                      const std::string& referenceFrameName = "J2000");
   SpiceBody(const std::string& observerBodyName,
             const SpiceEphemeris& ephemeris,
             const Epoch& epoch = Epoch(),
             const std::string& targetBodyName = "SUN",
             const std::string& referenceFrameName = "J2000");

   void SetEphemeris(const SpiceEphemeris& ephemeris);

   void LoadKernal(const std::string& filename);

   virtual std::string ToString(const std::string& prefix = "") const override;

protected:
   virtual void VInitialize() override;
   virtual void VPropagateTo(const Epoch& epoch) override;
   //virtual EphemerisPointer VGetEphemeris() override;
   //virtual StateVector VQueryStateVector(const Epoch& epoch) override;

private:
   CartesianStateVector QueryCartesianStateVectorAt(const Epoch& epoch);

private:
   SpiceEphemeris m_ephemeris;
};

} // namespace otl