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
#include <OTL/Core/JplApproximateBody.h>

namespace otl
{

/// Define the Planet type as an alias of JplApproximateBody
typedef JplApproximateBody Planet;

////////////////////////////////////////////////////////////
/// \brief Planetary body identifiers
////////////////////////////////////////////////////////////
enum class PlanetId
{
   Invalid = -1,  ///< Invalid planet identifier
   Mercury,       ///< Mercury
   Venus,         ///< Venus
   Earth,         ///< Earth
   Mars,          ///< Mars
   Jupiter,       ///< Jupiter
   Saturn,        ///< Saturn
   Uranus,        ///< Uranus
   Neptune,       ///< Neptune
   Pluto,         ///< Pluto
   Count          ///< Number of planets
};

////////////////////////////////////////////////////////////
/// \brief Stream operator overload
/// \relates PlanetId
///
/// \param stream Templated stream object (e.g. ostream)
/// \returns T Reference to the stream object
///
////////////////////////////////////////////////////////////
template <typename T>
T& operator<<(T& stream, const PlanetId& planetId)
{
   stream << static_cast<int>(planetId);
   return stream;
}

////////////////////////////////////////////////////////////
/// \brief Helper function for converting a planet identifier into a planet name
///
/// \param planetId PlanetId enumerator identifier of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API std::string ConvertPlanetIdentifier2Name(PlanetId planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for converting a planet name into a planet identifier
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PlanetId ConvertPlanetName2Identifier(const std::string& name);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param planetId PlanetId enumerator identifier of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PhysicalProperties GetPlanetPhysicalProperties(const PlanetId& planetId);

////////////////////////////////////////////////////////////
/// \brief Helper function for retrieving the physical properties of a solar system planet
///
/// \param name Name of the planet
///
////////////////////////////////////////////////////////////
OTL_CORE_API PhysicalProperties GetPlanetPhysicalProperties(const std::string& planetName);

} // namespace otl

