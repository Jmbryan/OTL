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

class IPropagator;
typedef std::shared_ptr<IPropagator> PropagatorPointer;

class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

struct PhysicalProperties
{
   double mass;      ///< Mass
   double mu;        ///< Gravitational parameter
   double radius;    ///< Equatorial radius

   PhysicalProperties();
   PhysicalProperties(double _mass, double _mu, double _radius);

   //std::string ToString() const;
   //std::string ToDetailedString(std::string prefix = "") const;
};

class OrbitalBody2;
//class MpcorbPlanet : public OrbitalBody2
//{
//public:
//   explicit MpcorbPlanet(const std::string& name,
//                         const Epoch& epoch = Epoch::MJD2000(0.0));
//
//};

//class SpiceBody : public OrbitalBody2
//{
//public:
//   SpiceBody(const std::string& name);
//};

class OrbitalBody2
{
public:
   OrbitalBody2();

   OrbitalBody2(const std::string& name,
                const PhysicalProperties& physicalProperties,
                double gravitationalParameterCentralBody,
                const StateVector& stateVector,
                const Epoch& epoch = Epoch::MJD2000(0.0));

   OrbitalBody2(const std::string& name,
                const PhysicalProperties& physicalProperties,
                double gravitationalParameterCentralBody,
                const OrbitalElements& orbitalElements,
                const Epoch& epoch = Epoch::MJD2000(0.0));

   virtual ~OrbitalBody2();

   void SetEphemeris(const EphemerisPointer& ephemeris);
   void SetPropagator(const PropagatorPointer& propagator);
   void SetMaxPropagationTime(const Time& threshold);

   const std::string& GetName() const;
   const PhysicalProperties& GetPhysicalProperties() const;
   const Epoch& GetEpoch() const;
   double GetGravitationalParameterCentralBody() const;
   const Vector3d& GetPosition() const;
   const Vector3d& GetVelocity() const;
   const StateVector& GetStateVector() const;
   const OrbitalElements& GetOrbitalElements() const;
   const keplerian::Orbit& GetOrbit() const;
   double GetOrbitRadius() const;
   keplerian::Orbit::Type GetOrbitType() const;

   bool IsOrbitType(keplerian::Orbit::Type orbitType) const;

   void Propagate(const Time& timeDelta, const PropagationType& propagationType = PropagationType::OrbitalElements);
   void PropagateTo(const Epoch& epoch, const PropagationType& propagationType = PropagationType::OrbitalElements);

   void PropagateOrbitalElements(const Time& timeDelta);
   void PropagateOrbitalElementsTo(const Epoch& epoch);

   void PropagateStateVector(const Time& timeDelta);
   void PropagateStateVectorTo(const Epoch& epoch);

   void QueryEphemeris(const Epoch& epoch);
   void QueryOrbitalElements(const Epoch& epoch);
   void QueryStateVector(const Epoch& epoch);
   void QueryPhysicalProperties();

   std::string ToString() const;
   std::string ToDetailedString(std::string prefix = "") const;

private:
   virtual void VPropagate(const Time& timeDelta, const PropagationType& propagationType);
   virtual void VPropagateOrbitalElements(const Time& timeDelta);
   virtual void VPropagateStateVector(const Time& timeDelta);
   virtual void VQueryEphemeris(const Epoch& epoch);

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
};

inline void OrbitalBody2::Propagate(const Time& timeDelta, const PropagationType& propagationType)
{ 
   PropagateEpoch(timeDelta);
   VPropagate(timeDelta, propagationType);   
}
inline void OrbitalBody2::PropagateOrbitalElements(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   VPropagateOrbitalElements(timeDelta);
}
inline void OrbitalBody2::PropagateStateVector(const Time& timeDelta)
{
   PropagateEpoch(timeDelta);
   VPropagateStateVector(timeDelta);
}
inline void OrbitalBody2::PropagateTo(const Epoch& epoch, const PropagationType& propagationType)
{
   PropagateEpochTo(epoch);
   VPropagate(epoch - m_epoch, propagationType);
}
inline void OrbitalBody2::PropagateOrbitalElementsTo(const Epoch& epoch)
{
   PropagateEpochTo(epoch);
   VPropagateOrbitalElements(epoch - m_epoch);
}
inline void OrbitalBody2::PropagateStateVectorTo(const Epoch& epoch)
{
   PropagateEpochTo(epoch);
   VPropagateStateVector(epoch - m_epoch);
}
inline void OrbitalBody2::QueryEphemeris(const Epoch& epoch)
{
   VQueryEphemeris(epoch);
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

   const keplerian::Orbit& GetOrbit() const;

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

   void SetPropagator(const PropagatorPointer& propagator);

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