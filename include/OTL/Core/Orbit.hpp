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

class Orbit
{
public:

   enum OrbitDirection
   {
      InvalidDirection = -1,
      Propgrade,
      Retrograde,
      NumDirections
   };

   enum OrbitType
   {
      InvalidType = -1,
      Circular,
      Elliptical,
      Parabolic,
      Hyperbolic,
      NumTypes
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
   OrbitType GetType() const;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbit in time
   ///
   /// The number of seconds can be positive or negative for
   /// forewards and backwards propagation respectively.
   ///
   /// \param seconds Number of seconds to propagate the orbit for
   ///
   ////////////////////////////////////////////////////////////
   void Propagate(double seconds);

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
   OrbitType m_orbitType;              ///< Type of orbit (prograde or retrograde)
};

#include <OTL/Core/Orbit.inl>

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::Orbit
/// \ingroup core
///
/// otl::Orbit is a simple class that defines a Keplerian
/// orbit. A orbit can be represented by either a state vector
/// or by a set of orbital elements. 
///
/// Usage example:
/// \code
/// otl::Orbit orbit(ASTRO_MU_EARTH);
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