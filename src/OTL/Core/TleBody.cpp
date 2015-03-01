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
#include <OTL/Core/TleBody.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Tle.h>

namespace otl
{
TleBody::TleBody() :
OrbitalBody()
{

}

TleBody::TleBody(const std::string& line1, const std::string& line2) :
OrbitalBody("Unknown", Epoch()),
m_line1(line1),
m_line2(line2)
{

}

std::string TleBody::ToString(const std::string& prefix) const
{
   std::ostringstream os;
   os << prefix << "Line 1: " << m_line1 << std::endl;
   os << prefix << "Line 2: " << m_line2 << std::endl;
   os << prefix << "Reference Epoch: " << std::endl;
   os << m_referenceEpoch.ToDetailedString("   ");
   os << prefix << "Orbital Body:" << std::endl;
   os << OrbitalBody::ToString(prefix + "   ");

   return os.str();
}

void TleBody::VInitialize()
{
   try
   {
      m_tle = std::make_shared<Tle>("TLE Body", m_line1, m_line2);
      m_sgp4 = std::make_shared<SGP4>(*m_tle);

      // Init the name
      std::string name = m_tle->Name();
      SetName(name);

      // Init the epoch
      m_referenceEpoch = Epoch::JD(m_tle->Epoch().ToJulian());
      SetEpoch(m_referenceEpoch);

      // Init the orbital elements

      // Convert Mean Motion from rev/day to rad/s
      double n = m_tle->MeanMotion() * 2.0 * kPI / kSECONDS_PER_DAY;

      // Compute semimajor axis
      double a = pow(kMU / SQR(n), 1.0 / 3.0);

      OrbitalElements coes;
      coes.semiMajorAxis      = a;
      coes.eccentricity       = m_tle->Eccentricity();
      coes.meanAnomaly        = m_tle->MeanAnomaly(false);
      coes.inclination        = m_tle->Inclination(false);
      coes.argOfPericenter    = m_tle->ArgumentPerigee(false);
      coes.lonOfAscendingNode = m_tle->RightAscendingNode(false);
      SetOrbitalElements(coes);
   }
   catch (TleException& e)
   {
      OTL_ERROR() << "TleException caught while initializing TleBody: " << e.what();
   }
   catch (SatelliteException& e)
   {
      OTL_ERROR() << "SatelliteException caught while initializing TleBody: " << e.what();
   }
}

void TleBody::VPropagateTo(const Epoch& epoch)
{
   try
   {
      auto timeDelta = epoch - m_referenceEpoch;
      Eci eci = m_sgp4->FindPosition(timeDelta.Minutes());

      Vector position = eci.Position();
      Vector velocity = eci.Velocity();

      CartesianStateVector cartesianStateVector(
         position.x,
         position.y,
         position.z,
         velocity.x,
         velocity.y,
         velocity.z);

      SetCartesianStateVector(cartesianStateVector);
   }
   catch (SatelliteException& e)
   {
      OTL_ERROR() << "SatelliteException caught while propagating TleBody: " << e.what();
   }
   catch (DecayedException& e)
   {
      OTL_ERROR() << "DecayedException caught while propagating TleBody: " << e.what();
   }
}

} // namespace otl