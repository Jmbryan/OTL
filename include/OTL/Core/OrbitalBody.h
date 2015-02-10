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
#include <OTL/Core/Orbit.h>
#include <OTL/Core/Epoch.h>

namespace otl
{

// Forward declarations
class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

class PhysicalProperties
{
public:
   PhysicalProperties();
   PhysicalProperties(double mass, double radius);
   PhysicalProperties(double mass, double radius, double safeRadius);

   double GetMass() const;
   double GetRadius() const;
   double GetSafeRadius() const;   
   double GetGravitationalParameter() const;

   std::string ToString() const;
   std::string ToDetailedString(std::string prefix = "") const;

private:
   double m_mass;       ///< Mass
   double m_radius;     ///< Equatorial radius
   double m_safeRadius; ///< Safe equatorial radius 
   double m_mu;         ///< Gravitational parameter
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates PhysicalProperties
///
/// The physical properties are converted to a string by calling the
/// PhysicalProperties::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const PhysicalProperties& physicalProperties)
{
   stream << physicalProperties.ToString();
   return stream;
}


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
   OrbitalBody(const std::string& name,
               const PhysicalProperties& physicalProperties,
               double gravitationalParameterCentralBody,
               const test::StateVector& stateVector,
               const Epoch& epoch = Epoch::MJD2000(0.0));


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
   void SetPropagator(const PropagatorPointer& propagator);

   ////////////////////////////////////////////////////////////
   /// \brief Set the ephemeris database
   ///
   /// \param ephemeris Smart pointer to ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   void SetEphemeris(const EphemerisPointer& ephemeris);

   ////////////////////////////////////////////////////////////
   /// \brief Set the max propagation time
   ///
   /// Maximum amount of time allowed to propagate before
   /// forcing an ephemeris update. Only applicable if
   /// an ephemeris database has been specified by calling
   /// the SetEphemeris() method.
   ///
   /// \param maxTime Maximum propagation Time
   ///
   ////////////////////////////////////////////////////////////
   void SetMaxPropagationTime(const Time& maxTime);

   ////////////////////////////////////////////////////////////
   /// \brief Get the name of the orbital body
   ///
   /// \return Name of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   const std::string& GetName() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the physical properties of the orbital body
   ///
   /// \return PhysicalProperties of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const PhysicalProperties& GetPhysicalProperties() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of central body being orbited
   ///
   /// \return Gravitational parameter of the central body
   ///
   ////////////////////////////////////////////////////////////
   double GetGravitationalParameterCentralBody() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current cartesian state vector of the orbital body
   ///
   /// If the internal state vector is stored in StateVectorType::Orbital
   /// format, then this function will convert from
   /// OrbitalElements to CartesianStateVector by calling the
   /// ConvertOrbitalElements2CartesianStateVector() function.
   /// However, the result is cached so subsequent calls will not
   /// suffer this overhead until the state vector is modified by
   /// either calling the Propagate() or QueryStateVector() methods.
   ///
   /// If the state vector has not been specified through the
   /// constructor or QueryStateVector() method, then this
   /// function will return a StateVector of all zeros.
   ///
   /// \return Current CartesianStateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   StateVector GetCartesianStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbital elements of the orbital body
   ///
   /// If the internal state vector is stored in StateVectorType::Cartesian
   /// format, then this function will convert from
   /// CartesianStateVector to OrbitalElements by calling the
   /// ConvertCartesianStateVector2OrbitalElements() function.
   /// However, the result is cached so subsequent calls will not
   /// suffer this overhead until the state vector is modified by
   /// either calling the Propagate() or QueryStateVector() methods.
   ///
   /// If the state vector has not been specified through the
   /// constructor or QueryStateVector() method, then this
   /// function will return a StateVector of all zeros.
   ///
   /// \return Current OrbitalElements of the orbit
   ///
   ////////////////////////////////////////////////////////////
   OrbitalElements GetOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current state vector of the orbital body
   ///
   /// Returns a generic StateVector object whose type may be
   /// either StateVectorType::Orbital or StateVectorType::Cartesian.
   ///
   /// If the state vector has not been specified through the
   /// constructor or QueryStateVector() method, then this
   /// function will return a StateVector of all zeros.
   ///
   /// \return Current StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const test::StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbit of the orbital body
   ///
   /// Returns the Orbit member variable which contains
   /// the state vector, gravitational parameter of the
   /// central body, and propagation algorithm.
   ///
   /// \return Orbit of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const keplerian::Orbit& GetOrbit() const;

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
   double GetOrbitRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current epoch of the orbital body
   ///
   /// \return Current Epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Epoch& GetEpoch() const;

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
   void Propagate(const Time& timeDelta);
   
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital body to a given epoch
   ///
   /// This function propagates the state vector of the orbital
   /// body to a given epoch using the internal propagation algorithm.
   /// The propagation algorithm defaults to KeplerianPropagator
   /// but can be specified by calling the SetPropagator() method.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Propagation Time duration
   ///
   ////////////////////////////////////////////////////////////
   void PropagateTo(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the physical properties of the orbital body
   ///
   /// Immediately execute the database query and return the
   /// result. This function calls the VQueryPhysicalProperties()
   /// virtual method which may be re-implemented by derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   /// \returns PhysicalProperties of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const PhysicalProperties& QueryPhysicalProperties();

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the gravitational parameter of the central body
   ///
   /// Immediately execute the database query and return the
   /// result. This function calls the VQueryPhysicalProperties()
   /// virtual method which may be re-implemented by derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   /// \returns Gravitational parameter of the central body
   ///
   ////////////////////////////////////////////////////////////
   double QueryCentralBodyMu();

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the state vector of the orbital body at a given epoch
   ///
   /// Immediately execute the database query and return the
   /// result. This function calls the VQueryPhysicalProperties()
   /// virtual method which may be re-implemented by derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   /// \param Epoch at which the state vector is desired
   /// \returns StateVector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const test::StateVector& QueryStateVector(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Lazily query the ephemeris database for the physical properties of the orbital body
   ///
   /// Lazy evaluation is used. This means that instead of
   /// immediately executing the database query, the query
   /// command gets stored in a queue which doesn't get
   /// processed until the physical properties are actually needed.
   ///
   /// When it comes time to process the query, the VQueryPhysicalProperties()
   /// virtual method is called which may be re-implemented by
   /// derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   ////////////////////////////////////////////////////////////  
   void LazyQueryPhysicalProperties();

   ////////////////////////////////////////////////////////////
   /// \brief Lazily query the ephemeris database for the gravitational parameter of the central body
   ///
   /// Lazy evaluation is used. This means that instead of
   /// immediately executing the database query, the query
   /// command gets stored in a queue which doesn't get
   /// processed until the central body gravitational parameter
   /// is actually needed.
   ///
   /// When it comes time to process the query, the VQueryCentralBodyMu()
   /// virtual method is called which may be re-implemented by
   /// derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   ////////////////////////////////////////////////////////////
   void LazyQueryCentralBodyMu();

   ////////////////////////////////////////////////////////////
   /// \brief Lazily query the ephemeris database for the state vector at a given epoch
   ///
   /// Lazy evaluation is used. This means that instead of
   /// immediately executing the database query, the query
   /// command gets stored in a queue which doesn't get
   /// processed until the state vector is actually needed.
   ///
   /// When it comes time to process the query, the VQueryStateVector()
   /// virtual method is called which may be re-implemented by
   /// derived classes.
   ///
   /// This function is only applicable if an ephemeris database
   /// has been specified by calling the SetEphemeris() method.
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void LazyQueryStateVector(const Epoch& epoch);

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
   std::string ToString() const;

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
   std::string ToDetailedString(std::string prefix = "") const;

protected:
   typedef std::function<void(void)> DelayedCommand;
   typedef std::map<std::string, DelayedCommand> DelayedCommandList;

   virtual void VPropagate(const Time& timeDelta); 
   virtual void VQueryPhysicalProperties();
   virtual void VQueryCentralBodyMu();
   virtual void VQueryStateVector(const Epoch& epoch);

   void SetPhysicalProperties(const PhysicalProperties& physicalProperties);
   void SetGravitationalParameterCentralBody(double centralBodyMu);
   void SetStateVector(const test::StateVector& stateVector);

   void AddDelayedCommand(const std::string& name, const DelayedCommand& delayedCommand);
   void RemoveDelayedCommand(const std::string& name) const;

private:
   void PropagateEpoch(const Time& timeDelta);
   bool IsEphemerisUpdateRequired(const Time& timeDelta);

   void ExecuteDelayedCommand(const std::string& name) const;
   void ExecuteAllDelayedCommands() const;
   
private:
    std::string m_name;                       ///< Name of the orbital body
    PhysicalProperties m_physicalProperties;  ///< Physical properties of the orbital body
    Epoch m_epoch;                            ///< Current epoch of the orbital body
    keplerian::Orbit m_orbit;                 ///< Keplerian orbit of the orbital body
    EphemerisPointer m_ephemeris;             ///< Smart pointer to ephemeris database
    Time m_maxPropagationTime;                ///< Max propagation time before next ephemeris update
    
    mutable DelayedCommandList m_delayedCommands;
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
/// Class that represents any point-mass object that orbits
/// another object (e.g planets, asteroids, spacecraft, etc.)
///
/// The orbital body can be propagated 
///
/// The state vector and orbital elements of the orbital
/// body can be obtained by calling the methods:
///
/// \li GetStateVector()
/// \li GetOrbitalElements()
///
/// This class is an abstract base class and cannot be instantiated.
///
/// \see Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////