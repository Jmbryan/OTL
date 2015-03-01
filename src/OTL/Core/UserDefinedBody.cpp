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

#include <OTL/Core/UserDefinedBody.h>

namespace otl
{

////////////////////////////////////////////////////////////
UserDefinedBody::UserDefinedBody() :
OrbitalBody()
{

}

////////////////////////////////////////////////////////////
UserDefinedBody::UserDefinedBody(const std::string& name,
                                 const PhysicalProperties& physicalProperties,
                                 double gravitationalParameterCentralBody,
                                 const OrbitalElements& orbitalElements,
                                 const Epoch& epoch) :
OrbitalBody(name, physicalProperties, gravitationalParameterCentralBody, orbitalElements, epoch)
{
}

////////////////////////////////////////////////////////////
UserDefinedBody::UserDefinedBody(const std::string& name,
                                 const PhysicalProperties& physicalProperties,
                                 double gravitationalParameterCentralBody,
                                 const CartesianStateVector& cartesiantateVector,
                                 const Epoch& epoch) :
OrbitalBody(name, physicalProperties, gravitationalParameterCentralBody, cartesiantateVector, epoch)
{

}

////////////////////////////////////////////////////////////
std::string UserDefinedBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void UserDefinedBody::VInitialize()
{
   m_referenceEpoch = GetEpoch();
   m_referenceOrbitalElements = GetOrbitalElements();
}

////////////////////////////////////////////////////////////
void UserDefinedBody::VPropagateTo(const Epoch& epoch)
{
   m_orbit.SetOrbitalElements(m_referenceOrbitalElements);
   m_orbit.Propagate(epoch - m_referenceEpoch);
}

} // namespace otl