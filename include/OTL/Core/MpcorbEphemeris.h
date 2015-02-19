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
#include <OTL/Core/StateVector.h>
#include <OTL/Core/Epoch.h>

namespace otl
{

// Forward declarations
class IPropagator;
typedef std::shared_ptr<IPropagator> PropagatorPointer;

class OTL_CORE_API MpcorbEphemeris : public IEphemeris
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor using data file
   ///
   /// \param dataFilename Full path to ephemeris data file
   ///
   ////////////////////////////////////////////////////////////
   explicit MpcorbEphemeris(const std::string& dataFilename = "");

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~MpcorbEphemeris();

   ////////////////////////////////////////////////////////////
   /// \brief Set the propagator algorithm
   ///
   /// \param propagator Smart pointer to the propagator
   ///
   ////////////////////////////////////////////////////////////
   void SetPropagator(const PropagatorPointer& propagator);

   ////////////////////////////////////////////////////////////
   /// \brief Get the reference state vector for a given entity
   ///
   /// \param name Name of entity
   /// \return Reference StateVector of entity
   ///
   ////////////////////////////////////////////////////////////
   StateVector GetReferenceStateVector(const std::string& name);

   ////////////////////////////////////////////////////////////
   /// \brief Get the reference epoch for a given entity
   ///
   /// \param name Name of entity
   /// \return Reference Epoch of entity
   ///
   ////////////////////////////////////////////////////////////
   Epoch GetReferenceEpoch(const std::string& name);
   
protected:
   ////////////////////////////////////////////////////////////
   /// \brief Load the ephemeris database
   ///
   /// This function will attempt to load the MPCORB
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
   /// \param name Name of the entity
   /// \return True if the entity is supported by the ephemeris
   ///
   ////////////////////////////////////////////////////////////
   virtual bool VIsValidName(const std::string& name) override;

   ////////////////////////////////////////////////////////////
   /// \brief Returns true if the epoch is within the acceptable range for the ephemeris database
   ///
   /// This function always returns true for this ephemeris type.
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
   StateVector m_referenceStateVector; ///< Temporary variable for retrieving reference state vector
   Epoch m_referenceEpoch;                   ///< Temporary variable for retrieving reference epoch
   PropagatorPointer m_propagator;           ///< Smart pointer to propagator algorithm for propagating the reference state vector 
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::MpcorbEphemeris
/// \ingroup otl
///
/// \see IEphemeris, Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////