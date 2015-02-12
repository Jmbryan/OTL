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
#include <OTL/Core/EphemerisBody.h>

namespace otl
{

// Forward declarations
class JplApproximateEphemeris;
typedef std::shared_ptr<JplApproximateEphemeris> JplApproximateEphemerisPointer;

class OTL_CORE_API JplApproximateBody : public IEphemerisBody
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   JplApproximateBody();

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from name
   ///
   /// The ephemeris database defaults to JplApproximateEphemeris.
   ///
   /// \param name Name of the planet to be created
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   explicit JplApproximateBody(const std::string& name,
                               const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet from name and ephemeris database
   ///
   /// \param name Name of the planet to be created
   /// \param ephemeris Smart pointer to IEphemeris database
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   JplApproximateBody(const std::string& name,
                      const JplApproximateEphemerisPointer& ephemeris,
                      const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Set the JPL approximate ephemeris database
   ///
   /// \param ephemeris Smart pointer to JPL approximate ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   void SetEphemeris(const JplApproximateEphemerisPointer& ephemeris);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the planet to a single-line formatted string
   ///
   /// The planet is converted to a single-line string
   /// with the following format:
   ///
   /// "name=[name] epoch=[Epoch]"
   ///
   /// e.g.
   ///
   /// "name=Earth epoch=[Epoch]"
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
   /// "Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// e.g.
   ///
   /// "Orbital Body:
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
   virtual void VInitialize() override;
   virtual EphemerisPointer VGetEphemeris() override;
   virtual test::StateVector VQueryStateVector(const Epoch& epoch) override; 

private:
   JplApproximateEphemerisPointer m_ephemeris;  ///< Smart pointer to ephemeris database
};

/// Convenience alias
typedef JplApproximateBody Planet;

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
T& operator<<(T& stream, const JplApproximateBody& planet)
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
T& operator<<(T& stream, const PlanetId& planetId)
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
OTL_CORE_API std::string ConvertPlanetIdentifier2Name(PlanetId planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting a planet name into a planet identifier
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PlanetId ConvertPlanetName2Identifier(const std::string& name);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param planetId Planet::PlanetId identifier code of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PhysicalProperties GetPlanetPhysicalProperties(const PlanetId& planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PhysicalProperties GetPlanetPhysicalProperties(const std::string& planetName);

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::JplApproximateBody
/// \ingroup otl
///
/// Represents a solar system major planetary body using the
/// "JPL Approximate Ephemeris" as described here:
/// [TODO]: reference
/// 
/// The state vector of the body at a desired Epoch can be obtained
/// by calling the inherited member function QueryStateVector().
/// Convenience functions for retrieving the state vector and
/// specific variants are also provided:
/// \li GetStateVector()
/// \li GetCartesianStateVector()
/// \li GetOrbitalElements()
///
/// The following alias is also provided for convenience:
/// typedef JplApproximateBody Planet
///
/// Usage example:
/// \code
/// // Create a planet representing Earth
/// otl::Planet planet(PlanetId::Earth);
/// OTL_ASSERT(planet.GetName() == "Earth");
///
/// // Query the state vector at the epoch Janurary 10, 2014
/// // The JplApproximateEphemeris returns the state vector in
/// StateVectorType::Orbital format, so a conversion is required
/// to obtain the CartesianStateVector.
/// planet.QueryStateVector(Epoch::GregorianDateTime(2014, 1, 10));
/// auto myOrbitalElements1 = planet.GetOrbitalElements();
/// auto myCartesianStateVector1 = planet.GetCartesianStateVector(); // automatic conversion from orbital elements
///
/// // Propagate the planet forward 30 days.
/// planet.Propagate(Time::Days(30));
/// auto myOrbitalElements2 = planet.GetOrbitalElements();
/// auto myCartesianStateVector2 = planet.GetCartesianStateVector(); // automatic conversion from orbital elements
///
/// \endcode
///
/// \see IEphemerisBody, JplApproximateEphemeris, StateVector, OrbitalElements, CartesianStateVector
///
////////////////////////////////////////////////////////////