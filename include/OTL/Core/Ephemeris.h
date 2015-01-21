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
#include <mutex>

namespace otl
{

class Epoch;
class IEphemeris;
typedef std::shared_ptr<IEphemeris> EphemerisPointer;


class OTL_CORE_API IEphemeris
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ////////////////////////////////////////////////////////////
    IEphemeris();
    //IEphemeris(const IEphemeris& other) = delete;
    //IEphemeris& operator=(const IEphemeris&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ////////////////////////////////////////////////////////////
    virtual ~IEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Query the ephemeris database for the position vector at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Entity name
    /// \param epoch Time at which the position vector is desired
    /// \param [out] position Resulting position vector
    ///
    ////////////////////////////////////////////////////////////
    void GetPosition(const std::string& name, const Epoch& epoch, Vector3d& position);

    ////////////////////////////////////////////////////////////
    /// \brief Query the ephemeris database for the velocity vector at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Entity name
    /// \param epoch Time at which the velocity vector is desired
    /// \param [out] velocity Resulting velocity vector
    ///
    ////////////////////////////////////////////////////////////
    void GetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity);

    ////////////////////////////////////////////////////////////
    /// \brief Query the ephemeris database for the state vector at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Entity name
    /// \param epoch Time at which the position vector is desired
    /// \param [out] stateVector Resulting state vector
    ///
    ////////////////////////////////////////////////////////////
    void GetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector);

    ////////////////////////////////////////////////////////////
    /// \brief Query the ephemeris database for the orbital elements at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name Entity name
    /// \param epoch Time at which the orbital elements is desired
    /// \param [out] orbitalElements Resulting orbital elements
    ///
    ////////////////////////////////////////////////////////////
    void GetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements);

protected:
    virtual void VLoad() = 0;
    virtual void VInitialize() = 0;
    virtual bool VIsValidName(const std::string& name) = 0;
    virtual bool VIsValidEpoch(const Epoch& epoch) = 0;
    virtual void VGetPosition(const std::string& name, const Epoch& epoch, Vector3d& position) = 0;
    virtual void VGetVelocity(const std::string& name, const Epoch& epoch, Vector3d& velocity) = 0;
    virtual void VGetStateVector(const std::string& name, const Epoch& epoch, StateVector& stateVector) = 0;
    virtual void VGetOrbitalElements(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements) = 0;

private:
    void Initialize();

private:
    bool m_initialized;
    std::mutex m_mutex;
};

} // namespace otl