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
#include <OTL/Core/Base.h>
#include <OTL/Core/Epoch.h>
#include <OTL/Core/KeplerianPropagator.h>

namespace otl
{

// Forward declarations
//class IPropagator;
//typedef std::shared_ptr<IPropagator> PropagatorPointer;

namespace keplerian
{

class OTL_CORE_API Orbit
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Orbit types
   ////////////////////////////////////////////////////////////
   enum class Type
   {
      Invalid = -1,  ///< Invalid orbit type
      Circular,      ///< Circular orbit (eccentricity equal to zero)
      Elliptical,    ///< Elliptical orbit (eccentricity between zero and one)
      Parabolic,     ///< Parabolic orbit (eccentricity equal to one)
      Hyperbolic,    ///< Hyperbolic orbit (eccentricity greater than one)
      Count          ///< Number of orbit types
   };

   ////////////////////////////////////////////////////////////
   /// \brief Orbit directions
   ////////////////////////////////////////////////////////////
   enum class Direction
   {
      Invalid = -1,  ///< Invalid orbit direction
      Prograde,      ///< Prograde (counterclockwise as viewed from above the orbit)
      Retrograde,    ///< Retrograde (clockwise as viewed from above the orbit)
      Count          ///< Number of orbit directions
   };

   ////////////////////////////////////////////////////////////
   /// \brief Orbit properties
   ////////////////////////////////////////////////////////////
   struct OrbitProperties
   {
      Type type;                       ///< Type of orbit (circular, elliptical, hyperbolic, etc.)
      double radius;                   ///< Radius of the orbit (km)
      double anomaly;                  ///< Eccentric, hyperbolic, or parabolic anomaly (radians)
      double trueAnomaly;              ///< True anomaly (radians)
      double meanMotion;               ///< Mean motion (radians / sec)
      double period;                   ///< Orbit period (seconds)
      double timeSincePerapsis;        ///< Time since periapsis (seconds)
      double specificAngularMomentum;  ///< Specific angular momentum ()
      double semiparameter;            ///< Semiparameter (km)
   };

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Orbit();

   ////////////////////////////////////////////////////////////
   /// \brief Create the orbit from gravitational parameter
   ///
   /// \param mu Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
	//explicit Orbit(double mu);
   
   ////////////////////////////////////////////////////////////
   /// \brief Create the orbit from gravitational parameter and state vector
   ///
   /// \param mu Gravitational parameter of the central body of the orbit
   /// \param stateVector StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   //Orbit(double mu, const StateVector& stateVector);
   Orbit(double mu, const OrbitalElements& orbitalElements, Direction orbitDirection = Direction::Prograde);
   Orbit(double mu, const StateVector& StateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   ~Orbit();

   ////////////////////////////////////////////////////////////
   /// \brief Set the gravitational parameter of the central body
   ///
   /// \param mu Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void SetGravitationalParameterCentralBody(double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Set the state vector of the orbit
   ///
   /// \param stateVector StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void SetOrbitalElements(const OrbitalElements& orbitalElements, Direction orbitDirection = Direction::Prograde);
   void SetStateVector(const StateVector& stateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Set the propagation algorithm
   ///
   /// The propagator algorithm is by the Propagate() function
   /// to propagate the orbit forwards or backwards in time.
   ///
   /// \param propagator Smart pointer to the propagation algorithm
   ///
   ////////////////////////////////////////////////////////////
   //void SetPropagator(const PropagatorPointer& propagator);

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the central body
   ///
   /// \return Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
   double GetGravitationalParameterCentralBody() const;
 
   ////////////////////////////////////////////////////////////
   /// \brief Get the current cartesian state vector of the orbit
   ///
   /// If the internal state vector is stored in StateVectorType::Orbital
   /// format, then this function will convert from OrbitalElements
   /// to StateVector by calling the
   /// ConvertOrbitalElements2StateVector() function.
   /// However, the result is cached so subsequent calls will
   /// not suffer this overhead until the state vector is modified
   /// by either calling the SetStateVector(), Propagate(), or
   /// PropagateTrueAnomaly() methods.
   ///
   /// If the state vector has not been specified through the
   /// constructor or the SetStateVector() method, then this
   /// function will return a StateVector of all zeros.
   ///
   /// \return Current StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbital elements of the orbit
   ///
   /// If the internal state vector is stored in StateVectorType::Cartesian
   /// format, then this function will convert from
   /// StateVector to OrbitalElements by calling the
   /// ConvertStateVector2OrbitalElements() function.
   /// However, the result is cached so subsequent calls will
   /// not suffer this overhead until the state vector is modified
   /// by either calling the SetStateVector(), Propagate(), or
   /// PropagateTrueAnomaly() methods.
   ///
   /// If the state vector has not been specified through the
   /// constructor or the SetStateVector() method, then this
   /// function will return an OrbitalElements of all zeros.
   ///
   /// \return Current OrbitalElements of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current state vector of the orbit
   ///
   /// Returns a generic StateVector object whose type may be
   /// either StateVectorType::Orbital or StateVectorType::Cartesian.
   ///
   /// If the state vector has not been specified through the
   /// constructor or the SetStateVector() method, then this
   /// function will return a StateVector of all zeros.
   ///
   /// \return Current StateVector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   //const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the orbit
   ///
   /// \return Radius of the orbit
   ///
   ////////////////////////////////////////////////////////////
   //double GetOrbitRadius() const;

   Direction GetOrbitDirection() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the type of the orbit
   ///
   /// Returns an Orbit::Type enumerator which indicates the type
   /// of the orbit (circular, elliptical, hyperbolic, etc.)
   ///
   /// \return OrbitType enumerator
   ///
   ////////////////////////////////////////////////////////////
   Type GetOrbitType() const;

   const OrbitProperties& GetOrbitProperties() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the elapsed propagation time of the orbit
   ///
   /// The elapsed propagation time is the amount of time since
   /// the reference state vector. The elapsed propagation time
   /// is modified whenver the Propagate() method is called, and
   /// is reset whenever the SetStateVector() method is called.
   ///
   /// \return Elapsed propagation Time
   ///
   ////////////////////////////////////////////////////////////
   //const Time& GetElapsedPropagationTime() const;

   bool IsDirection(Direction orbitDirection) const;

   ////////////////////////////////////////////////////////////
   /// \brief Is the orbit of this type
   ///
   /// \return True if the orbit matches the orbit type
   ///
   ////////////////////////////////////////////////////////////
   bool IsType(Type orbitType) const;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit in time
   ///
   /// This function propagates the state vector of the orbit
   /// in time using the internal propagation algorithm.
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
   /// \brief Propagate the orbit to the true anomaly
   ///
   /// The true anomaly is the only orbital element that varies with
   /// time (assuming a non-perturbed orbit). This function
   /// propagates the orbit to the desired true anomaly.
   ///
   /// \note The propagation algorithm is not used for this function
   /// 
   /// \param trueAnomaly Desired true anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void PropagateToMeanAnomaly(double meanAnomaly);
   void PropagateToTrueAnomaly(double trueAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbit to a single-line formatted string
   ///
   /// The orbit is converted to a single-line string
   /// with the following format:
   ///
   /// "t=[orbit type] r=[orbit radius] mu=[grav. param] [StateVector]"
   ///
   /// e.g.
   ///
   /// "t=elliptical r=7414.32 mu=398600 [StateVector]"
   ///
   /// where [StateVector] is the result from calling the
   /// StateVector::ToString() method.
   ///
   /// \see StateVector::ToString()
   ///
   /// \returns std::string Stringified orbit
   ///
   ////////////////////////////////////////////////////////////
   //std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbit to a detailed multi-line formatted string
   ///
   /// The orbit is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Orbit Type:                           [orbit type]
   ///  Orbit Radius:                         [orbit radius]
   ///  Central Body Gravitational Parameter: [grav. param.]
   ///  StateVector:
   ///     [StateVector]
   /// "
   ///
   /// e.g.
   ///
   /// "Orbit Type:                           Elliptical
   ///  Orbit Radius:                         7414.318917
   ///  Central Body Gravitational Parameter: 398600.441800
   ///  StateVector:
   ///     [StateVector]
   /// "
   ///
   /// where [StateVector] is the result from calling the
   /// StateVector::ToDetailedString() method.
   ///
   /// \note Units are not shown because that information is not stored in the orbit
   ///
   /// \see StateVector::ToDetailedString()
   ///
   /// \returns std::string Stringified orbit
   ///
   ////////////////////////////////////////////////////////////
   //std::string ToDetailedString(std::string prefix = "") const;
   std::string ToString(std::string prefix = "") const;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Update the orbit properties
   ///
   /// This function computes the orbit type and orbit radius
   /// from the state vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitProperties() const;
   void UpdateOrbitalElements() const;
   void UpdateStateVector() const;

private:
   double m_gravitationalParameterCentralBody;           ///< Gravitational parameter of the central body
   mutable OrbitProperties m_properties;                 ///< Additional properties of the orbit
   mutable OrbitalElements m_orbitalElements;            ///< Orbital elements
   mutable StateVector m_StateVector;  ///< Cartesian state vector
   mutable Direction m_direction;                        ///< Orbit direction (e.g. Prograde or Retrograde)
   mutable bool m_propertiesDirty;                       ///< True if the orbit properties are not up-to-date
   mutable bool m_orbitalElementsDirty;                  ///< True if the orbital elements are not up-to-date
   mutable bool m_StateVectorDirty;             ///< True if the cartesian state vector is not up-to-date
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates Orbit
///
/// The orbit is converted to a string by calling the
/// Orbit::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const Orbit& orbit)
{
   stream << orbit.ToString();
   return stream;
}

Orbit::OrbitProperties ComputeOrbitProperties(double mu, const OrbitalElements& orbitalElements);
Orbit::OrbitProperties ComputeOrbitProperties(double mu, const StateVector& StateVector);

inline double ComputeOrbitRadius(const OrbitalElements& orbitalElements, double trueAnomaly)
{
   // r = a * (1.0 - ecosE) from Lambert Revisited
   return orbitalElements.semiMajorAxis * (1.0 - SQR(orbitalElements.eccentricity)) / (1.0 + orbitalElements.eccentricity * cos(trueAnomaly));
}

inline double ComputeOrbitRadius(const StateVector& StateVector)
{
   return StateVector.position.norm();
}

Orbit::Type ComputeOrbitType(double eccentricity);

inline Orbit::Type ComputeOrbitType(double mu, const OrbitalElements& orbitalElements)
{
   return ComputeOrbitType(orbitalElements.eccentricity);
}

inline Orbit::Type ComputeOrbitType(double mu, const StateVector& StateVector)
{
   const auto& R = StateVector.position;
   const auto& V = StateVector.velocity;
   return ComputeOrbitType(((SQR(V.norm()) / mu - 1.0 / R.norm()) * R - (R.dot(V) / mu) * V).norm());
}

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::Orbit
/// \ingroup keplerian
///
/// Defines a 2D or 3D Keplerian orbit which is defined by
/// a state vector and the gravitational parameter of the
/// central body being orbited.
///
/// Usage example:
/// \code
/// using otl::keplerian
///
/// // Create the orbit and define the current state vector
/// Orbit orbit(ASTRO_MU_EARTH);
/// orbit.SetStateVector(myStateVector);
///
/// // Orbital elements and orbital radius are computed from state vector
/// myOrbitalElements = orbit.GetOrbitalElements();
/// myRadius = orbit.GetRadius();
///
/// // Propagate forward 10 minutes
/// orbit.Propagate(Time::Minutes(10.0));
///
/// // Propagate to true anomaly of 60 degrees
/// orbit.PropagateToTrueAnomaly(60.0 * MATH_DEG_TO_RAD);
///
/// // Get updated cartesian state vector and orbital elements
/// auto StateVector2 = orbit.GetStateVector();
/// auto orbitalElements2 = orbit.GetOrbitalElements();
///
/// \endcode
///
////////////////////////////////////////////////////////////