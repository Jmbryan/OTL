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

// Forward declarations
class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

class IEphemerisBody : public OrbitalBody
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   IEphemerisBody();

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the ephemeris body
   ///
   /// \param name Name of the body
   /// \param physicalProperties PhysicalProperties of the body
   /// \param gravitationalParameterCentralBody Gravitational parameter of the central body
   /// \param stateVector Current StateVector of the body
   /// \param epoch Current Epoch of the body
   ///
   ////////////////////////////////////////////////////////////
   IEphemerisBody(const std::string& name,
                  const PhysicalProperties& physicalProperties,
                  double gravitationalParameterCentralBody,
                  const StateVector& stateVector,
                  const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~IEphemerisBody();

   ////////////////////////////////////////////////////////////
   /// \brief Get the physical properties of the body
   ///
   /// This function will call ForceInitialize() if
   /// not already initialized.
   ///
   /// \return PhysicalProperties of the body
   ///
   ////////////////////////////////////////////////////////////
   virtual const PhysicalProperties& GetPhysicalProperties() const override;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbit of the body
   ///
   /// This function will call ForceInitialize() if
   /// not already initialized.
   ///
   /// \return Current Orbit of the body
   ///
   ////////////////////////////////////////////////////////////
   virtual const keplerian::Orbit& GetOrbit() const override;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the body in time using ephemeris and propagation
   ///
   /// This function extends the OrbitalBody::Propagate()
   /// method by blending the ephemeris database with the
   /// propagation algorithm. A max propagation time can
   /// be specified which will force an ephemeris update
   /// whenever the elapsed propagation time exceeds it.
   /// The max propagation time is set by calling the
   /// SetMaxPropagationTime() function.
   ///
   /// Setting the max propagation time to Time::Infinity()
   /// (the default) disables the ephemeris updates and
   /// is identical to OrbitalBody::Propagate(). Likewise,
   /// setting the max propagation time to zero effectively
   /// disables the propagation algorithm and the ephemeris
   /// database is solely used instead.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   /// \see OrbitalBody::Propagate()
   ///
   /// \param timeDelta Propagation Time duration
   ///
   ////////////////////////////////////////////////////////////
   void BlendedPropagate(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the body to a given epoch using ephemeris and propagation
   ///
   /// Perform blended propagation to the desired epoch. 
   ///
   /// \see BlendedPropagate()
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void BlendedPropagateTo(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Set the max propagation time
   ///
   /// Maximum amount of time allowed to propagate before
   /// forcing an ephemeris update.
   ///
   /// \param maxTime Maximum propagation Time
   ///
   ////////////////////////////////////////////////////////////
   void SetMaxPropagationTime(const Time& maxTime);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the state vector of the body at a given epoch
   ///
   /// This function calls the VQueryStateVector() virtual method
   /// which must be re-implemented by derived classes.
   ///
   /// \param Epoch at which the state vector is desired
   /// \returns StateVector of the body
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& QueryStateVector(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Force initialization of ephemeris body
   ///
   /// Immediately retrieves the ephemeris data from the
   /// ephemeris database.
   ///
   /// \warn This function may result in file IO
   ///
   ////////////////////////////////////////////////////////////
   void ForceInitialize();

   ////////////////////////////////////////////////////////////
   /// \brief Converts the body to a single-line formatted string
   ///
   /// This function calls the base OrbitalBody::ToString()
   /// method.
   ///
   /// \see OrbitalBody::ToString()
   ///
   /// \returns std::string Stringified body
   ///
   ////////////////////////////////////////////////////////////
   virtual std::string ToString() const override;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the body to a detailed multi-line formatted string
   ///
   /// The body is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Max propagation time:
   ///     [Time]
   ///  Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// e.g.
   ///
   /// "Max propagatime time:
   ///     [Time]
   ///  Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// where [Time] and [OrbitalBody] are the results from calling
   /// the respective ToDetailedString() methods.
   ///
   /// \see Time::ToDetailedString(), OrbitalBody::ToDetailedString()
   ///
   /// \returns std::string Stringified body
   ///
   ////////////////////////////////////////////////////////////
   virtual std::string ToDetailedString(std::string prefix = "") const override;

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Initialize the body
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   ////////////////////////////////////////////////////////////
   virtual void VInitialize() = 0;

   ////////////////////////////////////////////////////////////
   /// \brief Get a smart pointer to the ephemeris database
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \return Smart pointer to IEphemeris database
   ///
   ////////////////////////////////////////////////////////////
   virtual EphemerisPointer VGetEphemeris() = 0;

   ////////////////////////////////////////////////////////////
   /// \brief Query the state vector of the body at a given epoch
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param epoch Desired Epoch
   /// \return StateVector of the body at the given Epoch
   ///
   ////////////////////////////////////////////////////////////
   virtual StateVector VQueryStateVector(const Epoch& epoch) = 0;
   
private:
   ////////////////////////////////////////////////////////////
   /// \brief Is an ephemeris update required?
   ///
   /// This function is used by the blended propagation
   /// methods to determine whether an ephemeris update
   /// is required based on the elapsed propagation time.
   ///
   /// \param timeDelta Next propagation time delta
   /// \return True if an ephemeris update is required, False otherwise
   ///
   ////////////////////////////////////////////////////////////
   bool IsEphemerisUpdateRequired(const Time& timeDelta);

private:
   mutable bool m_initialized;   ///< TRUE if the ephemeris data has been initialized
   Time m_maxPropagationTime;    ///< Max propagation time before next ephemeris update
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::JplApproximateBody
/// \ingroup otl
///
/// Interface class for all orbital bodies that use an ephemeris
/// database (JPL Approximate Ephemeris, MPCORB, SPICE, etc.).
///
/// \see OrbitalBody
///
////////////////////////////////////////////////////////////