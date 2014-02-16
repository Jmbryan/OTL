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
#include <OTL/Core/Base.hpp>

namespace otl
{

class Time
{
public:
   static Time Seconds(double seconds);
   static Time Minutes(double minutes);
   static Time Hours(double hours);
   static Time Days(double days);

   void SetSeconds(double seconds);
   void SetMinutes(double minutes);
   void SetHours(double hours);
   void SetDays(double days);

   double Seconds() const;
   double Minutes() const;
   double Hours() const;
   double Days() const;

   void AddSeconds(double seconds);
   void AddMinutes(double minutes);
   void AddHours(double hours);
   void AddDays(double days);

private:
   Time() {}

private:
   double m_seconds;
};

Time& operator+=(Time& lhs, const Time& rhs);
Time& operator-=(Time& lhs, const Time& rhs);
Time operator+ (const Time& lhs, const Time& rhs);
Time operator- (const Time& lhs, const Time& rhs);

} // namespace otl