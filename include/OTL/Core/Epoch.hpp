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

struct GregorianDateTime
{
   int year;
   int month;
   int day;
   int hour;
   int min;
   double sec;
};

class Epoch
{
public:

   enum class Type
   {
      Invalid = -1,  ///< Invalid epoch type
      JD,            ///< Julian Date
      MJD,           ///< Modified Julian Date
      MJD2000,       ///< Modified Julian Date 2000
      Count          ///< Number of epoch types
   };

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Epoch();

   ////////////////////////////////////////////////////////////
   /// \brief Create the epoch from the specified format
   ///
   /// \param epoch Epoch value
   /// \param type Epoch type (JD, MJD, or MJD2000)
   ///
   ////////////////////////////////////////////////////////////
   Epoch(double epoch, Type type = Type::MJD2000);

   ////////////////////////////////////////////////////////////
   /// \brief Create the epoch from a Gregorian date and time
   ///
   /// \param date Gregorian date and time
   ///
   ////////////////////////////////////////////////////////////
   Epoch(const GregorianDateTime& date);

   void Set(double epoch, Type type = Type::MJD2000);
   void SetJD(double julianDate);
   void SetMJD(double modifiedJulianDate);
   void SetMJD2000(double modifiedJulianDate2000);
   void SetGregorian(const GregorianDateTime& date);

   double Get(Type type = Type::MJD2000) const;
   double GetJD() const;
   double GetMJD() const;
   double GetMJD2000() const;
   GregorianDateTime GetGregorian() const;

private:
   double m_mjd2000; ///< Modified Julian Date 2000
};

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator +=
///
/// This operator performs a memberwise addition of both Epochs,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Epoch)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
Epoch& operator +=(Epoch& left, const Epoch& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator -=
///
/// This operator performs a memberwise subtraction of both Epochs,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Epoch)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
Epoch& operator -=(Epoch& left, const Epoch& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator +
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Epoch)
///
/// \return Memberwise addition of both Epochs
///
////////////////////////////////////////////////////////////
Epoch operator +(const Epoch& left, const Epoch& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator -
///
/// \param left  Left operand (a Epoch)
/// \param right Right operand (a Epoch)
///
/// \return Memberwise subtraction of both Epochs
///
////////////////////////////////////////////////////////////
Epoch operator -(const Epoch& left, const Epoch& right);

double ConvertJD2MJD(double julianDate);
double ConvertJD2MJD2000(double julianDate);
double ConvertMJD2JD(double modifiedJulianDate);
double ConvertMJD2MJD2000(double modifiedJulianDate);
double ConvertMJD20002JD(double modifiedJulianDate2000);
double ConvertMJD20002MJD(double modifiedJulianDate2000);

#include <OTL/Core/Epoch.inl>

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Epoch
/// \ingroup otl
///
/// otl::Epoch is a simple class that defines a point in time.
/// An epoch supports multiple formats including Julian Date (JD),
/// Modified Julian Date (MJD), Modified Julian Date 2000 (MJD2000),
/// and Gregorian.
///
/// Internally, the date is always stored as a MJD2000. Setting
/// the date using a different format results in calling the
/// conversion helper functions ConvertMJD20002[OtherFormat].
/// Likewise, retreiving a date in a different format results
/// in calling the conversion helper functions Convert[OtherFormat]2MJD2000.
///
/// Usage example:
/// \code
/// otl::Epoch epoch1;
/// epoch1.SetMJD2000(1000);                    // One thousand days past Januray 1st, 2000.
/// otl::Epoch epoch2(1000);                    // Implicit conversion from double
/// otl::Epoch epoch3 = epoch1 + epoch2;        // Support for common operator overloads
///
/// double mjd = otl::ConvertMJD20002MJD(1000);
/// assert(epoch1.GetMJD() == mjd);
///
/// double jd = otl::ConvertMJD20002JD(1000);
/// otl::Epoch epoch4(jd, Epoch::Type::JD);
/// assert(epoch1 == epoch4);                   // Comparison works even if epochs are created with different formats
/// 
/// otl::GregorianDateTime dateTime;
/// dateTime.year  = 2014;
/// dateTime.month = 1;
/// dateTime.day   = 14;
/// dateTime.hour  = 19;
/// dateTime.min   = 10;
/// dateTime.sec   = 15.0;
/// otl::Epoch epoch5(dateTime);
///
///\endcode
///
////////////////////////////////////////////////////////////