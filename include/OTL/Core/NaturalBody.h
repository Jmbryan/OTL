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
#include <OTL/Core/OrbitalBody.h>
#include <OTL/Core/Epoch.h>

namespace otl
{

class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;

class OTL_CORE_API NaturalBody : public OrbitalBody
{
public:

   struct PhysicalProperties
   {
      double mass;      ///< Mass
      double mu;        ///< Gravitational parameter
      double radius;    ///< Equatorial radius

      PhysicalProperties();
      PhysicalProperties(double _mass, double _mu, double _radius);

      std::string ToString() const;
      std::string ToDetailedString(std::string prefix = "") const;
   };

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   NaturalBody();

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the natural body
   ///
   /// \param name std::string represeting the name of the natural body 
   /// \param mass Mass of the natural body
   /// \param radius Radius of the natural body
   /// \param Epoch of the natural body
   ///
   ////////////////////////////////////////////////////////////
   NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Constructs the natural body
   ///
   /// \param name std::string represeting the name of the natural body 
   /// \param mass Mass of the natural body
   /// \param radius Radius of the natural body
   /// \param Epoch of the natural body
   /// \param EphemerisPointer Smart pointer to the ephemeris database
   ///
   ////////////////////////////////////////////////////////////
   NaturalBody(const std::string& name, double mass, double radius, const Epoch& epoch, const EphemerisPointer& ephemeris);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~NaturalBody();

   ////////////////////////////////////////////////////////////
   /// \brief Set the physical properties of the natural body
   ///
   /// \param physicalProperties PhysicalProperties of the natural body
   ///
   ////////////////////////////////////////////////////////////
   void SetPhysicalProperties(const PhysicalProperties& physicalProperties);

   ////////////////////////////////////////////////////////////
   /// \brief Set the epoch of the natural body
   ///
   /// \param epoch epoch of the natural body
   ///
   ////////////////////////////////////////////////////////////
   void SetEpoch(const Epoch& epoch);

   ////////////////////////////////////////////////////////////
   /// \brief Set an ephemeris database of the natural body
   ///
   /// \param ephemeris Smart pointer to the IEphemeris database
   ///
   ////////////////////////////////////////////////////////////
   void SetEphemeris(const EphemerisPointer& ephemeris);

   ////////////////////////////////////////////////////////////
   /// \brief Use an ephemeris database to propagate the natural body
   ///
   /// If this option is set to true, then the natural body
   /// will use an IEphemeris database instead of the IPropagator
   /// to propagate the natural body.
   ///
   /// \warning If this option is true, the SetEphemeris() function
   /// must be called prior to propagating the natural body. Failure
   /// to do so will result in a InvalidPointerException to be thrown.
   ///
   /// \param bool true to use ephemeris for propagation
   ///
   ////////////////////////////////////////////////////////////
   void UseEphemerisForPropagation(bool useEphemerisForPropagation);
       
   ////////////////////////////////////////////////////////////
   /// \brief Get the current epoch of the natural body
   ///
   /// \return Current Epoch of the natural body
   ///
   ////////////////////////////////////////////////////////////
   const Epoch& GetEpoch() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the physical properties of the natural body
   ///
   /// \return PhysicalProperties of the natural body
   ///
   ////////////////////////////////////////////////////////////
   const PhysicalProperties& GetPhysicalProperties() const;

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

   std::string ToString() const;
   std::string ToDetailedString(std::string prefix = "") const;

private:
   PhysicalProperties m_physicalProperties;  ///< Physical properties of the natural body
   Epoch m_epoch;                            ///< Current epoch of the natural body
   EphemerisPointer m_ephemeris;             ///< Smart pointer to ephemeris database
   bool m_useEphemerisForPropagation;        ///< If true, ephemeris is used to propagate orbit
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates NaturalBody
///
/// The natural body is converted to a string by calling the
/// NaturalBody::ToString() method.
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template<typename T>
T& operator<<(T& stream, const NaturalBody& naturalBody)
{
   stream << naturalBody.ToString();
   return stream;
}

} // namespace otl