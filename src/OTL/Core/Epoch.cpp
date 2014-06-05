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

////////////////////////////////////////////////////////////
Epoch::Epoch() :
m_mjd2000(0.0)
{

}

////////////////////////////////////////////////////////////
Epoch Epoch::JD(double julianDate)
{
   Epoch epoch;
   epoch.SetJD(julianDate);
   return epoch;
}

////////////////////////////////////////////////////////////
Epoch Epoch::MJD(double modifiedJulianDate)
{
   Epoch epoch;
   epoch.SetMJD(modifiedJulianDate);
   return epoch;
}

////////////////////////////////////////////////////////////
Epoch Epoch::MJD2000(double modifiedJulianDate2000)
{
   Epoch epoch;
   epoch.SetMJD2000(modifiedJulianDate2000);
   return epoch;
}

////////////////////////////////////////////////////////////
Epoch Epoch::Gregorian(const GregorianDateTime& dateTime)
{
   Epoch epoch;
   epoch.SetGregorian(dateTime);
   return epoch;
}

////////////////////////////////////////////////////////////
double ConvertGregorian2JD(const GregorianDateTime& dateTime)
{
   double julianDate = 367.0 * dateTime.year -
                       Round0(7.0 * (dateTime.year + Round0((dateTime.month + 9.0) / 12.0)) / 4.0) +
                       Round0(275.0 * dateTime.month / 9.0) +
                       dateTime.day + 1721013.5 +
                       dateTime.hour / 24.0 +
                       (dateTime.min/ 60.0 / 24.0) +
                       dateTime.sec / 3600.0 / 24.0;
   return julianDate;
}

////////////////////////////////////////////////////////////
GregorianDateTime ConvertJD2Gregorian(double julianDate)
{
   static int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

   double T1900 = (julianDate - 2415019.5) / 365.25;
   int year = static_cast<int>(1900 + floor(T1900));
   int leapYears = static_cast<int>(floor((year - 1900 - 1) * 0.25));
   double days = (julianDate - 2415019.5) - ((year - 1900) * 365.0 + leapYears);
   if (days < 1.0)
   {
      year -= 1;
      leapYears = static_cast<int>(floor((year - 1900 - 1) * 0.25));
      days = (julianDate - 2415019.5) - ((year - 1900) * 365.0 + leapYears);
   }

   bool bLeapYear = false;
   if (year % 4 == 0)
   {
      bLeapYear = true;
   }

   int dayOfYear = static_cast<int>(floor(days));
   int month = 1;
   int daysSum = 0;
   while (month <= 12 && dayOfYear > (daysSum + daysPerMonth[month - 1]))
   {
      daysSum += daysPerMonth[month++ - 1];
      if (month == 2 && bLeapYear)
      {
         daysSum += 1;
      }
   }

   int day = dayOfYear - daysSum;

   double tau = (days - dayOfYear) * 24;
   int hour = static_cast<int>(floor(tau));
   int minute = static_cast<int>(floor((tau - hour) * 60.0));
   double seconds = (tau - hour - minute/60.0) * 3600.0;

   GregorianDateTime dateTime;
   dateTime.year = year;
   dateTime.month = month;
   dateTime.day = day;
   dateTime.hour = hour;
   dateTime.min = minute;
   dateTime.sec = seconds;

   return dateTime;
}

} // namespace otl
