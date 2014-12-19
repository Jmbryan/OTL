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
#include <OTL/Core/Ephemeris.h>

namespace otl
{

// Forward declarations
namespace keplerian {
class IKeplersEquation;
typedef std::shared_ptr<IKeplersEquation> KeplersEquationPointer;
}

class JplApproximateEphemeris : public IEphemeris
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ////////////////////////////////////////////////////////////
    JplApproximateEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ////////////////////////////////////////////////////////////
    virtual ~JplApproximateEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Query the database for the state vector of a planet at a given epoch
    ///
    /// This function is only applicable to the major planets and Pluto.
    ///
    /// \param name std::string which represents the name of the planet
    /// \param epoch Epoch which represents the time at which the state vector is desired
    /// \param [out] stateVector Resulting StateVector computed at the given Epoch
    ///
    ////////////////////////////////////////////////////////////
    virtual void QueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector);

    ////////////////////////////////////////////////////////////
    /// \brief Query the database for the orbital elements of a planet at a given epoch
    ///
    /// This function is only applicable to the major planets and Pluto.
    ///
    /// \param name std::string which represents the name of the planet
    /// \param epoch Epoch which represents the time at which the state vector is desired
    /// \param [out] orbitalElements Resulting OrbitalElements computed at the given Epoch
    ///
    ////////////////////////////////////////////////////////////
    virtual void QueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements);

private:
    virtual void Initialize();

private:
    keplerian::KeplersEquationPointer m_keplersEquation; ///< Keplers equation used to convert mean anomaly to eccentric anomaly
};

} // namespace otl

////////////////////////////////////////////////////////////
/// \class otl::JplApproximateEphemeris
/// \ingroup otl
///
/// Implements JPL's "Keplerian Elements for Approximate
/// Positions of the Major Planets".
///
/// This ephemeris routine analytically computes the orbital elements
/// of a planetary body at a given epoch using a simple database
/// consisting of two coefficients for each orbital element; namely
/// the reference element and the rate. The general equation is:
///
/// \f$ element = element_0 + rate * (num centures since J2000) \f$
///
/// The exception is for the computation of the mean anomaly for
/// Jupiter through Pluto which require four additional coefficients.
///
/// The benefit of using an analytical ephemeris versus querying a
/// traditional high precision ephemeris database such as SPICE is
/// that it is typically much faster (approximatly an order of magnitude),
/// but at the cost of precision. However, applications such as trajectory
/// design and optimization do not require high precision ephemeris and
/// can greatly benefity from the speedup offered by an analytical ephemeris
/// alternative.
///
/// The state vector and orbital elements of the planet
/// at a desired Epoch can be obtained by calling
/// the inherited overloaded member function QueryDatabase().
///
/// This routine is only valid for the major planets and Pluto. Calling
/// the QueryDatabase() functions with any name other than those listed
/// below will cause a InvalidArguementException to be thrown:
/// \li Mercury
/// \li Venus
/// \li Earth (Earth-Moon barycenter)
/// \li Mars
/// \li Jupiter
/// \li Saturn
/// \li Uranus
/// \li Neptune
/// \li Pluto
///
/// \note This routine is only valid for the time
/// period between 3000 BC to 3000 AD.
///
/// \note This routine is considered an approximate
/// (low precision) method. For a high precision ephemeris,
/// consider using the SpiceEphemeris class.
///
/// \reference http://ssd.jpl.nasa.gov/?planet_pos
///
/// \see IEphemeris, Epoch, StateVector, OrbitalElements
///
////////////////////////////////////////////////////////////