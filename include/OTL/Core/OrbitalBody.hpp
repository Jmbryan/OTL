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
#include <OTL/Core/Base.hpp>

// Forward declarations
class Epoch;

namespace otl
{

class OrbitalBody
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
   /// \param radius Radius of the orbital body
   /// \param mu Gravitational parameter of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   OrbitalBody(const std::string& name, double radius, double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Set the name of the orbital body
   ///
   /// \param name Name of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   void SetName(const std::string& name);
   
   ////////////////////////////////////////////////////////////
   /// \brief Set the radius of the orbital body
   ///
   /// \param radius Radius of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   void SetRadius(double radius);

   ////////////////////////////////////////////////////////////
   /// \brief Set the gravitational parameter of the orbital body
   ///
   /// \param mu Gravitational parameter of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetMu(double mu);

   ////////////////////////////////////////////////////////////
   /// \brief Get the name of the orbital body
   ///
   /// \return Name of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   std::string GetName() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the orbital body
   ///
   /// \return Radius of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   double SetRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the gravitational parameter of the orbital body
   ///
   /// \return Gravitational parameter of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   double GetMu() const;

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
   virtual void GetStateVectorsAtEpoch(const Epoch& epoch, StateVector& stateVector) const = 0;
   
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
   virtual void GetOrbitalElementsAtEpoch(const Epoch& epoch, OrbitalElements& orbitalElements) const = 0;

protected:
   std::string m_name;  ///< Name of the orbital body
   double m_radius;     ///< Radius of the orbital body
   double m_mu;         ///< Gravitational parameter of the orbital body
};

#include <OTL/Core/OrbitalBody.inl>

} // namespace otl