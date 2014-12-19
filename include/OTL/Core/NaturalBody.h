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
#include <OTL/Core/OrbitalBody.hpp>
#include <OTL/Core/Epoch.hpp>

namespace otl
{

class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

class NaturalBody : public OrbitalBody
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   NaturalBody();

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the natural body
   ///
   /// \param name Name of the natural body 
   /// \param mass Mass of the natural body
   /// \param radius Radius of the natural body
   /// \param 
   ///
   ////////////////////////////////////////////////////////////
   NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Set the radius of the orbital body
   ///
   /// \param radius Radius of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   void SetRadius(double radius);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch of the natural body
   ///
   /// \param epoch epoch of the natural body
   ///
   ////////////////////////////////////////////////////////////
   void SetEpoch(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Get the radius of the orbital body
   ///
   /// \return Radius of the orbital body 
   ///
   ////////////////////////////////////////////////////////////
   double GetRadius() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the current epoch of the orbital body
   ///
   /// \return Current epoch of the orbital body
   ///
   ////////////////////////////////////////////////////////////
   const Epoch& GetEpoch() const;

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

private:
   double m_radius;              ///< Radius of the orbital body
   Epoch m_epoch;                ///< Current epoch of the orbital body
   EphemerisPointer m_ephemeris; ///< Smart pointer to ephemeris database
};

} // namespace otl