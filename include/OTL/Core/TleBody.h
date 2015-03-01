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

// Forward declarations
class Tle;
class SGP4;
typedef std::shared_ptr<Tle> TlePointer;
typedef std::shared_ptr<SGP4> SGP4Pointer;

namespace otl
{

class OTL_CORE_API TleBody : public OrbitalBody
{
public:
   TleBody();
   TleBody(const std::string& line1, const std::string& line2);
   
   virtual std::string ToString(const std::string& prefix = "") const override;

protected:
   virtual void VInitialize() override;
   virtual void VPropagateTo(const Epoch& epoch) override;

private:
   std::string m_line1;
   std::string m_line2;
   TlePointer m_tle;
   SGP4Pointer m_sgp4;
   Epoch m_referenceEpoch;
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates TleBody
///
/// The body is converted to a string by calling the
/// Tle::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const TleBody& planet)
{
   stream << planet.ToString();
   return stream;
}

} // namespace otl