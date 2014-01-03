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

#include <OTL/Core/Base.hpp>

namespace otl
{

////////////////////////////////////////////////////////////
/**
 \brief Calculate canonical unit conversion factors
 
 This routine calculates the conversion factors for converting real units to canonical units.
 Working in canonical units typically improves computational efficiency and has the benefit
 that the gravitational parameter is unity which saves numerous mathematical operations.

 In Earth-centered reference frames, the equitorial radius of the Earth is typically used as
 the reference radius whereas in a Sun-centered reference frame, one astronomical unit (AU)
 is typically used as the reference radius.

 Canonical units are obtained by multiplying real units by the appropriate conversion factor.

 Usage example:
 \code
 double DU, TU, VU; // canonical unit converters
 CalculateCanonicalUnits(ASTRO_RADIUS_EARTH, ASTRO_MU_EARTH, DU, TU, VU);

 double distance = 10000.0; // km
 double canonicalDistance = distance * DU;

 double time = 1200.0; // seconds
 double canonicalTime = time * TU;

 \endcode

 \param radius Reference radius to be used as the distance unit (DU)
 \param mu Gravitional parameter of the central body
 \param [out] DU Conversion factor from real distance units to canonical distance units
 \param [out] TU Conversion factor from real time units to canonical time units
 \param [out] VU Conversion factor from real velocity units to canonical velocity units
 */
////////////////////////////////////////////////////////////
void CalculateCanonicalUnits(double radius, double mu,
                             double& DU, double& TU, double& VU);

////////////////////////////////////////////////////////////
/**
 \brief Convert state vectors to orbital elements
 
 This routine calculates the classical orbital elements (orbitalElements) of an object given its position
 and velocity (state) vectors.

 \Reference Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Algorithm 9

 \param stateVector Position and velocity vectors
 \param [out] orbitalElements Computed orbital elements
*/
////////////////////////////////////////////////////////////
void ConvertStateVector2OrbitalElements(const StateVector& stateVector,
                                        OrbitalElements& orbitalElements);

////////////////////////////////////////////////////////////
/**
 \brief Convert orbital elements to state vectors

 This routine calculates the position and velocity (state) vectors of an object given its
 classical orbital elements (orbitalElements).

 \Reference Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Algorithm 10

 \param orbitalElements rbital elements
 \param [out] stateVector Computed position and velocity vectors
*/
////////////////////////////////////////////////////////////
void ConvertOrbitalElements2StateVector(const OrbitalElements& orbitalElements,
                                        StateVector& stateVector);

////////////////////////////////////////////////////////////
Vector3d ConvertSpherical2Cartesian(double radius, double delta, double theta);

} // namespace otl