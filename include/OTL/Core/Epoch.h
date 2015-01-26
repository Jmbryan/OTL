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
#include <OTL/Core/Time.h>

namespace otl
{

   enum class DayOfWeek
   {
      Invalid = -1,
      Sunday,
      Monday,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday,
      Count
   };

////////////////////////////////////////////////////////////
/// \brief Simple data structure for expressing a Gregorian date and time
/// \ingroup otl
/// 
////////////////////////////////////////////////////////////
struct OTL_CORE_API GregorianDateTime
{
   int year;   ///< The year
   int month;  ///< The month
   int day;    ///< The day
   int hour;   ///< The hour
   int min;    ///< The minute
   double sec; ///< The second

   GregorianDateTime();
   GregorianDateTime(int _year, int _month, int _day);
   GregorianDateTime(int _year, int _month, int _day,
                     int _hour, int _minute, double _second);
};

class OTL_CORE_API Epoch
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Epoch();

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using a Julian Date
   ///
   /// Creates an Epoch object from a Julian Date.
   /// Internally, the Julian Date is converted into
   /// a Modified Julian Date 2000.
   ///
   /// \param julianDate Julian Date
   /// \returns instance of Epoch
   ///
   ////////////////////////////////////////////////////////////
   static Epoch JD(double julianDate);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using a Modified Julian Date
   ///
   /// Creates an Epoch object from a Modified Julian Date.
   /// Internally, the Modified Julian Date is converted into
   /// a Modified Julian Date 2000.
   ///
   /// \param modifiedJulianDate Modified Julian Date
   /// \returns instance of Epoch
   ///
   ////////////////////////////////////////////////////////////
   static Epoch MJD(double modifiedJulianDate);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using a Modified Julian Date 2000
   ///
   /// Creates an Epoch object from a Modified Julian Date 2000.
   ///
   /// \param modifiedJulianDate2000 Modified Julian Date 2000
   /// \returns instance of Epoch
   ///
   ////////////////////////////////////////////////////////////
   static Epoch MJD2000(double modifiedJulianDate2000);

   ////////////////////////////////////////////////////////////
   /// \brief Static constructor using a Gregorian Date and time
   ///
   /// Creates an Epoch object from a Gregorian Date and time.
   /// Internally, the Gregordian Date and time is converted into
   /// a Modified Julian Date 2000.
   ///
   /// \param dateTime Gregorian Date and time
   /// \returns instance of Epoch
   ///
   ////////////////////////////////////////////////////////////
   static Epoch Gregorian(const GregorianDateTime& dateTime);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch to the Julian Date.
   ///
   /// The epoch is converted to MJD2000 internally.
   ///
   /// \param julianDate New epoch expressed as a Julian Date
   ///
   ////////////////////////////////////////////////////////////
   void SetJD(double julianDate);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch to the Modified Julian Date.
   ///
   /// The epoch is converted to MJD2000 internally.
   ///
   /// \param modifiedJulianDate New epoch expressed as a Modified Julian Date
   ///
   ////////////////////////////////////////////////////////////
   void SetMJD(double modifiedJulianDate);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch to the Modified Julian Date 2000.
   ///
   /// \param modifiedJulianDate2000 New epoch expressed as a Modified Julian Date 2000
   ///
   ////////////////////////////////////////////////////////////
   void SetMJD2000(double modifiedJulianDate2000);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch to the Gregorian date and time.
   ///
   /// The epoch is converted to MJD2000 internally.
   ///
   /// \param dateTime New epoch expressed as a Gregorian date and time
   ///
   ////////////////////////////////////////////////////////////
   void SetGregorian(const GregorianDateTime& dateTime);

   ////////////////////////////////////////////////////////////
   /// \brief Get the Julian Date.
   ///
   /// The epoch is converted from MJD2000 internally.
   ///
   /// \returns double representing the epoch expressed as a Julian Date
   ///
   ////////////////////////////////////////////////////////////
   double GetJD() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the Modified Julian Date.
   ///
   /// The epoch is converted from MJD2000 internally.
   ///
   /// \returns double representing the epoch expressed as a Modified Julian Date
   ///
   ////////////////////////////////////////////////////////////
   double GetMJD() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the Modified Julian Date 2000.
   ///
   /// \returns double representing the epoch expressed as a Modified Julian Date 2000
   ///
   ////////////////////////////////////////////////////////////
   double GetMJD2000() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the Gregorian date and time.
   ///
   /// The epoch is converted from MJD2000 internally.
   ///
   /// \returns GregorianDateTime representing the epoch expressed as a Gregorian date and time
   ///
   ////////////////////////////////////////////////////////////
   GregorianDateTime GetGregorian() const;

private:
   double m_mjd2000; ///< Modified Julian Date 2000
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Epoch
///
/// The epoch is converted to a string in the following format:
/// "dayOfWeek year-month-date hour:min:sec" e.g. "Wed 2014-1-15 11:30:0"
/// "Wed. Jan. 15, 2014 - 11:30:00"
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
//template<typename T>
std::ostream& operator<<(std::ostream& stream, const Epoch& epoch);
//{
//    const auto& date = epoch.GetGregorian();
//    auto dayOfWeek = CalculateDayOfWeek(date);
//    auto dayOfWeekString = ConvertDayOfWeek2String(dayOfWeek).substr(0, 3);
//    auto monthString = ConvertMonth2String(date.month).substr(0, 3);
//
//    stream.fill('0');
//    stream.width(2);
//    stream << dayOfWeekString << " " << monthString << " "
//           << date.day << ", " << date.year << " - "
//           << date.hour << ":" << date.min << ":" << static_cast<int>(date.sec);
//    return stream;
//}

////////////////////////////////////////////////////////////
/// \brief Converts the epoch to a multi-line formatted string
/// \relates Epoch
///
/// The epoch is converted to a multi-line string
/// in the following format:
///
/// "Wednesday January 15, 2014 - 11:30:00 UTC"
///
/// "Epoch:
///     Gregorian:
///        Year:      [year]
///        Month:     [month]
///        Day:       [day]
///        Hour:      [hour]
///        Minute:    [minute]
///        Second:    [second]
///        DayOfWeek: [dayOfWeek]
///        DayOfYear: [dayOfYear]
///     Julian:
///        Julian:               [julian date]
///        Modified Julian:      [modified julian date]
///        Modified Julian 2000: [modified julian date 2000]
/// "
///
/// "Epoch:
///     Gregorian:
///        [day of week] [month] [day], [year] - [hour]:[minute]:[second]
///        Wednesday January 15, 2014 - 11:30:00
///     Julian:
///        Julian:               [julian date]
///        Modified Julian:      [modified julian date]
///        Modified Julian 2000: [modified julian date 2000]
/// "
///
/// e.g.
///
/// "Epoch:
///     Gregorian:
///        Date:          Wednesday January 15, 2014 - 11:30:00
///        Day of Year:   xx
///     Julian:
///        Date:          xx
///        Modified:      xx
///        Modified 2000: xx
/// "
///
/// \param epoch Epoch to be formatted
/// \returns std::string Formatted epoch
///
////////////////////////////////////////////////////////////
OTL_CORE_API std::string HumanReadable(const Epoch& epoch);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator +=
/// \relates Epoch
///
/// This operator adds a Time to the Epoch,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Time)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
OTL_CORE_API Epoch& operator +=(Epoch& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator -=
/// \relates Epoch
///
/// This operator subtracts a Time from the Epoch,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Time)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
OTL_CORE_API Epoch& operator -=(Epoch& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator +
/// \relates Epoch
///
/// This operator adds the Time to the Epoch,
/// and assigns the result to a new Epoch.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Time)
///
/// \return Instance of Epoch
///
////////////////////////////////////////////////////////////
OTL_CORE_API Epoch operator +(const Epoch& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator -
/// \relates Epoch
///
/// This operator subratcs the Time from the Epoch,
/// and assigns the result to a new Epoch.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Time)
///
/// \return Instance of Epoch
///
////////////////////////////////////////////////////////////
OTL_CORE_API Epoch operator -(const Epoch& left, const Time& right);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date to Julian Date.
/// \relates Epoch
///
/// \param modifiedJulianDate Modified Julian date
/// \returns Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertMJD2JD(double modifiedJulianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date 2000 to Julian Date
/// \relates Epoch
///
/// \param modifiedJulianDate2000 Modified Julian date 2000
/// \returns Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertMJD20002JD(double modifiedJulianDate2000);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Gregorian Date to Julian Date
/// \relates Epoch
///
/// \param dateTIme Gregorian date and time
/// \returns Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertGregorian2JD(const GregorianDateTime& dateTime);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Julian Date to Modified Julian Date
/// \relates Epoch
///
/// \param julianDate Julian date
/// \returns Modified Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertJD2MJD(double julianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date 2000 to Modified Julian Date
/// \relates Epoch
///
/// \param modifiedJulianDate2000 Modified Julian date 2000
/// \returns Modified Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertMJD20002MJD(double modifiedJulianDate2000);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Gregorian Date to Modified Julian Date
/// \relates Epoch
///
/// \param dateTime Gregorian date and time
/// \returns Modified Julian date
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertGregorian2MJD(const GregorianDateTime& dateTime);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Julian Date to Modified Julian Date 2000
/// \relates Epoch
///
/// \param julianDate Julian date
/// \returns Modified Julian date 2000
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertJD2MJD2000(double julianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date to Modified Julian Date 2000
/// \relates Epoch
///
/// \param modifiedJulianDate Modified Julian date
/// \returns Modified Julian date 2000
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertMJD2MJD2000(double modifiedJulianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Gregorian Date to Modified Julian Date 2000
/// \relates Epoch
///
/// \param dateTime Gregorian date and time
/// \returns Modified Julian date 2000
///
////////////////////////////////////////////////////////////
OTL_CORE_API double ConvertGregorian2MJD2000(const GregorianDateTime& dateTime);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Julian Date to Gregorian Date
/// \relates Epoch
///
/// \param julianDate Julian date
/// \returns Gregorian date and time
///
////////////////////////////////////////////////////////////
OTL_CORE_API GregorianDateTime ConvertJD2Gregorian(double julianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date to Gregorian Date
/// \relates Epoch
///
/// \param modifiedJulianDate Modified Julian date
/// \returns Gregorian date and time
///
////////////////////////////////////////////////////////////
OTL_CORE_API GregorianDateTime ConvertMJD2Gregorian(double modifiedJulianDate);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting from Modified Julian Date 2000 to Gregorian Date
/// \relates Epoch
///
/// \param modifiedJulianDate2000 Modified Julian date 2000
/// \returns Gregorian date and time
///
////////////////////////////////////////////////////////////
OTL_CORE_API GregorianDateTime ConvertMJD20002Gregorian(double modifiedJulianDate2000);

OTL_CORE_API int CalculateDayOfYear(const GregorianDateTime& date);
OTL_CORE_API DayOfWeek CalculateDayOfWeek(const GregorianDateTime& date);

OTL_CORE_API std::string ConvertMonth2String(int month);
OTL_CORE_API std::string ConvertDayOfWeek2String(const DayOfWeek& dayOfWeek);


} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Epoch
/// \ingroup otl
///
/// Represents a point in time expressed in a variety of formats.
/// Supported formats include:
/// \li Julian Date (JD)
/// \li Modified Julian Date (MJD)
/// \li Modified Julian Date 2000 (MJD2000)
/// \li Gregorian Date & Time
///
/// Internally, the date is always stored as a MJD2000. Setting
/// the date using a different format results in calling the
/// conversion helper functions ConvertMJD20002[OtherFormat].
/// Likewise, retreiving a date in a different format results
/// in calling the conversion helper functions Convert[OtherFormat]2MJD2000.
///
/// Usage example:
/// \code
/// // The following three epochs all represent the date January 1st, 2000
/// otl::Epoch epoch1 = otl::Epoch::JD(2451545.5);
/// otl::Epoch epoch2 = otl::Epoch::MJD(51544.0);
/// otl::Epoch epoch3 = otl::Epoch::MJD2000(0.0)
/// OTL_ASSERT(epoch1 == epoch2 && epoch2 == epoch3);
///
/// double mjd2000_1 = epoch1.GetMJD2000();
/// double mjd2000_2 = otl::ConvertJD2MJD2000(epoch2.GetJD()); // Helper conversion functions
/// OTL_ASSERT(mjd2000_1 == mjd2000_2);
///
/// otl::GregorianDateTime dateTime1 = epoch1.GetGregorian();
/// OTL_ASSERT(dateTime1.year == 2000 && dateTime1.month == 1 && dateTime1.day = 1);
///
/// otl::GregorianDateTime dateTime2;
/// dateTime2.year  = 2014;
/// dateTime2.month = 1;
/// dateTime2.day   = 14;
/// dateTime2.hour  = 19;
/// dateTime2.min   = 10;
/// dateTime2.sec   = 15.0;
/// otl::Epoch epoch5(dateTime2);
///
///\endcode
///
////////////////////////////////////////////////////////////