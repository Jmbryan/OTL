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
#include <OTL/Core/Conversion.hpp>
#include <OTL/Core/Base.hpp>

namespace otl
{

namespace keplerian
{

// Forward declarations
class IPropagateAlgorithm;

class Orbit
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
	Orbit(double mu);
   
   ////////////////////////////////////////////////////////////
   /// \brief Create the fully defined orbit
   ///
   /// The orbital elements are automatically computed from
   /// the state vector.
   ///
   /// \param stateVector State vector of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   Orbit(const StateVector& stateVector, double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Create the fully defined orbit
   ///
   /// The state vector is automatically computed from
   /// the orbital elements.
   ///
   /// \param orbitalElements Orbital elements of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   Orbit(const OrbitalElements& orbitalElements, double mu);
   
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
   /// \brief Set the state vector of the orbit at this time
   ///
   /// The orbital elements are automatically updated.
   /// 
   /// \param stateVector State vector of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   void SetStateVector(const StateVector& stateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Set the orbital elements of the orbit at this time
   ///
   /// The state vector is automatically updated.
   /// 
   /// \param orbitalElements Orbital elements of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   void SetOrbitalElements(const OrbitalElements& orbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Set the true anomaly orbital element
   ///
   /// The true anomaly is the only orbital element that varies with
   /// time (assuming a non-perturbed orbit).
   /// The state vector is automatically updated.
   /// 
   /// \param orbitalElements Orbital elements of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   void SetTrueAnomaly(double trueAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the central body
   ///
   /// \return Gravitational parameter of the central body of the orbit
   ///
   ////////////////////////////////////////////////////////////
   double GetMu() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the state vector of the orbit at this time
   ///
   /// \return State vector of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& GetStateVector() const;
   
   ////////////////////////////////////////////////////////////
   /// \brief Get the orbital elements of the orbit at this time
   ///
   /// \return Orbital elements of the orbit at this time
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;
   
   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the orbit
   ///
   /// \return Radius of the orbit
   ///
   ////////////////////////////////////////////////////////////
   double GetRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the type of the orbit
   ///
   /// \return Type of the orbit
   ///
   ////////////////////////////////////////////////////////////
   Type GetType() const;

   ////////////////////////////////////////////////////////////
   /// \brief Is the orbit of this type
   ///
   /// \return True if the orbit is this type
   ///
   ////////////////////////////////////////////////////////////
   bool IsType(Type orbitType) const;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit in time
   ///
   /// The time can be positive or negative for
   /// forewards and backwards propagation respectively.
   ///
   /// \param timeDelta Time object for specifying the propagation duration
   ///
   ////////////////////////////////////////////////////////////
   void Propagate(const Time& timeDelta);

private:
   ////////////////////////////////////////////////////////////
   /// \brief Update the state vector of the orbit
   ///
   /// This function re-computes the state vector of the orbit
   /// based on the orbital elements.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateStateVector();
   
   ////////////////////////////////////////////////////////////
   /// \brief Update the orbital elements of the orbit
   ///
   /// This function re-computes the orbital elements of the orbit
   /// based on the state vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitalElements();
   
   ////////////////////////////////////////////////////////////
   /// \brief Update the radius of the orbit
   ///
   /// This function re-computes the radius of the orbit based on
   /// the position vector.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateRadius();

   ////////////////////////////////////////////////////////////
   /// \brief Update the type of the orbit
   ///
   /// This function re-computes the type of the orbit based on
   /// the eccentricity.
   ///
   ////////////////////////////////////////////////////////////
   void UpdateOrbitType();

private:
   StateVector m_stateVector;          ///< State Vector representing the orbit
   OrbitalElements m_orbitalElements;  ///< Orbital Elements representing the orbit

   double m_mu;                        ///< Gravitational parameter of the central body (kg^2/m^3)
   double m_radius;                    ///< Radius of the orbit (m)
   Type m_orbitType;                   ///< Type of orbit (prograde or retrograde)

   std::unique_ptr<IPropagateAlgorithm> m_propagator; ///< Pointer to the propagate algorithm
};

#include <OTL/Core/Orbit.inl>

} // namespace keplerian

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::keplerian::Orbit
/// \ingroup keplerian
///
/// Simple class that defines a 2D or 3D Keplerian orbit.
/// A orbit can be represented by either a state vector
/// or by a set of orbital elements. 
///
/// Usage example:
/// \code
/// otl::keplerian::Orbit orbit(ASTRO_MU_EARTH);
/// orbit.SetStateVector(myStateVector);
///
/// myOrbitalElements = orbit.GetOrbitalElements();
/// myRadius = orbit.GetRadius();
///
/// orbit.Propagate(600.0f); // propagate forward 10 minutes
/// myNewStateVector = orbit.GetStateVector();
/// myNewOrbitalElements = orbit.GetOrbitalElements();
///
/// \endcode
///
////////////////////////////////////////////////////////////