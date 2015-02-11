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

#ifdef OTL_SPICE
#pragma once
#include <OTL/Core/Ephemeris.h>

namespace otl
{

class OTL_CORE_API SpiceEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit SpiceEphemeris(const std::string& dataFileName,
                           const std::string& observerBodyName = "SUN",
                           const std::string& referenceFrameName = "J2000");

   ////////////////////////////////////////////////////////////
   /// \brief Disable copy constructor
   ////////////////////////////////////////////////////////////
   SpiceEphemeris(const SpiceEphemeris&) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Disable assignment operator
   ////////////////////////////////////////////////////////////
   SpiceEphemeris& operator=(const SpiceEphemeris&) = delete;

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~SpiceEphemeris();

   ////////////////////////////////////////////////////////////
   /// \brief Set the ephemeris data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   void SetDataFile(const std::string& dataFileName);

   ////////////////////////////////////////////////////////////
   /// \brief Set the reference frame
   ///
   /// \param referenceFrame Name of reference frame
   ///
   ////////////////////////////////////////////////////////////
   void SetReferenceFrame(const std::string& referenceFrameName);

   ////////////////////////////////////////////////////////////
   /// \brief Set the abberation corrections
   ///
   /// \param abberationCorrections Name of abberation corrections
   ///
   ////////////////////////////////////////////////////////////
   void SetAbberationCorrections(const std::string& abberationCorrectionsName);

   ////////////////////////////////////////////////////////////
   /// \brief Set the observer body
   ///
   /// \param observerBody Name of observer body
   ///
   ////////////////////////////////////////////////////////////
   void SetObserverBody(const std::string& observerBodyName);

   double GetBodyProperty(const std::string& targetBodyName, const std::string& propertyName);
   std::vector<double> GetBodyProperties(const std::string& targetBodyName,
                                         const std::string& propertyName,
                                         const int maxDimension = 10);

   OrbitalElements ConvertCartesianStateVectorToOrbitalElements(const StateVector& cartesianStateVector,
                                                                double gravitationalParameterCentralBody,
                                                                const Epoch& epoch);

   StateVector ConvertOrbitalElementsToCartesianStateVector(const OrbitalElements& orbitalElements,
                                                            double gravitationalParameterCentralBody,
                                                            const Epoch& epoch);

   int GetNumKernalsLoaded() const;

   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris data file into memory
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   void LoadDataFile(const std::string& dataFileName);

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
   /// \brief Is the entity name valid
   ///
   /// \param name Name of the entity in question
   /// \return True if the entity valid
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
   /// \brief Query the database for the position vector of an entity at a given epoch
   ///
   /// \param name Entity name
   /// \param epoch Time at which the position vector is desired
   /// \param [out] position Resulting position vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the velocity vector of an entity at a given epoch
   ///
   /// \param name Entity name
   /// \param epoch Time at which the velocity vector is desired
   /// \param [out] velocity Resulting velocity vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the state vector of an entity at a given epoch
   ///
   /// \param name Entity name
   /// \param epoch Time at which the state vector is desired
   /// \param [out] state Resulting state vector
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector) override;

   ////////////////////////////////////////////////////////////
   /// \brief Query the database for the orbital elements of an entity at a given epoch
   ///
   /// \param name Entity name
   /// \param epoch Time at which the orbital elements is desired
   /// \param [out] orbitalElements Resulting orbital elements
   ///
   ////////////////////////////////////////////////////////////
   virtual void VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements) override;

   virtual void VGetPhysicalProperties(const std::string& name, PhysicalProperties& physicalProperties) override;
   virtual void VGetGravitationalParameterCentralBody(const std::string& name, double& gravitationalParameterCentralBody) override;
   virtual void VGetStateVector(const std::string& name, const Epoch& epoch, test::StateVector& stateVector) override;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Calculate SPICE ephemeris time from epoch
   ///
   /// \param epoch Date to convert to ephemeris time
   ///
   ////////////////////////////////////////////////////////////
   double CalculateEphemerisTime(const Epoch& epoch) const;

private:
   std::string m_dataFilename;            ///< Full path to the ephemeris data file
   std::string m_observerBodyName;        ///< Name of observer body
   std::string m_referenceFrameName;      ///< Name of reference frame
   std::string m_abberationCorrections;   ///< Name of aberration corrections
   
};

} // namespace otl

#endif

////////////////////////////////////////////////////////////
/// \class otl::SpiceEphemeris
/// \ingroup otl
///
/// \see IEphemeris, Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////