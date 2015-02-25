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
#include <OTL/Core/MpcorbEphemeris.h>

namespace otl
{

//// Forward declarations
//class MpcorbEphemeris;
//typedef std::shared_ptr<MpcorbEphemeris> MpcorbEphemerisPointer;

class OTL_CORE_API MpcorbBody : public OrbitalBody
{
public:
   MpcorbBody();
   explicit MpcorbBody(const std::string& name,
                       const Epoch& epoch = Epoch());
   MpcorbBody(const std::string& name,
              const MpcorbEphemeris& ephemeris,
              const Epoch& epoch = Epoch());

   ////////////////////////////////////////////////////////////
   /// \brief Set the MPCORB ephemeris database
   ///
   /// \param ephemeris Smart pointer to MPCORB ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   void SetEphemeris(const MpcorbEphemeris& ephemeris);

   void LoadEphemerisDataFile(const std::string& filename);

   //virtual std::string ToString() const override;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the body to a detailed multi-line formatted string
   ///
   /// The body is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// e.g.
   ///
   /// "Orbital Body:
   ///     [OrbitalBody]
   /// "
   ///
   /// where [OrbitalBody] is the result from calling the
   /// OrbitalBody::ToDetailedString() method.
   ///
   /// \see OrbitalBody::ToDetailedString()
   ///
   /// \returns std::string Stringified body
   ///
   ////////////////////////////////////////////////////////////
   virtual std::string ToString(const std::string& prefix = "") const override;  

//protected:
   virtual void VInitialize() override;
   virtual void VPropagateTo(const Epoch& epoch) override;
   //virtual EphemerisPointer VGetEphemeris() override;
   //virtual StateVector VQueryStateVector(const Epoch& epoch) override;
   
private:
   MpcorbEphemeris m_ephemeris;
   Epoch m_referenceEpoch;
   OrbitalElements m_referenceOrbitalElements;
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::MpcorbBody
/// \ingroup otl
///
/// Represents a solar system minor planetary body using the
/// MPCORB Ephemeris as described here:
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
/// The following alias is also provided for convenience:
/// typedef JplApproximateBody MinorPlanet
///
/// Usage example:
/// \code
/// // Create a minor planet representing Ceres
/// otl::MinorPlanet minorPlanet("Ceres");
///
/// // Retrieve some information about Ceres
/// auto dateOfDiscovery = minorPlanet.GetDateOfDiscovery();
/// auto dateOfLastObservation = minorPlanet.GetDateOfLasteObservation();
///
/// // Query the state vector at the epoch Janurary 10, 2014
/// // The MpcorbEphemeris returns the state vector in
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
/// \see IEphemerisBody, MpcorbEphemeris, StateVector, OrbitalElements, CartesianStateVector
///
////////////////////////////////////////////////////////////