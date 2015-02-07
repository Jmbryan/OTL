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
#include <OTL/Core/OrbitalBody.h>

namespace otl
{

class OTL_CORE_API Planet : public OrbitalBody
{
public:

   ////////////////////////////////////////////////////////////
   /// \brief Planetary body identifiers
   ////////////////////////////////////////////////////////////
   enum class PlanetId
   {
      Invalid = -1,  ///< Invalid planet identifier
      Mercury,       ///< Mercury
      Venus,         ///< Venus
      Earth,         ///< Earth
      Mars,          ///< Mars
      Jupiter,       ///< Jupiter
      Saturn,        ///< Saturn
      Uranus,        ///< Uranus
      Neptune,       ///< Neptune
      Pluto,         ///< Pluto
      Count          ///< Number of planets
   };

   ////////////////////////////////////////////////////////////
   /// Typedefs
   ////////////////////////////////////////////////////////////
   typedef std::map<PlanetId, std::pair<std::string, PhysicalProperties>> PlanetDictionary;

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Planet();

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from identifier
   ///
   /// The ephemeris database defaults to JplApproximateEphemeris.
   ///
   /// \param planetId Identifier of the planet to be created
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   explicit Planet(PlanetId planetId, const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from name
   ///
   /// The ephemeris database defaults to JplApproximateEphemeris.
   ///
   /// \param name Name of the planet to be created
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   explicit Planet(const std::string& name, const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from identifier and ephemeris database
   ///
   /// \param planetId Identifier of the planet to be created
   /// \param ephemeris Smart pointer to IEphemeris database
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   Planet(PlanetId planetId, const EphemerisPointer& ephemeris, const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from name and ephemeris database
   ///
   /// \param name Name of the planet to be created
   /// \param ephemeris Smart pointer to IEphemeris database
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   Planet(const std::string& name, const EphemerisPointer& ephemeris, const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Converts the planet to a single-line formatted string
   ///
   /// The planet is converted to a single-line string
   /// with the following format:
   ///
   /// "id=[id] name=[name] epoch=[Epoch]"
   ///
   /// e.g.
   ///
   /// "id=2 name=Earth epoch=[Epoch]"
   ///
   /// where [Epoch] is the result from calling the Epoch::ToString()
   /// method.
   ///
   /// \see Epoch::ToString()
   ///
   /// \returns std::string Stringified planet
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the planet to a detailed multi-line formatted string
   ///
   /// The planet is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Identifier: [id]
   ///  Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// e.g.
   ///
   /// "Identifier: 2
   ///  Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// where [OrbitalBody] is the result from calling the
   /// OrbitalBody::ToDetailedString() method.
   ///
   /// \see OrbitalBody::ToDetailedString()
   ///
   /// \returns std::string Stringified planet
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString(std::string prefix = "") const;

protected:
   virtual void VQueryPhysicalProperties() override;
   virtual void VQueryCentralBodyMu() override;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Initialize the planet of type planetId
   ///
   /// This function initializes the planet state
   /// and ephemeris database.
   ///
   /// \param planetId Identifier of the planet
   /// \param ephemeris Smart pointer to ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   void Initialize(PlanetId id, const EphemerisPointer& ephemeris);

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet dictionary
   ///
   /// This function creates a lookup table for retrieving
   /// the name, radius, and gravitational parameter of a
   /// planet given an identifier.
   ///
   ////////////////////////////////////////////////////////////
   //static PlanetDictionary CreatePlanetInfo();

private:
   PlanetId m_id; ///< Planet identifier enumerator
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Planet
///
/// The planet is converted to a string by calling the
/// Planet::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const Planet& planet)
{
   stream << planet.ToString();
   return stream;
}

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Planet::PlanetId
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template <typename T>
T& operator<<(T& stream, const Planet::PlanetId& planetId)
{
    stream << static_cast<int>(planetId);
    return stream;
}

////////////////////////////////////////////////////////////
/// \brief Helper function for converting a planet identifier into a planet name
///
/// \param planetId Planet::PlanetId enumerator identifier of the planet
///
////////////////////////////////////////////////////////////
std::string ConvertPlanetIdentifier2Name(Planet::PlanetId planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting a planet name into a planet identifier
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
Planet::PlanetId ConvertPlanetName2Identifier(const std::string& name);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param planetId Planet::PlanetId identifier code of the planet
///
////////////////////////////////////////////////////////////
PhysicalProperties GetPlanetPhysicalProperties(const Planet::PlanetId& planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
PhysicalProperties GetPlanetPhysicalProperties(const std::string& planetName);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Planet
/// \ingroup otl
///
/// Represents a solar system planetary body. The state
/// vector of the planet at a desired Epoch can be obtained
/// by calling the inherited member function QueryStateVector().
/// The state vector can then be retrieved in generic format
/// by calling GetStateVector(). Convenience functions for
/// retrieving the CartesianStateVector and OrbitalElements
/// are also provided:
/// \li GetCartesianStateVector()
/// \li GetOrbitalElements()
///
/// Internally, the state vector is determined based on
/// the ephemeris algorithm specified. By default, each planet
/// uses the JplApproximateEphemeris. However, a different
/// ephemeris can be used by calling the inherited
/// SetEphemeris() member function.
///
/// Usage example:
/// \code
/// // Create a planet representing Earth using the default JplApproximateEphemeris
/// otl::Planet planet(Planet::Earth);
/// myName = planet.GetName();
/// OTL_ASSERT(myName == "Earth");
///
/// // Query the state vector at the epoch Janurary 10, 2014
/// // The JplApproximateEphemeris returns the OrbitalElements,
/// // so a conversion is required to obtain the CartesianStateVector.
/// planet.QueryStateVector(Epoch::GregorianDateTime(2014, 1, 10));
/// auto myOrbitalElements1 = planet.GetOrbitalElements();
/// auto myCartesianStateVector1 = planet.GetCartesianStateVector(); // automatic conversion from orbital elements
///
/// // Propagate the planet forward 30 days.
/// planet.Propagate(Time::Days(30));
/// auto myOrbitalElements2 = planet.GetOrbitalElements();
/// auto myCartesianStateVector2 = planet.GetCartesianStateVector(); // automatic conversion from orbital elements
///
/// // Switch to JplEphemeris and then query the state vector at the epoch Janurary 10, 2015
/// // The JplEphemeris returns the CartesianStateVector,
/// // so a conversion is required to obtain the OrbitalElements.
/// planet.SetEphemeris(JplEphemeris(dataFilename));
/// planet.QueryStateVector(Epoch::GregorianDateTime(2015, 1, 10));
/// auto myOrbitalElements3 = planet.GetOrbitalElements(); // automatic conversion from cartesian state vector
/// auto myCartesianStateVector3 = planet.GetCartesianStateVector();
/// \endcode
///
/// \see OrbitalBody, IEphemeris, StateVector, OrbitalElements, CartesianStateVector
///
////////////////////////////////////////////////////////////