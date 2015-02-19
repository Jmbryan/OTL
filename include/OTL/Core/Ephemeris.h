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
#include <OTL/Core/Export.h>
#include <string>
#include <memory>
#include <mutex>

namespace otl
{

// Forward declarations
class PhysicalProperties;
class Epoch;
class StateVector;
class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

class OTL_CORE_API IEphemeris
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ////////////////////////////////////////////////////////////
    IEphemeris();
    //IEphemeris(const IEphemeris& other) = delete;
    //IEphemeris& operator=(const IEphemeris&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Constructor using data filename
    ///
    /// \param dataFilename Full path to ephemeris data file
    ///
    ////////////////////////////////////////////////////////////
    explicit IEphemeris(const std::string& dataFilename);
   
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ////////////////////////////////////////////////////////////
    virtual ~IEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Set the ephemeris data file and immediately load it into memory
    ///
    /// \param dataFilename Full path to ephemeris data file
    ///
    ////////////////////////////////////////////////////////////
    void LoadDataFile(const std::string& dataFilename);

    ////////////////////////////////////////////////////////////
    /// \brief Set the ephemeris data file
    ///
    /// This function uses lazy evaluation. The data file will
    /// not be loaded into memory until the next ephemeris query
    /// is made.
    ///
    /// \param dataFilename Full path to ephemeris data file
    ///
    ////////////////////////////////////////////////////////////
    void SetDataFilename(const std::string& dataFilename);

    ////////////////////////////////////////////////////////////
    /// \brief Get the ephemeris data file
    ///
    /// \return std::string full path to ephemeris data file
    ///
    ////////////////////////////////////////////////////////////
    const std::string& GetDataFilename() const;

    ////////////////////////////////////////////////////////////
    /// \brief Query the the physical properties of an entity
    ///
    /// \param name Name of entity
    /// \return PhysicalProperties of entity
    ///
    ////////////////////////////////////////////////////////////
    PhysicalProperties GetPhysicalProperties(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief Query the the gravitational parameter of an entity's central body
    ///
    /// \param name Name of entity
    /// \return Gravitational parameter of the entity's central body
    ///
    ////////////////////////////////////////////////////////////
    double GetGravitationalParameterCentralBody(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief Query the state vector of an entity at a given epoch
    ///
    /// \param name Name of entity
    /// \param epoch Epoch at which the state vector is desired
    /// \return Resulting StateVector
    ///
    ////////////////////////////////////////////////////////////
    StateVector GetStateVector(const std::string& name, const Epoch& epoch);

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris database
   ///
   /// This function is called before VInitialize() and loads the
   /// ephemeris database.
   ///
   /// This is a pure virtual method which must be reimplemented by
   /// the derived class.
   ///
   ////////////////////////////////////////////////////////////
    virtual void VLoad() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the ephemeris database
    ///
    /// This function is called after VLoad() and performs
    /// further initialization required by the ephemeris database.
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    ////////////////////////////////////////////////////////////
    virtual void VInitialize() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Returns true if the entity name is found in the ephemeris database
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Name of the entity
    /// \return True if the entity is supported by the ephemeris
    ///
    ////////////////////////////////////////////////////////////
    virtual bool VIsValidName(const std::string& name) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Returns true if the epoch is within the acceptable range for the ephemeris database
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param epoch Epoch
    /// \return True if the Epoch is supported by the ephemeris
    ///
    ////////////////////////////////////////////////////////////
    virtual bool VIsValidEpoch(const Epoch& epoch) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Query the the physical properties of an entity
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Name of entity
    /// \return PhysicalProperties of entity
    ///
    ////////////////////////////////////////////////////////////
    virtual PhysicalProperties VGetPhysicalProperties(const std::string& name) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Query the the gravitational parameter of an entity's central body
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Name of entity
    /// \return Gravitational parameter of the entity's central body
    ///
    ////////////////////////////////////////////////////////////
    virtual double VGetGravitationalParameterCentralBody(const std::string&  name) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Query the state vector of an entity at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Name of entity
    /// \param epoch Epoch at which the state vector is desired
    /// \return Resulting StateVector
    ///
    ////////////////////////////////////////////////////////////
    virtual StateVector VGetStateVector(const std::string& name, const Epoch& epoch) = 0;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Initialize the ephemeris database
   ///
   /// This function lazily initializes the ephemeris database
   /// by calling the virtual VLoad() and VInitialize()
   /// functions.
   ///
   ////////////////////////////////////////////////////////////
    void Initialize();

private:
    bool m_initialized;          ///< TRUE if the ephemeris database has been fully initialized
    std::mutex m_mutex;          ///< Mutex for thread safety
    std::string m_dataFilename;  ///< Full path to the ephemeris data file
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::IEphemeris
/// \ingroup otl
///
/// Interface class for all ephemeris databases
///
/// This class can be used to query information for an entity
/// belonging to an ephemeris database. The entities supported
/// depend on the the type of ephemeris (e.g. JPL Approximate,
/// MPCORB, SPICE, etc.). The following queries are supported
/// by all ephemeris databases:
/// \li GetPhysicalProperties() returns an entity's PhysicalProperties
/// \li GetGravitationalParameterCentralBody() returns the gravitational
///     parameter of an entity's central body
/// \li GetStateVector() returns an entity's StateVector at a given Epoch
/// 
/// \see PhysicalProperties, Epoch, StateVector, JplApproximateEphemeris, MpcorbEphemeris, SpiceEphemeris
///
////////////////////////////////////////////////////////////