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

#include <OTL/Core/Time.h>

namespace otl
{

////////////////////////////////////////////////////////////
Time::AggregrateTime::AggregrateTime(int _years, int _days, int _hours, int _minutes, double _seconds) :
years(_years),
days(_days),
hours(_hours),
minutes(_minutes),
seconds(_seconds)
{

}

////////////////////////////////////////////////////////////
Time::Time() :
m_seconds(0.0)
{

}

////////////////////////////////////////////////////////////
Time Time::Seconds(double seconds)
{
   Time time;
   time.SetSeconds(seconds);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Minutes(double minutes)
{
   Time time;
   time.SetMinutes(minutes);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Hours(double hours)
{
   Time time;
   time.SetHours(hours);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Days(double days)
{
   Time time;
   time.SetDays(days);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Years(double years)
{
   Time time;
   time.SetYears(years);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Aggregrate(const AggregrateTime& aggregrateTime)
{
   Time time;
   time.SetAggregrate(aggregrateTime);
   return time;
}

////////////////////////////////////////////////////////////
Time Time::Infinity()
{
   return Time::Seconds(MATH_INFINITY);
}

////////////////////////////////////////////////////////////
void Time::SetSeconds(double seconds)
{
   m_seconds = seconds;
}

////////////////////////////////////////////////////////////
void Time::SetMinutes(double minutes)
{
   m_seconds = minutes * MATH_MIN_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::SetHours(double hours)
{
   m_seconds = hours * MATH_HOUR_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::SetDays(double days)
{
   m_seconds = days * MATH_DAY_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::SetYears(double years)
{
   m_seconds = years * MATH_YEAR_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::SetAggregrate(const AggregrateTime& aggregrateTime)
{
   double totalSeconds = aggregrateTime.seconds;
   totalSeconds += static_cast<double>(aggregrateTime.minutes) * MATH_MIN_TO_SEC;
   totalSeconds += static_cast<double>(aggregrateTime.hours)   * MATH_HOUR_TO_SEC;
   totalSeconds += static_cast<double>(aggregrateTime.days)    * MATH_DAY_TO_SEC;
   totalSeconds += static_cast<double>(aggregrateTime.years)   * MATH_YEAR_TO_SEC;

   m_seconds = totalSeconds;
}

////////////////////////////////////////////////////////////
double Time::Seconds() const
{
   return m_seconds;
}

////////////////////////////////////////////////////////////
double Time::Minutes() const
{
   return m_seconds * MATH_SEC_TO_MIN;
}

////////////////////////////////////////////////////////////
double Time::Hours() const
{
   return m_seconds * MATH_SEC_TO_HOUR;
}

////////////////////////////////////////////////////////////
double Time::Days() const
{
   return m_seconds * MATH_SEC_TO_DAY;
}

////////////////////////////////////////////////////////////
double Time::GetYears() const
{
   return m_seconds * MATH_SEC_TO_YEAR;
}

///////////////////////////////////////////////////////////
Time::AggregrateTime Time::GetAggregrate() const
{
   double seconds = Seconds();
   int years = static_cast<int>(seconds * MATH_SEC_TO_YEAR);
   seconds -= years * MATH_YEAR_TO_SEC;
   int days = static_cast<int>(seconds * MATH_SEC_TO_DAY);
   seconds -= days * MATH_DAY_TO_SEC;
   int hours = static_cast<int>(seconds * MATH_SEC_TO_HOUR);
   seconds -= hours * MATH_HOUR_TO_SEC;
   int minutes = static_cast<int>(seconds * MATH_SEC_TO_MIN);
   seconds -= minutes * MATH_MIN_TO_SEC;

   return AggregrateTime(years, days, hours, minutes, seconds);
}

////////////////////////////////////////////////////////////
void Time::AddSeconds(double seconds)
{
   m_seconds += seconds;
}

////////////////////////////////////////////////////////////
void Time::AddMinutes(double minutes)
{
   m_seconds += minutes * MATH_MIN_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::AddHours(double hours)
{
   m_seconds += hours * MATH_HOUR_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::AddDays(double days)
{
   m_seconds += days * MATH_DAY_TO_SEC;
}

////////////////////////////////////////////////////////////
void Time::AddYears(double years)
{
   m_seconds += years * MATH_YEAR_TO_SEC;
}

////////////////////////////////////////////////////////////
bool Time::IsZero() const
{
   return IsApprox(m_seconds, 0.0);
}

////////////////////////////////////////////////////////////
bool Time::IsInfinity() const
{
   return fabs(m_seconds) == MATH_INFINITY;
}

////////////////////////////////////////////////////////////
std::string Time::ToString() const
{
   std::ostringstream os;

   auto years = GetYears();
   if (years > 1.0)
   {
      os << years << " years";
      return os.str();
   }

   auto days = Days();
   if (days > 1.0)
   {
      os << days << " days";
      return os.str();
   }

   auto hours = Hours();
   if (hours > 1.0)
   {
      os << hours << " hours";
      return os.str();
   }

   auto minutes = Minutes();
   if (minutes > 1.0)
   {
      os << minutes << " minutes";
      return os.str();
   }

   os << Seconds() << " seconds";
   return os.str();
}

////////////////////////////////////////////////////////////
std::string Time::ToDetailedString(std::string prefix) const
{
   auto aggregate = GetAggregrate();

   std::ostringstream os;
   os << prefix << "Aggregrate:" << std::endl;
   os << prefix << "   Years:   " << aggregate.years << std::endl;
   os << prefix << "   Days:    " << aggregate.days << std::endl;
   os << prefix << "   Hours:   " << aggregate.hours << std::endl;
   os << prefix << "   Minutes: " << aggregate.minutes << std::endl;
   os << prefix << "   Seconds: " << std::setprecision(3) << std::fixed << aggregate.seconds << std::endl;
   os << prefix << "Total: " << std::endl;
   os << prefix << "   Years:   " << std::setprecision(11) << std::fixed << GetYears() << std::endl;
   os << prefix << "   Days:    " << std::setprecision(9) << std::fixed << Days() << std::endl;
   os << prefix << "   Hours:   " << std::setprecision(7) << std::fixed << Hours() << std::endl;
   os << prefix << "   Minutes: " << std::setprecision(5) << std::fixed << Minutes() << std::endl;
   os << prefix << "   Seconds: " << std::setprecision(3) << std::fixed << Seconds() << std::endl;

   return os.str();
}

////////////////////////////////////////////////////////////
bool operator==(const Time& left, const Time& right)
{
   return (IsApprox(left.Seconds(), right.Seconds(), 2.0 * MATH_EPSILON));
}

////////////////////////////////////////////////////////////
bool operator!=(const Time& left, const Time& right)
{
   return !(left == right);
}

////////////////////////////////////////////////////////////
bool operator >(const Time& left, const Time& right)
{
   return (left.Seconds() > right.Seconds());
}

////////////////////////////////////////////////////////////
bool operator <(const Time& left, const Time& right)
{
   return (left.Seconds() < right.Seconds());
}

////////////////////////////////////////////////////////////
bool operator>=(const Time& left, const Time& right)
{
   return ((left > right) || (left == right));
}

////////////////////////////////////////////////////////////
bool operator<=(const Time& left, const Time& right)
{
   return ((left < right) || (left == right));
}

////////////////////////////////////////////////////////////
Time& operator+=(Time& left, const Time& right)
{
   left.AddSeconds(right.Seconds());
   return left;
}

////////////////////////////////////////////////////////////
Time& operator-=(Time& left, const Time& right)
{
   left.AddSeconds(-1.0 * right.Seconds());
   return left;
}

////////////////////////////////////////////////////////////
Time operator+ (const Time& left, const Time& right)
{ return Time::Seconds(left.Seconds() + right.Seconds()); }

////////////////////////////////////////////////////////////
Time operator- (const Time& left, const Time& right)
{ return Time::Seconds(left.Seconds() - right.Seconds()); }

} // namespace otl