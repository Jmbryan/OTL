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

struct PhysicalProperties
{
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
   /// \param stateVector Current state vector of the orbital body
   /// \param epoch Current epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   OrbitalBody(const std::string& name,
               const PhysicalProperties& physicalProperties,
               double gravitationalParameterCentralBody,
               const StateVector& stateVector,
               const Epoch& epoch = Epoch::MJD2000(0.0));

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the orbital body
   ///
   /// \param name Name of the orbital body
   /// \param physicalProperties PhysicalProperties of the orbital body
   /// \param gravitationalParameterCentralBody Gravitational parameter of the central body
   /// \param orbitalElements Current orbital elements of the orbital body
   /// \param epoch Current epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   OrbitalBody(const std::string& name,
               const PhysicalProperties& physicalProperties,
               double gravitationalParameterCentralBody,
               const OrbitalElements& orbitalElements,
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
   /// \brief Get the position vector of the orbital body
   ///
   /// \return Current position vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetPosition() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the velocity vector of the orbital body
   ///
   /// \return Current velocity vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetVelocity() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the state vector of the orbital body
   ///
   /// \return Current StateVector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbital elements of the orbital body
   ///
   /// \return Current OrbitalElements of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbit of the orbital body
   ///
   /// \return Current Orbit of the orbital body
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
   /// \brief Get the orbital radius of the orbital body
   ///
   /// \return Orbital radius of the orbital body
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
   /// \brief Propagate the orbital body for a desired length of time
   ///
   /// Generic function for propagating the state of the orbital body.
   /// The PropagationType determines whether the orbital elements
   /// or state vector is propagated.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   /// \param propagationType PropagationType which determines whether the orbital elements or state vector is propagated
   ///
   ////////////////////////////////////////////////////////////
   void Propagate(const Time& timeDelta, const PropagationType& propagationType = PropagationType::OrbitalElements);
   
   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital body to a desired epoch
   ///
   /// Generic function for propagating the state of the orbital body.
   /// The PropagationType determines whether the orbital elements
   /// or state vector is propagated.
   ///
   /// \param epoch Desired Epoch
   /// \param propagationType PropagationType which determines whether the orbital elements or state vector is propagated
   ///
   ////////////////////////////////////////////////////////////
   void PropagateTo(const Epoch& epoch, const PropagationType& propagationType = PropagationType::OrbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital elements for a desired length of time
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   ///
   ////////////////////////////////////////////////////////////
   void PropagateOrbitalElements(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital elements to a desired epoch
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void PropagateOrbitalElementsTo(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector for a desired length of time
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   ///
   ////////////////////////////////////////////////////////////
   void PropagateStateVector(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the state vector to a desired epoch
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void PropagateStateVectorTo(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database at a desired epoch
   ///
   /// Generic function for querying the ephemeris database.
   /// The EphemerisQueryType determines what is queried from
   /// the database (e.g orbital elements, state vector, physical
   /// properties, etc.).
   ///
   /// \note This function is only applicable if an ephemeris has been specified by calling the SetEphemeris() method
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void QueryEphemeris(const Epoch& epoch, const EphemerisQueryType& queryType = EphemerisQueryType::OrbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the orbital elements at a desired epoch
   ///
   /// \note This function is only applicable if an ephemeris has been specified by calling the SetEphemeris() method
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void QueryOrbitalElements(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the state vector at a desired epoch
   ///
   /// \note This function is only applicable if an ephemeris has been specified by calling the SetEphemeris() method
   ///
   /// \param epoch Desired Epoch
   ///
   ////////////////////////////////////////////////////////////
   void QueryStateVector(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Query the ephemeris database for the orbital body's physical properties
   ///
   /// \note This function is only applicable if an ephemeris has been specified by calling the SetEphemeris() method
   ///
   ////////////////////////////////////////////////////////////
   void QueryPhysicalProperties();

   std::string ToString() const;
   std::string ToDetailedString(std::string prefix = "") const;

protected:
   virtual void VPropagate(const Time& timeDelta, const PropagationType& propagationType);
   virtual void VPropagateOrbitalElements(const Time& timeDelta);
   virtual void VPropagateStateVector(const Time& timeDelta);
   virtual void VQueryEphemeris(const Epoch& epoch, const EphemerisQueryType& queryType);
   virtual void VQueryOrbitalElements(const Epoch& epoch);
   virtual void VQueryStateVector(const Epoch& epoch);
   virtual void VQueryPhysicalProperties();

   virtual bool VInitializeEphemeris();

private:
   bool IsEphemerisUpdateRequired();
   void PropagateEpoch(const Time& timeDelta);
   void PropagateEpochTo(const Epoch& epoch);

private:
    std::string m_name;                       ///< Name of the orbital body
    PhysicalProperties m_physicalProperties;  ///< Physical properties of the orbital body
    Epoch m_epoch;                            ///< Current epoch of the orbital body
    keplerian::Orbit m_orbit;                 ///< Keplerian orbit of the orbital body
    EphemerisPointer m_ephemeris;             ///< Smart pointer to ephemeris database
    Time m_maxPropagationTime;                ///< Max propagation time before next ephemeris update
    Time m_elapsedPropagationTime;            ///< Elapsed propagation time since last ephemeris update
    bool m_ephemerisInitialized;              ///< True if the ephemeris database has been initialized
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
inline void OrbitalBody::Propagate(const Time& timeDelta, const PropagationType& propagationType)
{
   PropagateEpoch(timeDelta);
   VPropagate(timeDelta, propagationType);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::PropagateTo(const Epoch& epoch, const PropagationType& propagationType)
{
   PropagateEpochTo(epoch);
   VPropagate(epoch - m_epoch, propagationType);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::PropagateOrbitalElements(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   VPropagateOrbitalElements(timeDelta);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::PropagateOrbitalElementsTo(const Epoch& epoch)
{
   PropagateEpochTo(epoch);
   VPropagateOrbitalElements(epoch - m_epoch);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::PropagateStateVector(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   VPropagateStateVector(timeDelta);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::PropagateStateVectorTo(const Epoch& epoch)
{
   PropagateEpochTo(epoch);
   VPropagateStateVector(epoch - m_epoch);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::QueryEphemeris(const Epoch& epoch, const EphemerisQueryType& queryType)
{
   if (!m_ephemerisInitialized)
      VInitializeEphemeris();
   VQueryEphemeris(epoch, queryType);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::QueryOrbitalElements(const Epoch& epoch)
{
   if (!m_ephemerisInitialized)
      VInitializeEphemeris();
   VQueryOrbitalElements(epoch);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::QueryStateVector(const Epoch& epoch)
{
   if (!m_ephemerisInitialized)
      VInitializeEphemeris();
   VQueryStateVector(epoch);
}

////////////////////////////////////////////////////////////
inline void OrbitalBody::QueryPhysicalProperties()
{
   if (!m_ephemerisInitialized)
      VInitializeEphemeris();
   VQueryPhysicalProperties();
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