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
#include <stdexcept>

namespace otl
{

class Exception : public std::runtime_error
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor using std::string 
    ///
    /// Creates an std::runtime_error with the given message.
    ///
    /// \param message std::string containing the error message
    ///
    ////////////////////////////////////////////////////////////
    explicit Exception(const std::string& message);

    ////////////////////////////////////////////////////////////
    /// \brief Constructor using c-style string 
    ///
    /// Creates an std::runtime_error with the given message.
    ///
    /// \param message Pointer to the error message
    ///
    ////////////////////////////////////////////////////////////
    Exception(const char* message);
};

} // namespace otl
