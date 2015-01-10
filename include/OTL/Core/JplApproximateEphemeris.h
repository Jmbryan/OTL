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

class JplApproximateEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   JplApproximateEphemeris();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit JplApproximateEphemeris(const std::string& dataFilename);

   ////////////////////////////////////////////////////////////
   /// \brief Disable copy constructor
   ////////////////////////////////////////////////////////////
   JplApproximateEphemeris(const JplApproximateEphemeris& other) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Disable assignment operator
   ////////////////////////////////////////////////////////////
   JplApproximateEphemeris& operator=(const JplApproximateEphemeris&) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~JplApproximateEphemeris();

   ////////////////////////////////////////////////////////////
   /// \brief Set the ephemeris data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   void SetDataFile(const std::string& dataFilename);

   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris data file into memory
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   void LoadDataFile(const std::string& dataFilename);
   
protected:
   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris data file into memory
   ///
   /// Performs database file IO. This function lazily
   /// evalulated when the first ephemeris query is made and
   /// before VInitialize().
   ///
   ////////////////////////////////////////////////////////////
   virtual void VLoad() override;

   ////////////////////////////////////////////////////////////
   /// \brief Initialize the ephemeris
   ///
   /// Performs post-initialization. This function lazily
   /// evalulated when the first ephemeris query is made and
   /// after VLoad().
   ///
   ////////////////////////////////////////////////////////////
   virtual void VInitialize() override;

   ////////////////////////////////////////////////////////////
   /// \brief Is the planet name valid
   ///
   /// \param name Name of the planet in question
   /// \return True if the planet valid
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidName(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Is the epoch valid
   ///
   /// \param epoch Epoch in question
   /// \return True if the epoch valid
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidEpoch(const Epoch& epoch) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the position vector of a planet at a given epoch
   ///
   /// This function is only applicable to the major planets and Pluto.
   ///
   /// \param name Planet name
   /// \param epoch Time at which the position vector is desired
   /// \param [out] position Resulting position vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the velocity vector of a planet at a given epoch
   ///
   /// This function is only applicable to the major planets and Pluto.
   ///
   /// \param name Planet name
   /// \param epoch Time at which the velocity vector is desired
   /// \param [out] velocity Resulting velocity vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the state vector of a planet at a given epoch
   ///
   /// This function is only applicable to the major planets and Pluto.
   ///
   /// \param name Planet name
   /// \param epoch Time at which the state vector is desired
   /// \param [out] stateVector Resulting state vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the orbital elements of a planet at a given epoch
   ///
   /// This function is only applicable to the major planets and Pluto.
   ///
   /// \param name Planet name
   /// \param epoch Time at which the state vector is desired
   /// \param [out] orbitalElements Resulting orbital elements
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements) override;
    
private:
   std::string m_dataFilename; ///< Full path to the ephemeris data file
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
/// The benefit of using an analytical ephemeris versus querying a
/// traditional high precision ephemeris database such as SPICE is
/// that it is typically much faster (approximatly an order of magnitude),
/// but at the cost of precision. However, applications such as trajectory
/// design and optimization do not require high precision ephemeris and
/// can greatly benefity from the speedup offered by an analytical ephemeris
/// alternative.
///
/// The position, velocity, state vector, and orbital elements
/// of a planet at a desired Epoch can be obtained by calling
/// the inherited member functions:
/// \li GetPosition()
/// \li GetVelocity()
/// \li GetStateVector()
/// \li GetOrbitalElements()
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
/// \note This routine is only valid for the time
/// period between 3000 BC to 3000 AD.
///
/// \note This routine is considered an approximate
/// (low precision) method. For a high precision ephemeris,
/// consider using the SpiceEphemeris class.
///
/// \reference http://ssd.jpl.nasa.gov/?planet_pos
///
/// \see IEphemeris, Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////