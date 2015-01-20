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

////////////////////////////////////////////////////////////
/// \brief Calculate canonical unit conversion factors
/// \ingroup otl
/// 
/// This routine calculates the conversion factors for converting real units to canonical units.
/// Working in canonical units typically improves computational efficiency and has the benefit
/// that the gravitational parameter is unity which saves numerous mathematical operations.
///
/// In Earth-centered reference frames, the equitorial radius of the Earth is typically used as
/// the reference radius whereas in a Sun-centered reference frame, one astronomical unit (AU)
/// is typically used as the reference radius.
///
/// Canonical units are obtained by multiplying real units by the appropriate conversion factor.
///
/// Usage example:
/// \code
/// double DU, TU, VU; // canonical unit converters
/// CalculateCanonicalUnits(ASTRO_RADIUS_EARTH, ASTRO_MU_EARTH, DU, TU, VU);
///
/// double distance = 10000.0; // km
/// double canonicalDistance = distance * DU;
///
/// double time = 1200.0; // seconds
/// double canonicalTime = time * TU;
///
/// \endcode
///
/// \param radius Reference radius to be used as the distance unit (DU)
/// \param mu Gravitional parameter of the central body
/// \param [out] DU Conversion factor from real distance units to canonical distance units
/// \param [out] TU Conversion factor from real time units to canonical time units
/// \param [out] VU Conversion factor from real velocity units to canonical velocity units
///
////////////////////////////////////////////////////////////
void CalculateCanonicalUnits(double radius, double mu,
                             double& DU, double& TU, double& VU);

////////////////////////////////////////////////////////////
/// \brief Convert state vectors to orbital elements
/// \ingroup otl
/// 
/// Calculates the classical orbital elements of an object
/// given its position and velocity (state) vectors.
///
/// \Reference Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Algorithm 9
///
/// \param stateVector Position and velocity vectors
/// \param mu Gravitational parameter of the central body
/// \returns OrbitalElements after conversion
///
////////////////////////////////////////////////////////////
OrbitalElements ConvertStateVector2OrbitalElements(const StateVector& stateVector, double mu);

////////////////////////////////////////////////////////////
/// \brief Convert orbital elements to state vectors
/// \ingroup otl
///
/// Calculates the position and velocity (state) vectors of an
/// object given its classical orbital elements.
///
/// \Reference Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Algorithm 10
///
/// \param orbitalElements orbital elements
/// \param mu Gravitational parameter of the central body
/// \param [out] stateVector Computed position and velocity vectors
///
////////////////////////////////////////////////////////////
StateVector ConvertOrbitalElements2StateVector(const OrbitalElements& orbitalElements, double mu);

////////////////////////////////////////////////////////////
/// \brief Converts normalized spherical coordinates into a Cartesian vector
/// \ingroup otl
///
/// \param magnitude Magnitude of polar vector
/// \param normTheta Normalized azimuth angle between [0, 1]
/// \param normPhi Normalized inclination angle between [0, 1]
/// \returns Vector3d representing Cartesian vector
///
////////////////////////////////////////////////////////////
Vector3d ConvertNormalizedSpherical2Cartesian(double magnitude, double normTheta, double normPhi);

////////////////////////////////////////////////////////////
/// \brief Converts true anomaly to anomaly
/// \ingroup otl
///
/// This convenience function converts the true anomaly to
/// the appropriate anomaly (eccentric, hyperbolic, or parabolic)
/// depnding on the orbit type. Internally, this function will
/// call the following functions depending on the eccentricity:
/// \li ConvertTrueAnomaly2EccentricAnomaly()
/// \li ConvertTrueAnomaly2HyperbolicAnomaly()
/// \li ConvertTrueAnomaly2ParbolicAnomaly()
///
/// \param eccentricity Eccentricity
/// \param trueAnomaly True anomaly in radians
/// \returns Anomaly in radians
///
////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2Anomaly(double eccentricity, double trueAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts true anomaly to eccentric anomaly
/// \ingroup otl
///
/// The eccentric anomaly (E) is an auxillory angle for
/// elliptical orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ E = 2\atan{\sqrt{(1 - e)/(1 + e)}\tan{TA/2}} \f$
///
/// \param eccentricity Eccentricity
/// \param trueAnomaly True anomaly in radians
/// \returns Eccentric anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algoirthm 5, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2EccentricAnomaly(double eccentricity, double trueAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts true anomaly to hyperbolic anomaly
/// \ingroup otl
///
/// The hyperbolic anomaly (H) is an auxillory angle for
/// hyperbolic orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ H = 2\atanh{\sqrt{(e - 1)/(e + 1)}\tan{TA/2}} \f$
///
/// \param eccentricity Eccentricity
/// \param trueAnomaly True anomaly in radians
/// \returns Hyperbolic anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algoirthm 5, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2HyperbolicAnomaly(double eccentricity, double trueAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts true anomaly to parabolic anomaly
/// \ingroup otl
///
/// The parabolic anomaly (B) is an auxillory angle for
/// parabolic orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ B = \tanh{TA/2} \f$
///
/// \param eccentricity Eccentricity
/// \param trueAnomaly True anomaly in radians
/// \returns Parabolic anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algoirthm 5, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2ParabolicAnomaly(double trueAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts anomaly to true anomaly
/// \ingroup otl
///
/// This convenience function converts the anomaly (eccentric,
/// hyperbolic, or parabolic) to the true anomaly depnding
/// on the orbit type. Internally, this function will call the
/// following functions depending on the eccentricity:
/// \li ConvertEccentricAnomaly2TrueAnomaly()
/// \li ConvertHyperbolicAnomaly2TrueAnomaly()
/// \li ConvertParbolicAnomaly2TrueAnomaly()
///
/// \param eccentricity Eccentricity
/// \param anomaly Anomaly in radians
/// \returns True anomaly in radians
///
////////////////////////////////////////////////////////////
double ConvertAnomaly2TrueAnomaly(double eccentricity, double anomaly);

////////////////////////////////////////////////////////////
/// \brief Converts eccentric anomaly to true anomaly
/// \ingroup otl
///
/// The eccentric anomaly (E) is an auxillory angle for
/// elliptical orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ TA = 2\atan{\sqrt{(1 + e)/(1 - e)}\tan{E/2}} \f$
///
/// \param eccentricity Eccentricity
/// \param eccentricAnomaly Eccentric anomaly in radians
/// \returns True anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algorithm 6, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertEccentricAnomaly2TrueAnomaly(double eccentricity, double eccentricAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts hyperbolic anomaly to true anomaly
/// \ingroup otl
///
/// The hyperbolic anomaly (H) is an auxillory angle for
/// hyperbolic orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ TA = 2\atan{\sqrt{(e + 1)/(e - 1)}\tanh{H/2}} \f$
///
/// \param eccentricity Eccentricity
/// \param hyperbolicAnomaly Hyperbolic anomaly in radians
/// \returns True anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algoirthm 6, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertHyperbolicAnomaly2TrueAnomaly(double eccentricity, double hyperbolicAnomaly);

////////////////////////////////////////////////////////////
/// \brief Converts parabolic anomaly to true anomaly
/// \ingroup otl
///
/// The parabolic anomaly (B) is an auxillory angle for
/// parabolic orbits which is used as an intermediary angle
/// when converting between true anomaly and mean anomaly.
///
/// \f$ TA = 2\atan{B} \f$
///
/// \param eccentricity Eccentricity
/// \param parabolicAnomaly Parabolic anomaly in radians
/// \returns True anomaly in radians
///
/// \reference D. Vallado. Fundamentals of Astrodynamics and Applications 3rd Edition 2007. Algoirthm 6, section 2.2.6, page 85
///
////////////////////////////////////////////////////////////
double ConvertParabolicAnomaly2TrueAnomaly(double parabolicAnomaly);

} // namespace otl