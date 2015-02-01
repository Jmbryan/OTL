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

namespace otl
{

// Forward declarations
class IPropagator;
typedef std::shared_ptr<IPropagator> PropagatorPointer;

namespace keplerian
{

class OTL_CORE_API Orbit
{
public:

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
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   Orbit();

   ////////////////////////////////////////////////////////////
   /// \brief Create the orbit with a gravitational parameter
   ///
   /// \param mu Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
	explicit Orbit(double mu);
   
   ////////////////////////////////////////////////////////////
   /// \brief Create the fully defined orbit
   ///
   /// The orbital elements are automatically computed from
   /// the state vector.
   ///
   /// \param stateVector State vector of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   Orbit(double mu, const StateVector& stateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Create the fully defined orbit
   ///
   /// The state vector is automatically computed from
   /// the orbital elements.
   ///
   /// \param orbitalElements Orbital elements of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   Orbit(double mu, const OrbitalElements& orbitalElements);
   
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
   void SetMu(double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Set thea new state vector of the orbit
   ///
   /// The orbit radius is automatically updated.
   ///
   /// The orbital elements and orbit type are not updated
   /// until the GetOrbitalElements(), GetOrbitType(),
   /// IsType(), or PropagateToTrueAnomaly() functions are called.
   ///
   /// \param stateVector New state vector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void SetStateVector(const StateVector& stateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Set the new orbital elements of the orbit
   ///
   /// The orbit type is automatically updated.
   ///
   /// The state vector and orbit radius are not updated
   /// until the GetStateVector(), GetOrbitRadius(), or
   /// Propagate() functions are called.
   ///
   /// \param orbitalElements New orbital elements of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void SetOrbitalElements(const OrbitalElements& orbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Set the algorithm used for orbit propagation
   ///
   /// \param propagator Smart pointer to the propagation algorithm
   ///
   ////////////////////////////////////////////////////////////
   void SetPropagator(const PropagatorPointer& propagator);

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the central body
   ///
   /// \return Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
   double GetMu() const;
 
   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the orbit
   ///
   /// \return Radius of the orbit
   ///
   ////////////////////////////////////////////////////////////
   double GetOrbitRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the type of the orbit
   ///
   /// Returns an OrbitType enumerator which indicates the type
   /// of the orbit (circular, elliptical, hyperbolic, etc.)
   ///
   /// \return OrbitType enumerator
   ///
   ////////////////////////////////////////////////////////////
   Type GetOrbitType() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current position vector of the orbit
   ///
   /// If the StateVector or OrbitalElements have not been specified
   /// through the constructor or the Set methods, then this
   /// function will return a Vector3d of all zeros.
   ///
   /// \return Current position vector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetPosition() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current velocity vector of the orbit
   ///
   /// If the StateVector or OrbitalElements have not been specified
   /// through the constructor or the Set methods, then this
   /// function will return a Vector3d of all zeros.
   ///
   /// \return Current velocity vector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetVelocity() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current state vector of the orbit
   ///
   /// If the StateVector or OrbitalElements have not been specified
   /// through the constructor or the Set methods, then this
   /// function will return StateVector of all zeros.
   ///
   /// \return Current state vector of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current orbital elements of the orbit
   ///
   /// If the StateVector or OrbitalElements have not been specified
   /// through the constructor or the Set methods, then this
   /// function will return an OrbitalElements of all zeros.
   ///
   /// \return Current orbital elements of the orbit
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;

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
   /// This function dispatches to the following sub-functions
   /// depending on the value of PropagationType:
   /// \li PropagateOrbitalElements()
   /// \li PropagateStateVector()
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   /// \param propagationType PropagationType which determines whether the orbital elements or state vector is propagated
   ///
   ////////////////////////////////////////////////////////////
   void Propagate(const Time& timeDelta, const PropagationType& propagationType = PropagationType::OrbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit in time using the state vector
   ///
   /// This function propagates the orbital elements using the
   /// internal propagation algorithm. The propagation algorithm
   /// defaults to KeplerianPropagator but can be specified by
   /// calling the SetPropagator() method.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   ///
   ////////////////////////////////////////////////////////////
   void PropagateOrbitalElements(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit in time using the state vector
   ///
   /// This function propagates the state vector using the
   /// internal propagation algorithm. The propagation algorithm
   /// defaults to KeplerianPropagator but can be specified by
   /// calling the SetPropagator() method.
   ///
   /// \note The time can be positive or negative for forewards and backwards propagation respectively
   ///
   /// \param timeDelta Time object which specifies the propagation duration
   ///
   ////////////////////////////////////////////////////////////  
   void PropagateStateVector(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit to the true anomaly
   ///
   /// The true anomaly is the only orbital element that varies with
   /// time (assuming a non-perturbed orbit). This function
   /// propagates the orbit to the desired true anomaly.
   /// 
   /// \param trueAnomaly Desired true anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   void PropagateToTrueAnomaly(double trueAnomaly);

   void UseStateVectorForStringOutput(bool useStateVectorForStringOutput);

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbit to a single-line formatted string
   ///
   /// The orbit is converted to a single-line string
   /// with the following format:
   ///
   /// "mu=[grav. param] r=[orbit radius] [OrbitalElements]"
   ///
   /// e.g.
   ///
   /// "mu=398600 r=7414.32 [OrbitalElements]"
   ///
   /// where [OrbitalElements] is the result of calling the
   /// OrbitalElements::ToString() method. The state vector
   /// can be displayed instead, by calling the function
   /// UseStateVectorForStringOutput(true).
   ///
   /// \see OrbitalElements::ToString(), StateVector::ToString()
   ///
   /// \returns std::string Stringified orbit
   ///
   ////////////////////////////////////////////////////////////
   std::string ToString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Converts the orbit to a detailed multi-line formatted string
   ///
   /// The orbit is converted to a detailed multi-line string
   /// with the following format:
   ///
   /// "Orbit:
   ///     Gravitational Parameter: [grav. param.]
   ///     Radius:                  [orbit radius]
   ///     [OrbitalElements]
   ///     [StateVector]
   /// "
   ///
   /// e.g.
   ///
   /// "Orbit:
   ///     Gravitational Parameter: 398600.441800
   ///     Radius:                  7414.318917
   ///     [OrbitalElements]
   ///     [StateVector]
   /// "
   ///
   /// where [OrbitalElements] and [StateVector] are the results
   /// from calling the respective ToDetailedString() methods.
   ///
   /// \note Some units are not shown because that information is not stored in the orbit
   ///
   /// \returns std::string Stringified orbit
   ///
   ////////////////////////////////////////////////////////////
   std::string ToDetailedString(std::string prefix = "") const;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Update the state vector of the orbit
   ///
   /// This function re-computes the state vector of the orbit
   /// based on the orbital elements.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateStateVector() const;
   
   ////////////////////////////////////////////////////////////
   /// \brief Update the reference state vector of the orbit
   ///
   /// This function re-computes the reference state vector of
   /// the orbit based on the reference orbital elements.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateReferenceStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Update the orbital elements of the orbit
   ///
   /// This function re-computes the orbital elements of the orbit
   /// based on the state vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Update the reference orbital elements of the orbit
   ///
   /// This function re-computes the reference orbital elements of
   /// the orbit based on the reference state vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateReferenceOrbitalElements() const;
   
   ////////////////////////////////////////////////////////////
   /// \brief Update the radius of the orbit
   ///
   /// This function re-computes the radius of the orbit based on
   /// the position vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Update the type of the orbit
   ///
   /// This function re-computes the type of the orbit based on
   /// the eccentricity.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitType() const;

private:
   bool m_useStateVectorForStringOutput;                 ///< If TRUE, the ToString() method will print the state vector. By default, orbital elements are printed.
   double m_mu;                                          ///< Gravitational parameter of the central body (kg^2/m^3)
   Time m_elapsedPropagationTime;                        ///< Total elapsed propagation time since the reference orbital elements or state vector
   PropagatorPointer m_propagator;                       ///< Pointer to the propagation algorithm
   mutable double m_orbitRadius;                         ///< Radius of the orbit (m)
   mutable Type m_orbitType;                             ///< Type of orbit (circular, elliptical, hyperbolic, etc.)
   mutable OrbitalElements m_orbitalElements;            ///< Orbital Elements representing the orbit
   mutable OrbitalElements m_referenceOrbitalElements;   ///< Reference orbital elements
   mutable StateVector m_stateVector;                    ///< State Vector representing the orbit
   mutable StateVector m_referenceStateVector;           ///< Reference state vector
   mutable bool m_orbitalElementsDirty;                  ///< Flag which when TRUE indicates the orbital elements need to be updated
   mutable bool m_referenceOrbitalElementsDirty;         ///< Flag which when TRUE indicates the reference orbital elements need to be updated
   mutable bool m_stateVectorDirty;                      ///< Flag which when TRUE indicates the state vector needs to be updated
   mutable bool m_referenceStateVectorDirty;             ///< Flag which when TRUE indicates the reference state vector needs to be updated
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

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::Orbit
/// \ingroup keplerian
///
/// Defines a 2D or 3D Keplerian orbit. A orbit can be
/// represented by either a state vector or by a set
/// of orbital elements. 
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
/// // Propagate in retrograde direction to true anomaly of 60 degrees
/// orbit.PropagateToTrueAnomaly(60.0 * MATH_DEG_TO_RAD, Orbit::Retrograde);
///
/// // Get updated state vector and orbital elements
/// myNewStateVector = orbit.GetStateVector();
/// myNewOrbitalElements = orbit.GetOrbitalElements();
///
/// \endcode
///
////////////////////////////////////////////////////////////