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
#include <OTL/Core/Matrix.h>
#include <vector>

namespace otl
{

// Forward declarations
struct OrbitalElements;
struct CartesianStateVector;

class OTL_CORE_API SpiceEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit SpiceEphemeris(const std::string& dataFilename = "",
                           const std::string& observerBodyName = "SUN",
                           const std::string& referenceFrameName = "J2000");


   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~SpiceEphemeris();

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

   OrbitalElements ConvertCartesianStateVectorToOrbitalElements(const CartesianStateVector& cartesianStateVector,
                                                                double gravitationalParameterCentralBody,
                                                                const Epoch& epoch);

   CartesianStateVector ConvertOrbitalElementsToCartesianStateVector(const OrbitalElements& orbitalElements,
                                                            double gravitationalParameterCentralBody,
                                                            const Epoch& epoch);

   int GetNumKernalsLoaded() const;

   Vector3d GetPosition(const std::string& name, const Epoch& epoch) const;

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris database
   ///
   /// This function will attempt to load the SPICE
   /// ephemeris data from file if a valid filename is provided.
   /// Otherwise, OTL_ERROR will be called.
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
   /// The supported entities depend on the SPICE kernal(s) loaded.
   ///
   /// \param name Name of the entity
   /// \return True if the entity is supported by the ephemeris
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidName(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Returns true if the epoch is within the acceptable range for the ephemeris database
   ///
   /// The supported epochs depend on the SPICE kernal(s) loaded.
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

private:
   ////////////////////////////////////////////////////////////
   /// \brief Calculate SPICE ephemeris time from epoch
   ///
   /// \param epoch Epoch to convert to ephemeris time
   ///
   ////////////////////////////////////////////////////////////
   double CalculateEphemerisTime(const Epoch& epoch) const;

private:
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