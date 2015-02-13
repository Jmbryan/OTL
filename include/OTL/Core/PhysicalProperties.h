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
#include <string>

namespace otl
{

class PhysicalProperties
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   PhysicalProperties();

   ////////////////////////////////////////////////////////////
   /// \brief Construct physical properties from mass and radius
   ///
   /// \param mass Mass of the object
   /// \param radius Mean geometric radius of the object
   ///
   ////////////////////////////////////////////////////////////
   PhysicalProperties(double mass, double radius);

   ////////////////////////////////////////////////////////////
   /// \brief Construct physical properties from mass, radius, and safe radius
   ///
   /// \param mass Mass of the object
   /// \param radius Mean geometric radius of the object
   /// \param safeRadius Minimum geometric radius considered safe to approach to 
   ///
   ////////////////////////////////////////////////////////////
   PhysicalProperties(double mass, double radius, double safeRadius);

   ////////////////////////////////////////////////////////////
   /// \brief Get the mass of the object
   ///
   /// \return Mass of the object
   ///
   ////////////////////////////////////////////////////////////
   double GetMass() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the object
   ///
   /// \return Mean geometric radius of the object
   ///
   ////////////////////////////////////////////////////////////
   double GetRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the safe radius of the object
   ///
   /// \return Safe radius of the object
   ///
   ////////////////////////////////////////////////////////////
   double GetSafeRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the object
   ///
   /// \return Gravitational parameter of the object
   ///
   ////////////////////////////////////////////////////////////
   double GetGravitationalParameter() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the physical properties to a single-line formatted string
   ///
   /// The physical properties are converted to a single-line string
   /// with the following format:
   ///
   /// "m=[mass] r=[radius] sr=[safeRadius]"
   ///
   /// e.g.
   ///
   /// "m=[mass] r=[radius] sr=[safeRadius]"
   ///
   /// \returns std::string Stringified physical properties
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbital body to a detailed multi-line formatted string
   ///
   /// The orbital body is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Mass:                    [mass]
   ///  Equatorial Radius:       [radius]
   ///  Safe Equatorial Radius:  [safeRadius]
   ///  Gravitational Parameter: [gravitationalParameter]
   /// "
   ///
   /// e.g.
   ///
   /// "Mass:                    [1.0]
   ///  Equatorial Radius:       [6378.136300]
   ///  Safe Equatorial Radius:  [7015.949933]
   ///  Gravitational Parameter: [398600.441800]
   /// "
   ///
   /// where [PhysicalProperties], [Epoch], [Orbit], [IEphemeris], and [Time]
   /// are the results from calling the respective ToDetailedString()
   /// methods.
   ///
   /// \note Units are not shown because that information is not stored in the orbital body
   ///
   /// \see PhysicalProperties::ToDetailedString(), Epoch::ToDetailedString(), Orbit::ToDetailedString(), IEphemeris::ToDetailedString(), Time::ToDetailedString()
   ///
   /// \returns std::string Stringified orbital body
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString(std::string prefix = "") const;

private:
   double m_mass;       ///< Mass
   double m_radius;     ///< Equatorial radius
   double m_safeRadius; ///< Safe equatorial radius 
   double m_mu;         ///< Gravitational parameter
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates PhysicalProperties
///
/// The physical properties are converted to a string by calling the
/// PhysicalProperties::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const PhysicalProperties& physicalProperties)
{
   stream << physicalProperties.ToString();
   return stream;
}


} // namespace otl