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

private:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor not allowed
   ////////////////////////////////////////////////////////////
   Time() {}

private:
   double m_seconds; ///< Total number of seconds
};

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator==.
///
/// This operator compares strict equality between two times.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is equal to right
///
////////////////////////////////////////////////////////////
bool operator==(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator!=.
///
/// This operator compares strict inequality between two times.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns True if left is not equal to right
///
////////////////////////////////////////////////////////////
bool operator!=(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator+=.
///
/// Add left and right and assign the result to left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Reference to left
///
////////////////////////////////////////////////////////////
Time& operator+=(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=.
///
/// Subtract right from left and assign the result to left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Reference to left
///
////////////////////////////////////////////////////////////
Time& operator-=(Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=.
///
/// Create a new Time by adding left and right.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Sum of times
///
////////////////////////////////////////////////////////////
Time operator+ (const Time& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator-=.
///
/// Create a new Time by subtracting right from left.
///
/// \param left Left operand (a Time)
/// \param right right operand (a Time)
/// \returns Difference between times
///
////////////////////////////////////////////////////////////
Time operator- (const Time& left, const Time& right);

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
///
/// Internally, the time duration is always stored as
/// seconds and converted from/to other time formats
/// as necessary.
///
/// Usage example:
/// \code
/// otl::Time seconds = otl::Time::Seconds(60.0);
/// otl::Time minutes = otl::Minutes(1.0);
/// OLT_ASSERT(seconds == minutes);
///
/// otl::Time days = otl::Days(10.0);
/// days += 10.0;
/// days.AddHours(6.0);
/// days.AddMinutes(30.0);
/// totalSeconds = days.Seconds();
///
/// otl::keplerian::IPropagateAlgorithm* propagator = new PropagateLagrangian();
/// propagator->(myStateVector, myMu, otl::Time::Days(1.0));                        // propagate forward 1 day
/// propagator->(myStateVector, myMu, otl::Time::Seconds(-1.0 * MATH_DAY_TO_SEC));  // propagate backwards 1 day
/// OTL_SAFE_DELETE(propagator);
/// \endcode
/// 
////////////////////////////////////////////////////////////