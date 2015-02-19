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
#include <OTL/Core/Base.h>

namespace otl
{

// Forward declarations
class Epoch;
namespace keplerian {
   class IKeplersEquation;
   typedef std::shared_ptr<IKeplersEquation> KeplersEquationPointer;
}
class PhysicalProperties;

class JplApproximateEphemerisIO
{
public:
   JplApproximateEphemerisIO();
   explicit JplApproximateEphemerisIO(const std::string& dataFilename);
   JplApproximateEphemerisIO(const JplApproximateEphemerisIO& other) = delete;
   JplApproximateEphemerisIO& operator=(const JplApproximateEphemerisIO&) = delete;

   StateVector GetStateVector(const std::string& name, const Epoch& epoch);

   bool IsValidName(const std::string& name) const;
   bool IsValidEpoch(const Epoch& epoch) const;

   void Initialize();

private:
   void Load();

private:
   std::string m_dataFilename;
   int m_startYear;
   int m_endYear;
   keplerian::KeplersEquationPointer m_keplersEquation; ///< Keplers equation used to convert mean anomaly to eccentric anomaly 
};

} // namespace otl