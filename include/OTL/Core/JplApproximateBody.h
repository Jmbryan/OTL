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
#include <OTL/Core/JplApproximateEphemeris.h>

namespace otl
{

// Forward declarations
//class JplApproximateEphemeris;
//typedef std::shared_ptr<JplApproximateEphemeris> JplApproximateEphemerisPointer;

class OTL_CORE_API JplApproximateBody : public OrbitalBody
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   JplApproximateBody();

   ////////////////////////////////////////////////////////////
   /// \brief Create the body from name
   ///
   /// The JPL approximate ephemeris will be created using the
   /// default constructor.
   ///
   /// \param name Name of the body to be created
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   explicit JplApproximateBody(const std::string& name,
                               const Epoch& epoch = Epoch());

   ////////////////////////////////////////////////////////////
   /// \brief Create the body from name and JPL approximate ephemeris database
   ///
   /// \param name Name of the planet to be created
   /// \param ephemeris Smart pointer to JplApproximateEphemeris database
   /// \param epoch Initial Epoch of the planet
   ///
   ////////////////////////////////////////////////////////////
   JplApproximateBody(const std::string& name,
                      const JplApproximateEphemeris& ephemeris,
                      const Epoch& epoch = Epoch());

   ////////////////////////////////////////////////////////////
   /// \brief Set the JPL approximate ephemeris database
   ///
   /// \param ephemeris Smart pointer to JPL approximate ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   void SetEphemeris(const JplApproximateEphemeris& ephemeris);

   void LoadEphemerisDataFile(const std::string& filename);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the body to a single-line formatted string
   ///
   /// This function calls the base IEphemerisBody::ToString()
   /// method.
   ///
   /// \see IEphemerisBody::ToString()
   ///
   /// \returns std::string Stringified body
   ///
   ////////////////////////////////////////////////////////////
   //std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the body to a detailed multi-line formatted string
   ///
   /// This function calls the base IEphemerisBody::ToDetailedString()
   /// method.
   ///
   /// \see IEphemerisBody::ToDetailedString()
   ///
   /// \returns std::string Stringified body
   ///
   ////////////////////////////////////////////////////////////
   virtual std::string ToString(const std::string& prefix = "") const override;

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
   virtual void VPropagateTo(const Epoch& epoch) override;

   ////////////////////////////////////////////////////////////
   /// \brief Get a smart pointer to the JPL approximate ephemeris database
   ///
   /// Implements thre pure virtual function for retrieving a
   /// smart pointer to the ephemeris database.
   ///
   /// \return Smart pointer to IEphemeris database
   ///
   ////////////////////////////////////////////////////////////
   //virtual EphemerisPointer VGetEphemeris() override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the state vector of the body at a given epoch
   ///
   /// Implements the pure virtual function for querying the
   /// state vector of the body at a given epoch using the
   /// JPL approximate ephemeris database.
   ///
   /// \param epoch Desired Epoch
   /// \return StateVector of the body at the given Epoch
   ///
   ////////////////////////////////////////////////////////////
   //virtual StateVector VQueryStateVector(const Epoch& epoch) override;

private:
   OrbitalElements QueryOrbitalElementsAt(const Epoch& epoch);

private:
   JplApproximateEphemeris m_ephemeris; ///< JPL approximate ephemeris database
   //JplApproximateEphemerisPointer m_ephemeris;  ///< Smart pointer to JPL approximate ephemeris database
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates JplApproximateBody
///
/// The body is converted to a string by calling the
/// JplApproximateBody::ToString() method.
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