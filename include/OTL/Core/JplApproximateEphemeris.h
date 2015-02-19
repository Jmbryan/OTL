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
#include <OTL/Core/Ephemeris.h>

namespace otl
{

class OTL_CORE_API JplApproximateEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data filename
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit JplApproximateEphemeris(const std::string& dataFilename = "");

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~JplApproximateEphemeris();
 
protected:
   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris database
   ///
   /// This function will attempt to load the JPL approximate
   /// ephemeris data from file if a valid filename is provided.
   /// Otherwise, the default hardcoded data will be used.
   ///
   ////////////////////////////////////////////////////////////
   virtual void VLoad() override;

   ////////////////////////////////////////////////////////////
   /// \brief Initialize the ephemeris database
   ///
   /// This function does not have any affect for this ephemeris
   /// type.
   ///
   ////////////////////////////////////////////////////////////
   virtual void VInitialize() override;

   ////////////////////////////////////////////////////////////
   /// \brief Returns true if the entity name is found in the ephemeris database
   ///
   /// This function returns true only for the major planets
   /// and Pluto.
   ///
   /// \param name Name of the entity
   /// \return True if the entity is supported by the ephemeris
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidName(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Returns true if the epoch is within the acceptable range for the ephemeris database
   ///
   /// If the default hardcoded data is used, this function
   /// will return true for epochs between 3000 BC and 3000 AD.
   ///
   /// \param epoch Epoch
   /// \return True if the Epoch is supported by the ephemeris
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidEpoch(const Epoch& epoch) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the the physical properties of an entity
   ///
   /// \param name Name of entity
   /// \return PhysicalProperties of entity
   ///
   ////////////////////////////////////////////////////////////
   virtual PhysicalProperties VGetPhysicalProperties(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the the gravitational parameter of an entity's central body
   ///
   /// \param name Name of entity
   /// \return Gravitational parameter of the entity's central body
   ///
   ////////////////////////////////////////////////////////////
   virtual double VGetGravitationalParameterCentralBody(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the state vector of an entity at a given epoch
   ///
   /// \param name Name of entity
   /// \param epoch Epoch at which the state vector is desired
   /// \return Resulting StateVector
   ///
   ////////////////////////////////////////////////////////////
   virtual StateVector VGetStateVector(const std::string& name, const Epoch& epoch) override;
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::JplApproximateEphemeris
/// \ingroup otl
///
/// Implements JPL's "Keplerian Elements for Approximate
/// Positions of the Major Planets".
///
/// This ephemeris routine analytically computes the orbital elements
/// of a planetary body at a given epoch using a simple database
/// consisting of two coefficients for each orbital element; namely
/// the reference element and the rate. The general equation is:
///
/// \f$ element = element_0 + rate * (num centures since J2000) \f$
///
/// The exception is for the computation of the mean anomaly for
/// Jupiter through Pluto which require four additional coefficients.
///
/// The physical properties, gravitational parameter of central body,
/// and state vector of a planet at a given Epoch can be obtained by
/// calling the inherited member functions:
/// \li GetPhysicalProperties()
/// \li GetGravitationalParameterCentralBody()
/// \li GetStateVector()
///
/// This routine is only valid for the major planets and Pluto. Querying
/// the database with any name other than those listed below will result
/// in an OTL_ERROR:
/// \li Mercury
/// \li Venus
/// \li Earth (Earth-Moon barycenter)
/// \li Mars
/// \li Jupiter
/// \li Saturn
/// \li Uranus
/// \li Neptune
/// \li Pluto
///
/// \note By default, this ephemeris is only valid for the time
/// period between 3000 BC to 3000 AD.
///
/// \note This routine is considered an approximate
/// (low precision) method. For a high precision ephemeris,
/// consider using the SpiceEphemeris class.
///
/// \reference http://ssd.jpl.nasa.gov/?planet_pos
///
/// \see IEphemeris, PhysicalProperties, Epoch, StateVector, SpiceEphemeris
///
////////////////////////////////////////////////////////////