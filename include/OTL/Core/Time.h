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
#include <OTL/Core/Base.h>

namespace otl
{

class OTL_CORE_API Time
{
public:
   struct AggregrateTime
   {
      int years;
      int days;
      int hours;
      int minutes;
      double seconds;

      AggregrateTime(int _years, int _days, int _hours, int _minutes, double _seconds);
   };

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using seconds
   ///
   /// Creates a Time object from a number of seconds.
   ///
   /// \param seconds Number of seconds
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Seconds(double seconds);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using minutes
   ///
   /// Creates a Time object from a number of minutes.
   /// Internally, the minutes are converted into
   /// seconds.
   ///
   /// \param minutes Number of minutes
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Minutes(double minutes);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using hours
   ///
   /// Creates a Time object from a number of hours.
   /// Internally, the hours are converted into
   /// seconds.
   ///
   /// \param hours Number of hours
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Hours(double hours);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using days
   ///
   /// Creates a Time object from a number of days.
   /// Internally, the days are converted into
   /// seconds.
   ///
   /// \param days Number of days
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Days(double days);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using years
   ///
   /// Creates a Time object from a number of years.
   /// Internally, the years are converted into
   /// seconds.
   ///
   /// \param years Number of years
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Years(double years);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using an aggregrate time
   ///
   /// Creates a Time object from an aggregrate number of
   /// years, days, hours, minutes, and seconds. Internally,
   /// the aggregrate time is converted into total seconds.
   ///
   /// \param years Number of years
   /// \returns instance of Time
   ///
   ////////////////////////////////////////////////////////////
   static Time Aggregrate(const AggregrateTime& aggregrateTime);

   ////////////////////////////////////////////////////////////
   /// \brief Set the time to a number of seconds
   ///
   /// \param seconds Number of seconds
   ///
   ////////////////////////////////////////////////////////////
   void SetSeconds(double seconds);

   ////////////////////////////////////////////////////////////
   /// \brief Set the time to a number of minutes
   ///
   /// \param minutes Number of minutes
   ///
   ////////////////////////////////////////////////////////////
   void SetMinutes(double minutes);

   ////////////////////////////////////////////////////////////
   /// \brief Set the time to a number of hours
   ///
   /// \param hours Number of hours
   ///
   ////////////////////////////////////////////////////////////
   void SetHours(double hours);

   ////////////////////////////////////////////////////////////
   /// \brief Set the time to a number of days
   ///
   /// \param days Number of days
   ///
   ////////////////////////////////////////////////////////////
   void SetDays(double days);

   ////////////////////////////////////////////////////////////
   /// \brief Set the time to a number of years
   ///
   /// \param years Number of years
   ///
   ////////////////////////////////////////////////////////////
   void SetYears(double years);

   ////////////////////////////////////////////////////////////
   /// \brief Set the aggregrate time
   ///
   /// Sets the time by combining the aggregrate number of
   /// years, days, hours, minutes, and seconds.
   ///
   ////////////////////////////////////////////////////////////
   void SetAggregrate(const AggregrateTime& aggregrateTime);

   ////////////////////////////////////////////////////////////
   /// \brief Get the total number of seconds
   ///
   /// Fractional seconds are expressed as decimals.
   ///
   /// \returns Number of seconds
   ///
   ////////////////////////////////////////////////////////////
   double Seconds() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the total number of minutes
   ///
   /// Fractional minutes are expressed as decimals.
   ///
   /// \returns Number of minutes
   ///
   ////////////////////////////////////////////////////////////
   double Minutes() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the total number of hours
   ///
   /// Fractional hours are expressed as decimals.
   ///
   /// \returns Number of hours
   ///
   ////////////////////////////////////////////////////////////
   double Hours() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the total number of days
   ///
   /// Fractional days are expressed as decimals.
   ///
   /// \returns Number of days
   ///
   ////////////////////////////////////////////////////////////
   double Days() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the total number of years
   ///
   /// Fractional years are expressed as decimals.
   ///
   /// \returns Number of years
   ///
   ////////////////////////////////////////////////////////////
   double GetYears() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the aggregrate time
   ///
   /// Splits the total number of seconds into
   /// years, days, hours, minutes, and seconds.
   ///
   /// \returns AggregrateTime
   ///
   ////////////////////////////////////////////////////////////
   AggregrateTime GetAggregrate() const;

   ////////////////////////////////////////////////////////////
   /// \brief Add a number of seconds
   ///
   /// Negative seconds will result in subtraction.
   ///
   /// \param seconds Number of seconds
   ///
   ////////////////////////////////////////////////////////////
   void AddSeconds(double seconds);

   ////////////////////////////////////////////////////////////
   /// \brief Add a number of minutes
   ///
   /// Negative minutes will result in subtraction.
   ///
   /// \param minutes Number of minutes
   ///
   ////////////////////////////////////////////////////////////
   void AddMinutes(double minutes);

   ////////////////////////////////////////////////////////////
   /// \brief Add a number of hours
   ///
   /// Negative hours will result in subtraction.
   ///
   /// \param hours Number of hours
   ///
   ////////////////////////////////////////////////////////////
   void AddHours(double hours);

   ////////////////////////////////////////////////////////////
   /// \brief Add a number of days
   ///
   /// Negative days will result in subtraction.
   ///
   /// \param days Number of days
   ///
   ////////////////////////////////////////////////////////////
   void AddDays(double days);

   ////////////////////////////////////////////////////////////
   /// \brief Add a number of years
   ///
   /// Negative years will result in subtraction.
   ///
   /// \param years Number of years
   ///
   ////////////////////////////////////////////////////////////
   void AddYears(double years);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the time to a single-line formatted string
   ///
   /// The time is converted to a single-line string
   /// with the following format:
   ///
   /// "[total time] [time units]"
   ///
   /// e.g.
   ///
   /// "36.456 sec"
   /// "10.125 days"
   /// "2.512 years"
   ///
   /// \note The most significant 
   ///
   /// \returns std::string Stringified time
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the time to a detailed multi-line formatted string
   ///
   /// The time is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Time:
   ///     Aggregate:
   ///        Years:   [years]
   ///        Days:    [days]
   ///        Hours:   [hours]
   ///        Minutes: [minutes]
   ///        Seconds: [seconds]
   ///     Total:
   ///        Years:   [total years]
   ///        Days:    [total days]
   ///        Hours:   [total hours]
   ///        Minutes: [total minutes]
   ///        Seconds: [total seconds]
   /// "
   ///
   /// e.g.
   ///
   /// "Time:
   ///     Aggregate:
   ///        Years:   0
   ///        Days:    10
   ///        Hours:   2
   ///        Minutes: 30
   ///        Seconds: 15.45
   ///     Total:
   ///        Years:   0.027664
   ///        Days:    10.10435
   ///        Hours:   242.5043
   ///        Minutes: 14550.258
   ///        Seconds: 873015.45
   /// "
   ///
   /// \returns std::string Stringified time
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString() const;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor not allowed
   ////////////////////////////////////////////////////////////
   Time() {}

private:
   double m_seconds; ///< Total number of seconds
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Time
///
/// The Time is converted to a string by calling the
/// Time::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const Time& epoch)
{
   stream << epoch.ToString();
   return stream;
}

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator==
///
/// This operator compares approximate equality between two times.
///
/// \note Internally, the IsApprox() function is used with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator==(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator!=
///
/// This operator compares approximate inequality between two times.
///
/// \note Internally, the IsApprox() function is used with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is not equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator!=(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator >
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is strictly greater than right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator >(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator <
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is strictly less than right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator <(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator >=
///
/// \note Internally, the IsApprox() function is used with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is greater than or approximately equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator>=(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator <=
///
/// \note Internally, the IsApprox() function is used with epsilon = 2 * MATH_EPSILON
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left less than or approximately equal to right
///
////////////////////////////////////////////////////////////
OTL_CORE_API bool operator<=(const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator+=
///
/// Add left and right and assign the result to left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Reference to left
///
////////////////////////////////////////////////////////////
OTL_CORE_API Time& operator+=(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=
///
/// Subtract right from left and assign the result to left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Reference to left
///
////////////////////////////////////////////////////////////
OTL_CORE_API Time& operator-=(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=
///
/// Create a new Time by adding left and right.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Sum of times
///
////////////////////////////////////////////////////////////
OTL_CORE_API Time operator+ (const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=
///
/// Create a new Time by subtracting right from left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Difference between times
///
////////////////////////////////////////////////////////////
OTL_CORE_API Time operator- (const Time& left, const Time& right);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Time
///
/// Basic construct for representing a duration of time. 
/// Time is used throughout OTL to unify the use
/// of time and to make the interface for algorithms more
/// flexible by allowing any type of time format to be specified.
/// A time object is instantiated by calling one of the static
/// constructors:
///
/// \li otl::Time::Seconds()
/// \li otl::Time::Minutes()
/// \li otl::Time::Hours()
/// \li otl::Time::Days()
/// \li otl::Time::Years()
///
/// Internally, the time duration is always stored as
/// seconds and converted from/to other time formats
/// as necessary.
///
/// Usage example:
/// \code
/// otl::Time time = otl::Days(10.0);
/// time.AddHours(6.0);
/// time.AddMinutes(30.0);
/// double totalSeconds = time.Seconds();
///
/// // Operator overloads are also supported
/// auto time1 = time;
/// time1 += otl::Time::Days(1.0);
/// time1 -= otl::Time::Hours(2.0);
/// auto time2 = time1 + otl::Time::Days(10.0);
/// OTL_ASSERT(time2 > time1);
///
/// auto sixtySeconds = otl::Time::Seconds(60.0);
/// auto oneMinute = otl::Minutes(1.0);
/// OLT_ASSERT(sixtySeconds == oneMinute);
///
/// // In use with OTL's algorithms:
/// auto propagator = std::make_shared<otl::keplerian::KeplerianPropagator>();
/// propagator->Propagate(myStateVector, MATH_MU_EARTH, otl::Time::Days(1.0));                        // propagate forward 1 day
/// propagator->Propagate(myStateVector, MATH_MU_EARTH, otl::Time::Seconds(-1.0 * MATH_DAY_TO_SEC));  // propagate backwards 1 day
/// \endcode
/// 
////////////////////////////////////////////////////////////