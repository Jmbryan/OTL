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

#include <OTL/Core/Time.hpp>

namespace otl
{

Time Time::Seconds(double seconds)
{
   Time time;
   time.SetSeconds(seconds);
   return time;
}

Time Time::Minutes(double minutes)
{
   Time time;
   time.SetMinutes(minutes);
   return time;
}

Time Time::Hours(double hours)
{
   Time time;
   time.SetHours(hours);
   return time;
}

Time Time::Days(double days)
{
   Time time;
   time.SetDays(days);
   return time;
}

void Time::SetSeconds(double seconds)
{
   m_seconds = seconds;
}

void Time::SetMinutes(double minutes)
{
   m_seconds = minutes * MATH_MIN_TO_SEC;
}

void Time::SetHours(double hours)
{
   m_seconds = hours * MATH_HOUR_TO_SEC;
}

void Time::SetDays(double days)
{
   m_seconds = days * MATH_DAY_TO_SEC;
}

double Time::Seconds() const
{
   return m_seconds;
}

double Time::Minutes() const
{
   return m_seconds / MATH_DAY_TO_SEC;
}

double Time::Hours() const
{
   return m_seconds / MATH_HOUR_TO_SEC;
}

double Time::Days() const
{
   return m_seconds / MATH_DAY_TO_SEC;
}

void Time::AddSeconds(double seconds)
{
   m_seconds += seconds;
}

void Time::AddMinutes(double minutes)
{
   m_seconds += minutes * MATH_MIN_TO_SEC;
}

void Time::AddHours(double hours)
{
   m_seconds += hours * MATH_HOUR_TO_SEC;
}

void Time::AddDays(double days)
{
   m_seconds += days * MATH_DAY_TO_SEC;
}

Time& operator+=(Time& lhs, const Time& rhs)
{
   lhs.AddSeconds(rhs.Seconds());
   return lhs;
}

Time& operator-=(Time& lhs, const Time& rhs)
{
   lhs.AddSeconds(-1.0 * rhs.Seconds());
   return lhs;
}

Time operator+ (const Time& lhs, const Time& rhs)
{ return Time::Seconds(lhs.Seconds() + rhs.Seconds()); }

Time operator- (const Time& lhs, const Time& rhs)
{ return Time::Seconds(lhs.Seconds() - rhs.Seconds()); }


} // namespace otl