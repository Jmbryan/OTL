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


class IEphemeris
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ////////////////////////////////////////////////////////////
    //IEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ////////////////////////////////////////////////////////////
    virtual ~IEphemeris();

    ////////////////////////////////////////////////////////////
    /// \brief Query the database for the state vector of a natural body at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name std::string which represents the name of the natural body
    /// \param epoch Epoch which represents the time at which the state vector is desired
    /// \param [out] stateVector Resulting StateVector computed at the given Epoch
    ///
    ////////////////////////////////////////////////////////////
    void QueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector);

    ////////////////////////////////////////////////////////////
    /// \brief Query the database for the orbital elements of a natural body at a given epoch
    ///
    /// This is a pure virtual method which must be reimplemented by
    /// the derived class.
    ///
    /// \param name std::string which represents the name of the natural body
    /// \param epoch Epoch which represents the time at which the state vector is desired
    /// \param [out] orbitalElements Resulting OrbitalElements computed at the given Epoch
    ///
    ////////////////////////////////////////////////////////////
    void QueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements);

    static EphemerisPointer GetInstance();

protected:
    virtual void VLoad() = 0;
    virtual void VInitialize() = 0;
    virtual bool VIsNameValid(const std::string& name) = 0;
    virtual bool VIsEpochValid(const Epoch& epoch) = 0;
    virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector) = 0;
    virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements) = 0;

    IEphemeris();
    IEphemeris(const IEphemeris& other) = delete;
    IEphemeris& operator=(const IEphemeris&) = delete;

private:
    void Initialize();

private:
    bool m_initialized;
    std::mutex m_mutex;
    static EphemerisPointer instance;
};

} // namespace otl