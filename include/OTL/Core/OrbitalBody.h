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

namespace otl
{

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
   /// If a mass is not specified it will be
   /// initialized to 1.0.
   ///
   /// \param name Name of the orbital body
   /// \param mass Mass of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   OrbitalBody(const std::string& name, double mass);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~OrbitalBody();
 
   ////////////////////////////////////////////////////////////
   /// \brief Set the name of the orbital body
   ///
   /// \param name Name of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetName(const std::string& name);

   ////////////////////////////////////////////////////////////
   /// \brief Set the mass of the orbital body
   ///
   /// \param mass Mass of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetMass(double mass);

   ////////////////////////////////////////////////////////////
   /// \brief Set the gravitational parameter of the orbital body
   ///
   /// \param mu Gravitational parameter of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetMu(double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Set the new position vector of the orbital body
   ///
   /// \param position New position vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetPosition(const Vector3d& position);

   ////////////////////////////////////////////////////////////
   /// \brief Set the new velocity vector of the orbital body
   ///
   /// \param position New velocity vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetVelocity(const Vector3d& velocity);

   ////////////////////////////////////////////////////////////
   /// \brief Set the new state vector of the orbital body
   ///
   /// \param position New state vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetStateVector(const StateVector& stateVector);

   ////////////////////////////////////////////////////////////
   /// \brief Set the new orbital elements of the orbital body
   ///
   /// \param position New orbital elements of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetOrbitalElements(const OrbitalElements& orbitalElements);

   ////////////////////////////////////////////////////////////
   /// \brief Get the name of the orbital body
   ///
   /// \return Name of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   const std::string& GetName() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the mass of the orbital body
   ///
   /// \return mass of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   double GetMass() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the orbital body
   ///
   /// \return Gravitational parameter of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   double GetMu() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbit radius of the orbital body
   ///
   /// \return Orbit radius of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   double GetOrbitRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current position vector of the orbital body
   ///
   /// If SetEpoch has not been called, then this function will
   /// return a position vector of all zeros.
   ///
   /// \return Current position vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetPosition() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current velocity vector of the orbital body
   ///
   /// If SetEpoch has not been called, then this function will
   /// return a velocity vector of all zeros.
   ///
   /// \return Current velocity vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Vector3d& GetVelocity() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current state vector of the orbital body
   ///
   /// If SetEpoch() has not been called, then this function will
   /// return a StateVector of all zeros.
   ///
   /// \return Current state vector of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const StateVector& GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the orbital elements of the orbital body
   ///
   /// If SetEpoch() has not been called, then this function will
   /// return a OrbitalElements of all zeros. 
   ///
   /// \return Current orbital elements of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const OrbitalElements& GetOrbitalElements() const;

   ////////////////////////////////////////////////////////////
   /// \brief Propagate the orbital body in time
   ///
   /// The time can be positive or negative for
   /// forewards and backwards propagation respectively.
   ///
   /// This function is virtual and may be re-implemented
   /// by subclasses in order to utilize different propagation
   /// techniques such as querying an ephemeris database.
   ///
   /// \param timeDelta Time object for specifying the propagation duration
   ///
   ////////////////////////////////////////////////////////////
   virtual void Propagate(const Time& timeDelta);

   ////////////////////////////////////////////////////////////
   /// \brief Get the state vector of the orbital body at an Epoch
   ///
   /// Pure virtual function for calculating the state vector
   /// of the orbital body at a specified Epoch. Derived classes
   /// must re-implement this function with their respective
   /// ephemeris algorithm.
   ///
   /// \param epoch Epoch to obtain state vector at
   /// \param stateVector Resulting state vector computed at the specified Epoch
   ///
   ////////////////////////////////////////////////////////////
   //virtual void GetStateVectorAtEpoch(const Epoch& epoch, StateVector& stateVector) = 0;
   
   ////////////////////////////////////////////////////////////
   /// \brief Get the orbital elements of the orbital body at an Epoch
   ///
   /// Pure virtual function for calculating the orbital elements
   /// of the orbital body at a specified Epoch. Derived classes
   /// must re-implement this function with their respective
   /// ephemeris algorithm.
   ///
   /// \param epoch Epoch to obtain state vector at
   /// \param orbitalElements Resulting orbital elements computed at the specified Epoch
   ///
   ////////////////////////////////////////////////////////////
   //virtual void GetOrbitalElementsAtEpoch(const Epoch& epoch, OrbitalElements& orbitalElements) = 0;

   std::string ToString() const;
   std::string ToDetailedString(std::string prefix = "") const;

private:
    std::string m_name;        ///< Name of the orbital body
    keplerian::Orbit m_orbit;  ///< Keplerian orbit of the orbital body 
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

typedef std::shared_ptr<OrbitalBody> OrbitalBodyPointer;

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::OrbitalBody
/// \ingroup otl
///
/// Interface class for all orbital bodies (planets,
/// asteroids, comets, spacecraft, etc..).  
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