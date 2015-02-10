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

// Forward declarations
class JplEphemerisConverter;
typedef std::shared_ptr<JplEphemerisConverter> JplEphemerisConverterPointer;

class OTL_CORE_API JplEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit JplEphemeris(const std::string& dataFilename);

   ////////////////////////////////////////////////////////////
   /// \brief Disable copy constructor
   ////////////////////////////////////////////////////////////
   JplEphemeris(const JplEphemeris&) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Disable assignment operator
   ////////////////////////////////////////////////////////////
   JplEphemeris& operator=(const JplEphemeris&) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~JplEphemeris();

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

   void SetDataDirectory(const std::string& dataDirectory);
   void SetEntityList(const std::vector<std::string>& entityList);
   void CreateEphemerisFile(const Epoch& startDate, const Epoch& endDate, const std::string& outputFilename);

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

   virtual void VGetPhysicalProperties(const std::string& name, PhysicalProperties& physicalProperties) override;
   virtual void VGetGravitationalParameterCentralBody(const std::string& name, double& gravitationalParameterCentralBody) override;
   virtual void VGetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector) override;

private:
   std::string m_dataFilename;               ///< Full path to the ephemeris data file
   JplEphemerisConverterPointer m_converter; ///< Smart pointer to the converter helper object
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::JplEphemeris
/// \ingroup otl
///
/// \see IEphemeris, Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////