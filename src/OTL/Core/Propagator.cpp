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

#include <OTL/Core/Propagator.h>

namespace otl
{

IPropagator::IPropagator()
{

}

IPropagator::~IPropagator()
{

}

OrbitalElements IPropagator::Propagate(const OrbitalElements& initialOrbitalElements, const Time& timeDelta, double mu)
{
   OTL_ASSERT(!timeDelta.IsInfinity(), "Failed to propagate orbital elements. Propagation time must be less than infinity");
   OTL_ASSERT(!initialOrbitalElements.IsZero(), "Failed to propagate orbital elements. Orbital elements unitialized");
   return (timeDelta.IsZero() ? initialOrbitalElements : VPropagate(initialOrbitalElements, timeDelta, mu));
}

StateVector IPropagator::Propagate(const StateVector& initialStateVector, const Time& timeDelta, double mu)
{
   OTL_ASSERT(!timeDelta.IsInfinity(), "Failed to propagate state vector. Propagation time must be less than infinity");
   OTL_ASSERT(!initialStateVector.IsZero(), "Failed to propagate state vector. State vector unintialized");
   return (timeDelta.IsZero() ? initialStateVector : VPropagate(initialStateVector, timeDelta, mu));
}

} // namespace otl