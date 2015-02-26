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
#include <OTL/Core/LagrangianPropagator.h>

namespace otl
{

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody() :
OrbitalBody()
{

}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& observerBodyName,
                     const Epoch& epoch,
                     const std::string& targetBodyName,
                     const std::string& referenceFrameName) :
OrbitalBody(observerBodyName, epoch)
{

}

////////////////////////////////////////////////////////////
SpiceBody::SpiceBody(const std::string& observerBodyName,
                     const SpiceEphemeris& ephemeris,
                     const Epoch& epoch,
                     const std::string& targetBodyName,
                     const std::string& referenceFrameName) :
OrbitalBody(observerBodyName, epoch),
m_ephemeris(ephemeris)
{

}

////////////////////////////////////////////////////////////
void SpiceBody::SetEphemeris(const SpiceEphemeris& ephemeris)
{
   m_ephemeris = ephemeris;
}

////////////////////////////////////////////////////////////
void SpiceBody::LoadKernal(const std::string& filename)
{
   m_ephemeris.LoadDataFile(filename);
}

////////////////////////////////////////////////////////////
std::string SpiceBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToString(prefix + "   ");

   return os.str();
}

////////////////////////////////////////////////////////////
void SpiceBody::VInitialize()
{
   const auto& name = GetName();
   const auto& epoch = GetEpoch();
   
   // Init the physical properties
   m_physicalProperties = m_ephemeris.GetPhysicalProperties(name);

   // Init the orbit
   double mu = m_ephemeris.GetGravitationalParameterCentralBody(name);
   auto cartesianStateVector = QueryCartesianStateVectorAt(epoch);
   m_orbit = keplerian::Orbit(mu, cartesianStateVector);
}

////////////////////////////////////////////////////////////
void SpiceBody::VPropagateTo(const Epoch& epoch)
{
   m_orbit.SetCartesianStateVector(QueryCartesianStateVectorAt(epoch));
}

////////////////////////////////////////////////////////////
CartesianStateVector SpiceBody::QueryCartesianStateVectorAt(const Epoch& epoch)
{
   return m_ephemeris.GetCartesianStateVector(GetName(), epoch);
}

////////////////////////////////////////////////////////////
//EphemerisPointer SpiceBody::VGetEphemeris()
//{
//   return std::dynamic_pointer_cast<IEphemeris>(m_ephemeris);
//}

////////////////////////////////////////////////////////////
//StateVector SpiceBody::VQueryStateVector(const Epoch& epoch)
//{
//   if (m_ephemeris)
//   {
//      return m_ephemeris->GetStateVector(GetName(), epoch);
//   }
//   else
//   {
//      OTL_ERROR() << "Failed to query state vector for spice body " << Bracket(GetName())
//         << ": Invalid ephemeris pointer.";
//      return StateVector();
//   }
//}

} // namespace otl