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

////////////////////////////////////////////////////////////
inline void Epoch::SetJD(double julianDate)
{
   m_mjd2000 = ConvertJD2MJD2000(julianDate);
}

////////////////////////////////////////////////////////////
inline void Epoch::SetMJD(double modifiedJulianDate)
{
   m_mjd2000 = ConvertMJD2MJD2000(modifiedJulianDate);
}

////////////////////////////////////////////////////////////
inline void Epoch::SetMJD2000(double modifiedJulianDate2000)
{
   m_mjd2000 = modifiedJulianDate2000;
}

////////////////////////////////////////////////////////////
inline void Epoch::SetGregorian(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   SetJD(julianDate);
}

////////////////////////////////////////////////////////////
inline double Epoch::GetJD() const
{
   return ConvertMJD20002JD(m_mjd2000);
}

////////////////////////////////////////////////////////////
inline double Epoch::GetMJD() const
{
   return ConvertMJD20002MJD(m_mjd2000);
}

////////////////////////////////////////////////////////////
inline double Epoch::GetMJD2000() const
{
   return m_mjd2000;
}

////////////////////////////////////////////////////////////
inline GregorianDateTime Epoch::GetGregorian() const
{
   double julianDate = GetJD();
   return ConvertJD2Gregorian(julianDate);
}

////////////////////////////////////////////////////////////
inline Epoch& operator +=(Epoch& left, const Time& right)
{
    left.SetMJD2000(left.GetMJD2000() + right.Days());
    return left;
}

////////////////////////////////////////////////////////////
inline Epoch& operator -=(Epoch& left, const Time& right)
{
    left.SetMJD2000(left.GetMJD2000() - right.Days());
    return left;
}

////////////////////////////////////////////////////////////
inline Epoch operator +(const Epoch& left, const Time& right)
{
    return Epoch::MJD2000(left.GetMJD2000() + right.Days());
}

////////////////////////////////////////////////////////////
inline Epoch operator -(const Epoch& left, const Time& right)
{
    return Epoch::MJD2000(left.GetMJD2000() - right.Days());
}

////////////////////////////////////////////////////////////
inline double ConvertMJD2JD(double modifiedJulianDate)
{
   return modifiedJulianDate + 2400000.5;
}

////////////////////////////////////////////////////////////
inline double ConvertMJD20002JD(double modifiedJulianDate2000)
{
   return modifiedJulianDate2000 + 2451544.5;
}

////////////////////////////////////////////////////////////
inline double ConvertJD2MJD(double julianDate)
{
   return julianDate - 2400000.5;
}

////////////////////////////////////////////////////////////
inline double ConvertMJD20002MJD(double modifiedJulianDate2000)
{
   return modifiedJulianDate2000 + 51544.0;
}

////////////////////////////////////////////////////////////
inline double ConvertGregorian2MJD(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   return ConvertJD2MJD(julianDate);
}

////////////////////////////////////////////////////////////
inline double ConvertJD2MJD2000(double julianDate)
{
   return julianDate - 2451544.5;
}

////////////////////////////////////////////////////////////
inline double ConvertMJD2MJD2000(double modifiedJulianDate)
{
   return modifiedJulianDate - 51544.0;
}

////////////////////////////////////////////////////////////
inline double ConvertGregorian2MJD2000(const GregorianDateTime& dateTime)
{
   double julianDate = ConvertGregorian2JD(dateTime);
   return ConvertJD2MJD2000(julianDate);
}

////////////////////////////////////////////////////////////
inline GregorianDateTime ConvertMJD2Gregorian(double modifiedJulianDate)
{
   double julianDate = ConvertMJD2JD(modifiedJulianDate);
   return ConvertJD2Gregorian(julianDate);
}

////////////////////////////////////////////////////////////
inline GregorianDateTime ConvertMJD20002Gregorian(double modifiedJulianDate2000)
{
   double julianDate = ConvertMJD20002JD(modifiedJulianDate2000);
   return ConvertJD2Gregorian(julianDate);
}


