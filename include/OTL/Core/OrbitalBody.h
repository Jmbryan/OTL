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
#include <OTL/Core/PhysicalProperties.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/Orbit.h>

namespace otl
{

// Forward declarations
class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;


class OTL_CORE_API OrbitalBody
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   OrbitalBody();

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the orbital body
   ///
   /// \param name Name of the orbital body
   /// \param physicalProperties PhysicalProperties of the orbital body
   /// \param gravitationalParameterCentralBody Gravitational parameter of the central body
   /// \param stateVector Current StateVector of the orbital body
   /// \param epoch Current Epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   explicit OrbitalBody(const std::string& name, const Epoch& epoch = Epoch());
   OrbitalBody(const std::string& name,
               const PhysicalProperties& physicalProperties,
               double gravitationalParameterCentralBody,
               const OrbitalElements& orbitalElements,
               const Epoch& epoch = Epoch());
   OrbitalBody(const std::string& name,
               const PhysicalProperties& physicalProperties,
               double gravitationalParameterCentralBody,
               const CartesianStateVector& cartesiantateVector,
               const Epoch& epoch = Epoch());
   //OrbitalBody(const std::string& name,
   //            const PhysicalProperties& physicalProperties,
   //            double gravitationalParameterCentralBody,
   //            const StateVector& stateVector,
   //            const Epoch& epoch = Epoch::MJD2000(0.0));


   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~OrbitalBody();
 
   ////////////////////////////////////////////////////////////
   /// \brief Set the propagation algorithm
   ///
   /// \param propagator Smart pointer to propagator
   ///
   ////////////////////////////////////////////////////////////
   //void SetPropagator(const PropagatorPointer& propagator);

   ////////////////////////////////////////////////////////////
   /// \brief Get the name of the orbital body
   ///
   /// \return Name of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   const std::string& GetName() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current epoch of the orbital body
   ///
   /// \return Current Epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Epoch& GetEpoch() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the physical properties of the orbital body
   ///
   /// \return PhysicalProperties of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const PhysicalProperties& GetPhysicalProperties() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbit of the orbital body
   ///
   /// \return Current Orbit of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const keplerian::Orbit& GetOrbit() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current state vector of the orbital body
   ///
   /// Returns a generic StateVector object whose type may be
   /// either StateVectorType::Orbital or StateVectorType::Cartesian.
   ///
   /// \return Current StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   //const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbital elements of the orbital body
   ///
   /// If the state vector is stored in StateVectorType::Cartesian
   /// format, then this function will convert from
   /// CartesianStateVector to OrbitalElements by calling the
   /// ConvertCartesianStateVector2OrbitalElements() function.
   /// The result is cached so subsequent calls will not suffer
   /// this overhead again until the state vector is modified
   /// by e.g. calling the Propagate() method.
   ///
   /// \return Current OrbitalElements of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;
   const OrbitalElements& GetOrbitalElementsAt(const Epoch& epoch);
   //OrbitalElements GetOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current cartesian state vector of the orbital body
   ///
   /// If the state vector is stored in StateVectorType::Orbital
   /// format, then this function will convert from
   /// OrbitalElements to CartesianStateVector by calling the
   /// ConvertOrbitalElements2CartesianStateVector() function.
   /// The result is cached so subsequent calls will not suffer
   /// this overhead again until the state vector is modified
   /// by e.g. calling the Propagate() method.
   ///
   /// \return Current CartesianStateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const CartesianStateVector& GetCartesianStateVector() const;
   const CartesianStateVector& GetCartesianStateVectorAt(const Epoch& epoch);
   //CartesianStateVector GetCartesianStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the type of the orbit
   ///
   /// Returns an Orbit::Type enumerator which indicates the type
   /// of the orbit (circular, elliptical, hyperbolic, etc.)
   ///
   /// \return OrbitType enumerator
   ///
   ////////////////////////////////////////////////////////////
   keplerian::Orbit::Type GetOrbitType() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbit radius of the orbital body
   ///
   /// \return Orbit radius of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   //double GetOrbitRadius() const;
   const keplerian::Orbit::OrbitProperties& GetOrbitProperties() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of central body being orbited
   ///
   /// \return Gravitational parameter of the central body
   ///
   ////////////////////////////////////////////////////////////
   double GetGravitationalParameterCentralBody() const;

   ////////////////////////////////////////////////////////////
   /// \brief Is the orbit of this type
   ///
   /// \return True if the orbit matches the specified Orbit::Type
   ///
   ////////////////////////////////////////////////////////////
   bool IsOrbitType(keplerian::Orbit::Type orbitType) const;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital body in time
   ///
   /// This function propagates the state vector of the orbital
   /// body in time using the internal propagation algorithm.
   /// The propagation algorithm defaults to KeplerianPropagator
   /// but can be specified by calling the SetPropagator() method.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Propagation Time duration
   ///
   ////////////////////////////////////////////////////////////
   //void Propagate(const Time& timeDelta);
   
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital body to a given epoch
   ///
   /// This function propagates the state vector of the orbital
   /// body to a given epoch using the internal propagation algorithm.
   /// The propagation algorithm defaults to KeplerianPropagator
   /// but can be specified by calling the SetPropagator() method.
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   //void PropagateTo(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbital body to a single-line formatted string
   ///
   /// The orbital body is converted to a single-line string
   /// with the following format:
   ///
   /// "name=[name] prop=[PhysicalProperties] epoch=[Epoch]"
   ///
   /// e.g.
   ///
   /// "name=Earth prop=[PhysicalProperties] epoch=[Epoch]"
   ///
   /// where [PhysicalProperties] and [Epoch] are the results
   /// from calling the PhysicalProperties::ToString() and
   /// Epoch::ToString() methods.
   ///
   /// \see PhysicalProperties::ToString(), Epoch::ToString()
   ///
   /// \returns std::string Stringified orbital body
   ///
   ////////////////////////////////////////////////////////////
   //virtual std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbital body to a detailed multi-line formatted string
   ///
   /// The orbital body is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Name: [name]
   ///  Physical properties:
   ///     [PhysicalProperties]
   ///  Epoch:
   ///     [Epoch]
   ///  Orbit:
   ///     [Orbit]
   ///  Ephemeris:
   ///     [IEphemeris]
   ///  Max propagation time:
   ///     [Time]
   /// "
   ///
   /// e.g.
   ///
   /// "Name: Earth
   ///  Physical properties:
   ///     [PhysicalProperties]
   ///  Epoch:
   ///     [Epoch]
   ///  Orbit:
   ///     [Orbit]
   ///  Ephemeris:
   ///     [IEphemeris]
   ///  Max propagation time:
   ///     [Time]
   /// "
   ///
   /// where [PhysicalProperties], [Epoch], [Orbit], [IEphemeris], and [Time]
   /// are the results from calling the respective ToDetailedString()
   /// methods.
   ///
   /// \note Units are not shown because that information is not stored in the orbital body
   ///
   /// \see PhysicalProperties::ToDetailedString(), Epoch::ToDetailedString(), Orbit::ToDetailedString(), IEphemeris::ToDetailedString(), Time::ToDetailedString()
   ///
   /// \returns std::string Stringified orbital body
   ///
   ////////////////////////////////////////////////////////////
   virtual std::string ToString(const std::string& prefix = "") const;
   //virtual std::string ToDetailedString(std::string prefix = "") const;

protected:
   void SetEpoch(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Set the physical properties of the orbital body
   ///
   /// \param physicalProperties PhysicalProperties of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetPhysicalProperties(const PhysicalProperties& physicalProperties);

   ////////////////////////////////////////////////////////////
   /// \brief Set the gravitational parameter of the central body
   ///
   /// \param gravitationalParameterCentralBody Gravitational parameter of the central body
   ///
   ////////////////////////////////////////////////////////////
   void SetGravitationalParameterCentralBody(double gravitationalParameterCentralBody);
   //void SetOrbit(const keplerian::Orbit& orbit);

   ////////////////////////////////////////////////////////////
   /// \brief Set the state vector of the orbital body
   ///
   /// \param stateVector StateVector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetOrbitalElements(const OrbitalElements& orbitalElements, keplerian::Orbit::Direction direction = keplerian::Orbit::Direction::Prograde);
   void SetCartesianStateVector(const CartesianStateVector& cartesianStateVector);
   //void SetStateVector(const StateVector& stateVector);

   void PropagateTo(const Epoch& epoch);
   void Propagate(const Time& timeDelta);
   virtual void VPropagateTo(const Epoch& epoch) = 0;

   void Initialize() const;
   virtual void VInitialize() = 0;

protected:
    Epoch m_epoch;                            ///< Current epoch of the orbital body
    PhysicalProperties m_physicalProperties;  ///< Physical properties of the orbital body
    keplerian::Orbit m_orbit;                 ///< Keplerian orbit of the orbital body
    
private:
   std::string m_name;                       ///< Name of the orbital body
   mutable bool m_initialized;
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Orbit
///
/// The orbital body is converted to a string by calling the
/// OrbitalBody::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const OrbitalBody& orbitalBody)
{
   stream << orbitalBody.ToString();
   return stream;
}

////////////////////////////////////////////////////////////
typedef std::shared_ptr<OrbitalBody> OrbitalBodyPointer;

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::OrbitalBody
/// \ingroup otl
///
/// Represents any point-mass object that is in a Keplerian
/// orbit around another object (e.g planets, moons, asteroids,
/// spacecraft, etc.).
///
/// The orbital body is comprised of the following components:
/// \li Name
/// \li PhysicalProperties
/// \li Epoch
/// \li Orbit
///
/// The state of the orbital body can be retrieved by calling
/// the following functions:
/// \li GetName()
/// \li GetPhysicalProperties()
/// \li GetEpoch()
/// \li GetOrbit()
///
/// For convenience, the following accessors are also exposed
/// from the Orbit class:
/// \li GetStateVector()
/// \li GetCartesianStateVector()
/// \li GetOrbitalElements()
/// \li GetOrbitRadius()
/// \li GetOrbitType()
///
/// The state of the orbital body can be propagated by a given
/// amount of Time or to a desired Epoch by calling the
/// Propagate() and PropagateTo() methods respectively.
///
/// Usage example:
/// \code
/// \endcode
///
/// \see PhysicalProperties, Epoch, Orbit
///
////////////////////////////////////////////////////////////