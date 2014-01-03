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

#include <OTL/Core/Epoch.hpp>

namespace otl
{

void Epoch::Set(double epoch, Epoch::Type type)
{
   switch (type)
   {
   case Type::MJD2000:
      m_mjd2000 = epoch;
      break;

   case Type::MJD:
      m_mjd2000 = ConvertMJD2MJD2000(epoch);
      break;

   case Type::JD:
      m_mjd2000 = ConvertJD2MJD2000(epoch);
      break;
   }
}

inline void Epoch::SetGregorian(const GregorianDateTime& date)
{
   double julianDate = 367 * date.year -
                       Round0(7 * (date.year + Round0((date.month + 9) / 12)) / 4) +
                       Round0(275 * date.month / 9) +
                       date.day + 1721013.5 +
                       date.hour / 24 +
                       (date.min/ 60 / 24) +
                       date.sec / 3600 / 24;
   SetJD(julianDate);
}

double Epoch::Get(Epoch::Type type) const
{
   double epoch = 0.0;

   switch (type)
   {
   case Type::MJD2000:
      epoch = m_mjd2000;
      break;

   case Type::MJD:
      epoch = ConvertMJD20002MJD(m_mjd2000);
      break;

   case Type::JD:
      epoch = ConvertMJD20002JD(m_mjd2000);
      break;
   }

   return epoch;
}

} // namespace otl
