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

#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/OrbitalElements.h>
#include <OTL/Core/Time.h>

namespace otl
{

namespace keplerian
{

////////////////////////////////////////////////////////////
KeplerianPropagator::KeplerianPropagator()
{

}

////////////////////////////////////////////////////////////
KeplerianPropagator::~KeplerianPropagator()
{

}

////////////////////////////////////////////////////////////
OrbitalElements KeplerianPropagator::PropagateOrbitalElements(const OrbitalElements& orbitalElements, double mu, const Time& timeDelta)
{
   OrbitalElements newOrbitalElements(orbitalElements);
   newOrbitalElements.meanAnomaly = PropagateMeanAnomaly(orbitalElements, mu, timeDelta);
   return newOrbitalElements;
}

////////////////////////////////////////////////////////////
double KeplerianPropagator::PropagateMeanAnomaly(const OrbitalElements& orbitalElements, double mu, const Time& timeDelta)
{
   double meanMotion = sqrt(mu / std::abs(pow(orbitalElements.semiMajorAxis, 3.0)));
   return PropagateMeanAnomaly(orbitalElements.meanAnomaly, meanMotion, timeDelta);
}

////////////////////////////////////////////////////////////
double KeplerianPropagator::PropagateMeanAnomaly(double meanAnomaly, double meanMotion, const Time& timeDelta)
{
   return meanAnomaly + meanMotion * timeDelta.Seconds();
}

} // namespace keplerian

} // namespace otl