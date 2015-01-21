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

////////////////////////////////////////////////////////////
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
////////////////////////////////////////////////////////////
#if defined(_WIN32)

// Windows
#define OTL_SYSTEM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(__APPLE__) && defined(__MACH__)

// Apple platform, see which one it is
#include "TargetConditionals.h"

#if TARGET_OS_MAC

// MacOS
#define OTL_SYSTEM_MACOS

#else

// Unsupported Apple system
#error This Apple operating system is not supported by OTL library

#endif

#elif defined(__unix__)

// UNIX system, see which one it is
#if defined(__linux__)

// Linux
#define OTL_SYSTEM_LINUX

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

// FreeBSD
#define OTL_SYSTEM_FREEBSD

#else

// Unsupported UNIX system
#error This UNIX operating system is not supported by OTL library

#endif

#else

// Unsupported system
#error This operating system is not supported by OTL library

#endif

////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

#define OTL_DEBUG

#endif

////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if !defined(OTL_STATIC)

#if defined(OTL_SYSTEM_WINDOWS)

// Windows compilers need specific (and different) keywords for export and import
#define OTL_API_EXPORT __declspec(dllexport)
#define OTL_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER

#pragma warning(disable: 4251)

#endif

#else // Linux, FreeBSD, Mac OS X

#if __GNUC__ >= 4

// GCC 4 has special keywords for showing/hidding symbols,
// the same keyword is used for both importing and exporting
#define OTL_API_EXPORT __attribute__ ((__visibility__ ("default")))
#define OTL_API_IMPORT __attribute__ ((__visibility__ ("default")))

#else

// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
#define OTL_API_EXPORT
#define OTL_API_IMPORT

#endif

#endif

#else

// Static build doesn't need import/export macros
#define OTL_API_EXPORT
#define OTL_API_IMPORT

#endif