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
#include <OTL/Core/NaturalBody.h>

namespace otl
{

class Planet : public NaturalBody
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
   /// \brief Simple data structure containing the basic characteristics of a planet.
   /// \ingroup otl
   /// 
   ////////////////////////////////////////////////////////////
   struct PlanetInfo
   {
      std::string name; ///< Name of the planet
      double radius;    ///< Radius of the planet
      double mu;        ///< Gravitational parameter of the planet

      ////////////////////////////////////////////////////////////
      /// \brief Default constructor
      ////////////////////////////////////////////////////////////
      PlanetInfo();

      ////////////////////////////////////////////////////////////
      /// \brief Create the planet info with the given parameters
      ///
      /// \param Name Name of the planet
      /// \param Radius Radius of the planet
      /// \param Mu Gravitational parameter of the planet
      ///
      ////////////////////////////////////////////////////////////
      PlanetInfo(const std::string& Name, double Radius, double Mu);
   };

   ////////////////////////////////////////////////////////////
   /// Typedefs
   ////////////////////////////////////////////////////////////
   typedef std::map<PlanetId, PlanetInfo> PlanetDictionary;

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Planet();

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet according to identifier
   ///
   /// \param planetId Identifier of the planet to be created
   ///
   ////////////////////////////////////////////////////////////
   Planet(PlanetId planetId);

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet according to name
   ///
   /// \param name Name of the planet to be created
   ///
   ////////////////////////////////////////////////////////////
   Planet(const std::string& name);

   ////////////////////////////////////////////////////////////
   /// \brief Get the state vector of the planet at the the epoch
   ///
   /// \param epoch Epoch at which state vector is desired
   /// \param stateVector Resulting state vector 
   ///
   ////////////////////////////////////////////////////////////
   //virtual void GetStateVectorAtEpoch(const Epoch& epoch, StateVector& stateVector);
   
   ////////////////////////////////////////////////////////////
   /// \brief Get the orbital elements of the planet at the the epoch
   ///
   /// \param epoch Epoch at which state vector is desired
   /// \param orbitalElements Resulting orbital elements 
   ///
   ////////////////////////////////////////////////////////////
   //virtual void GetOrbitalElementsAtEpoch(const Epoch& epoch, OrbitalElements& orbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Convert a planet name into an planet identifier
   ///
   /// \param name Name of the planet
   ///
   ////////////////////////////////////////////////////////////
   static PlanetId ConvertName2Identifier(const std::string& name);

   ////////////////////////////////////////////////////////////
   /// \brief Convert a planet identifier into an planet name
   ///
   /// \param planetId Identifier of the planet
   ///
   ////////////////////////////////////////////////////////////
   static std::string ConvertIdentifier2Name(PlanetId planetId);

private:
   ////////////////////////////////////////////////////////////
   /// \brief Initialize the planet of type planetId
   ///
   /// This function retrieves the name, radius, and gravitational
   /// parameter of the planet given an identifier.
   ///
   /// \param planetId Identifier of the planet
   ///
   ////////////////////////////////////////////////////////////
   void Initialize(PlanetId id);

   ////////////////////////////////////////////////////////////
   /// \brief Create the planet dictionary
   ///
   /// This function creates a lookup table for retrieving
   /// the name, radius, and gravitational parameter of a
   /// planet given an identifier.
   ///
   ////////////////////////////////////////////////////////////
   static PlanetDictionary CreatePlanetInfo();

private:
   ////////////////////////////////////////////////////////////
   // Private Member data
   ////////////////////////////////////////////////////////////
   PlanetId m_id; ///< Planet identifier
   static const PlanetDictionary m_planetInfo; // Planet info lookup table
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Planet
/// \ingroup otl
///
/// Represents a solar system planetary body. The
/// state vector and orbital elements of the planet at a
/// desired Epoch can be obtained by calling the inherited
/// member functions:
/// \li SetEpoch()
/// \li GetStateVector()
/// \li GetOrbitalElements()
///
/// Internally, the state vector and orbital elements are
/// determined based on the ephemeris algorithm specified.
/// By default, each planet instantiates its own instance
/// of the JplApproximateEphemeris. However, a different
/// ephemeris can be used by calling the inherited
/// SetEphemeris() member function.
///
/// \note By default, the UseEphemerisForPropagation option
/// is enabled which uses the IEphemeris object instead of
/// the IPropagator object for propagation. This option can
/// be toggled by calling the inherited UseEphemerisForPropagation()
/// member function.
///
/// Usage example:
/// \code
/// otl::Planet planet(Planet::Earth);
/// myName = planet.GetName();
///
/// Epoch myEpoch = Epoch::GregorianDateTime(2014, 1, 10); // Set date to Jan 10, 2014
/// planet.SetEpoch(myEpoch);
///
/// Query the state vector at the current epoch.
/// StateVector myStateVector;
/// planet.GetStateVector(myStateVector);
///
/// // Propagate the planet forward 30 days.
/// // Internally, since the UseEphemerisForPropagation option
/// // is enabled by default, this will add 30 days to the current
/// // epoch and then query the ephemeris database for the state
/// // vector at the new epoch.
/// planet.Propagate(Time::Days(30));
///
/// Retrieve the orbital elements by automatically converting
/// the state vector.
/// OrbitalElements myOrbitalElements;
/// planet.GetOrbitalElements(myOrbitalElements); 
/// \endcode
///
/// \see OrbitalBody, Ephemeris, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////