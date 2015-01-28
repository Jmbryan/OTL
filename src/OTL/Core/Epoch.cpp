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

#include <OTL/Core/Epoch.h>
#include <OTL/Core/Time.h>

namespace otl
{

// Static global initialization
static const std::vector<int> g_daysPerMonth = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const std::vector<std::string> g_monthNames = { "January", "February", "March", "April",
                                                       "May", "June", "July", "August",
                                                       "September", "October", "November", "December" };
static const std::vector<std::string> g_dayNames = { "Sunday", "Monday", "Tuesday", "Wednesday",
                                                     "Thursday", "Friday", "Saturday" };

////////////////////////////////////////////////////////////
GregorianDateTime::GregorianDateTime() :
year(),
month(),
day(),
hour(),
min(),
sec()
{

}

////////////////////////////////////////////////////////////
GregorianDateTime::GregorianDateTime(int _year, int _month, int _day) :
year(_year),
month(_month),
day(_day),
hour(),
min(),
sec()
{

}

////////////////////////////////////////////////////////////
GregorianDateTime::GregorianDateTime(int _year, int _month, int _day,
                                     int _hour, int _minute, double _second) :
year(_year),
month(_month),
day(_day),
hour(_hour),
min(_minute),
sec(_second)
{

}

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
void Epoch::SetJD(double julianDate)
{
   m_mjd2000 = ConvertJD2MJD2000(julianDate);
}

////////////////////////////////////////////////////////////
void Epoch::SetMJD(double modifiedJulianDate)
{
   m_mjd2000 = ConvertMJD2MJD2000(modifiedJulianDate);
}

////////////////////////////////////////////////////////////
void Epoch::SetMJD2000(double modifiedJulianDate2000)
{
   m_mjd2000 = modifiedJulianDate2000;
}

////////////////////////////////////////////////////////////
void Epoch::SetGregorian(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   SetJD(julianDate);
}

////////////////////////////////////////////////////////////
double Epoch::GetJD() const
{
   return ConvertMJD20002JD(m_mjd2000);
}

////////////////////////////////////////////////////////////
double Epoch::GetMJD() const
{
   return ConvertMJD20002MJD(m_mjd2000);
}

////////////////////////////////////////////////////////////
double Epoch::GetMJD2000() const
{
   return m_mjd2000;
}

////////////////////////////////////////////////////////////
GregorianDateTime Epoch::GetGregorian() const
{
   double julianDate = GetJD();
   return ConvertJD2Gregorian(julianDate);
}

std::string Epoch::ToString() const
{
   const auto& date = GetGregorian();

   std::ostringstream os;
   os << std::setfill('0') << std::setw(4) << date.year << "-"
      << std::setfill('0') << std::setw(2) << date.month << "-"
      << std::setfill('0') << std::setw(2) << date.day << " "
      << std::setfill('0') << std::setw(2) << date.hour << ":"
      << std::setfill('0') << std::setw(2) << date.min << ":"
      << std::setfill('0') << std::setw(6) << std::setprecision(3) << std::fixed << date.sec;

   return os.str();
}

std::string Epoch::ToDetailedString(std::string prefix) const
{
   auto date = GetGregorian();
   auto jd = GetJD();
   auto mjd = GetMJD();
   auto mjd2000 = GetMJD2000();

   DayOfWeek dayOfWeek = CalculateDayOfWeek(date);
   int dayOfYear = CalculateDayOfYear(date);

   std::ostringstream os;
   os << prefix << "Epoch:" << std::endl;
   os << prefix << "   Gregorian:" << std::endl;
   os << prefix << "      Date:          " << ConvertDayOfWeek2String(dayOfWeek) << " "
                                           << ConvertMonth2String(date.month) << " "
                                           << std::setfill('0') << std::setw(2) << date.day << ", "
                                           << std::setfill('0') << std::setw(4) << date.year << std::endl;
   os << prefix << "      Time:          " << std::setfill('0') << std::setw(2) << date.hour << ":"
                                           << std::setfill('0') << std::setw(2) << date.min << ":"
                                           << std::setfill('0') << std::setw(6) << std::setprecision(3) << std::fixed << date.sec << std::endl;
   os << prefix << "      Day of Year:   " << dayOfYear << std::endl;
   os << prefix << "   Julian:" << std::endl;
   os << prefix << "      Date:          " << std::setprecision(6) << std::fixed << jd << std::endl;
   os << prefix << "      Modified:      " << std::setprecision(6) << std::fixed << mjd << std::endl;
   os << prefix << "      Modified 2000: " << std::setprecision(6) << std::fixed << mjd2000 << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
bool operator==(const Epoch& left, const Epoch& right)
{
   return IsApprox(left.GetMJD2000(), right.GetMJD2000(), 2.0 * MATH_EPSILON);
}

////////////////////////////////////////////////////////////
bool operator!=(const Epoch& left, const Epoch& right)
{
   return !(left == right);
}

////////////////////////////////////////////////////////////
bool operator >(const Epoch& left, const Epoch& right)
{
   return (left.GetMJD2000() > right.GetMJD2000());
}

////////////////////////////////////////////////////////////
bool operator <(const Epoch& left, const Epoch& right)
{
   return (left.GetMJD2000() < right.GetMJD2000());
}

////////////////////////////////////////////////////////////
bool operator>=(const Epoch& left, const Epoch& right)
{
   return (left > right || left == right);
}
////////////////////////////////////////////////////////////
bool operator<=(const Epoch& left, const Epoch& right)
{
   return (left < right || left == right);
}

////////////////////////////////////////////////////////////
Epoch& operator +=(Epoch& left, const Time& right)
{
   left.SetMJD2000(left.GetMJD2000() + right.Days());
   return left;
}

////////////////////////////////////////////////////////////
Epoch& operator -=(Epoch& left, const Time& right)
{
   left.SetMJD2000(left.GetMJD2000() - right.Days());
   return left;
}

////////////////////////////////////////////////////////////
Epoch operator +(const Epoch& left, const Time& right)
{
   return Epoch::MJD2000(left.GetMJD2000() + right.Days());
}

////////////////////////////////////////////////////////////
Epoch operator -(const Epoch& left, const Time& right)
{
   return Epoch::MJD2000(left.GetMJD2000() - right.Days());
}

////////////////////////////////////////////////////////////
double ConvertMJD2JD(double modifiedJulianDate)
{
   return modifiedJulianDate + 2400000.5;
}

////////////////////////////////////////////////////////////
double ConvertMJD20002JD(double modifiedJulianDate2000)
{
   return modifiedJulianDate2000 + 2451544.5;
}

////////////////////////////////////////////////////////////
double ConvertGregorian2JD(const GregorianDateTime& dateTime)
{
   double julianDate = 367.0 * dateTime.year -
      Round0(7.0 * (dateTime.year + Round0((dateTime.month + 9.0) / 12.0)) / 4.0) +
      Round0(275.0 * dateTime.month / 9.0) +
      dateTime.day + 1721013.5 +
      dateTime.hour / 24.0 +
      (dateTime.min / 60.0 / 24.0) +
      dateTime.sec / 3600.0 / 24.0;
   return julianDate;
}

////////////////////////////////////////////////////////////
double ConvertJD2MJD(double julianDate)
{
   return julianDate - 2400000.5;
}

////////////////////////////////////////////////////////////
double ConvertMJD20002MJD(double modifiedJulianDate2000)
{
   return modifiedJulianDate2000 + 51544.0;
}

////////////////////////////////////////////////////////////
double ConvertGregorian2MJD(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   return ConvertJD2MJD(julianDate);
}

////////////////////////////////////////////////////////////
double ConvertJD2MJD2000(double julianDate)
{
   return julianDate - 2451544.5;
}

////////////////////////////////////////////////////////////
double ConvertMJD2MJD2000(double modifiedJulianDate)
{
   return modifiedJulianDate - 51544.0;
}

////////////////////////////////////////////////////////////
double ConvertGregorian2MJD2000(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   return ConvertJD2MJD2000(julianDate);
}

////////////////////////////////////////////////////////////
GregorianDateTime ConvertJD2Gregorian(double julianDate)
{
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

   bool isLeapYear = (year % 4 == 0);

   int dayOfYear = static_cast<int>(floor(days));
   int month = 1;
   int daysSum = 0;
   while (month <= 12 && dayOfYear > (daysSum + g_daysPerMonth[month - 1]))
   {
      daysSum += g_daysPerMonth[month++ - 1];
      if (month == 2 && isLeapYear)
      {
         daysSum += 1;
      }
   }

   int day = dayOfYear - daysSum;

   double tau = (days - dayOfYear) * 24;
   int hour = static_cast<int>(floor(tau));
   int minute = static_cast<int>(floor((tau - hour) * 60.0));
   double seconds = (tau - hour - minute / 60.0) * 3600.0;

   return GregorianDateTime(year, month, day, hour, minute, seconds);
}

////////////////////////////////////////////////////////////
GregorianDateTime ConvertMJD2Gregorian(double modifiedJulianDate)
{
   double julianDate = ConvertMJD2JD(modifiedJulianDate);
   return ConvertJD2Gregorian(julianDate);
}

////////////////////////////////////////////////////////////
GregorianDateTime ConvertMJD20002Gregorian(double modifiedJulianDate2000)
{
   double julianDate = ConvertMJD20002JD(modifiedJulianDate2000);
   return ConvertJD2Gregorian(julianDate);
}

////////////////////////////////////////////////////////////
int CalculateDayOfYear(const GregorianDateTime& date)
{
   bool isLeapYear = (date.year % 4 == 0);
   int dayOfYear = 0;
   for (int i = 1; i < date.month; ++i)
   {
      dayOfYear += g_daysPerMonth[i - 1];
      if (i == 1 && isLeapYear)
      {
         dayOfYear += 1;
      }
   }
   dayOfYear += date.day;
   return dayOfYear;
}

////////////////////////////////////////////////////////////
DayOfWeek CalculateDayOfWeek(const GregorianDateTime& date)
{
   // Months table(s) for regular and leap years
   static const int monthTable[12]     = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
   static const int monthTableLeap[12] = { 6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };

   // Is this a leap year?
   bool isLeapYear = (date.year % 4 == 0);

   // Last and first two digits of the year
   int yearLast2 = date.year % 100;
   int yearFirst2 = static_cast<int>(0.01 * (date.year - yearLast2));

   // Compute year, month, day
   int y = yearLast2; 
   int m = isLeapYear ? monthTableLeap[date.month - 1] : monthTable[date.month - 1];
   int d = date.day;

   // Compute century number
   int temp = yearFirst2 % 4;
   int c = 6 - 2 * temp;

   // Compute day of week
   auto dayOfWeek = static_cast<DayOfWeek>((d + m + y + (y / 4) + c) % 7);
   return dayOfWeek;
}

////////////////////////////////////////////////////////////
std::string ConvertMonth2String(int month)
{
   OTL_ASSERT(month > 0 && month <= 12, "Invalid month. Month must be between 1..12 where Jan=1, Feb=2, etc.");
   return g_monthNames[month - 1];
}

////////////////////////////////////////////////////////////
std::string ConvertDayOfWeek2String(const DayOfWeek& dayOfWeek)
{
   OTL_ASSERT(dayOfWeek > DayOfWeek::Invalid && dayOfWeek < DayOfWeek::Count);
   return g_dayNames[static_cast<int>(dayOfWeek)];
}

} // namespace otl
