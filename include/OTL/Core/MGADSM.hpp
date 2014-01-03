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
#include <OTL/Core/Base.hpp>
#include <OTL/Core/Planet.hpp>
#include <OTL/Core/Propagate.hpp>
#include <OTL/Core/Lambert.hpp>
#include <OTL/Core/Flyby.hpp>

namespace otl
{

struct TrajectoryLeg
{
   Planet::PlanetId initialPlanetId;
   Planet::PlanetId finalPlanetId;

   int timeofFlightIndex;

   bool departure;
   int numDSM;
   bool flyby;
   bool rendezvous;
   bool insertion;

   OrbitalElements insertionOrbit;

   TrajectoryLeg() :
   initialPlanetId(Planet::PlanetId::Invalid),
   finalPlanetId(Planet::PlanetId::Invalid),
   departure(false),
   numDSM(0),
   flyby(false),
   rendezvous(false),
   insertion(false)
   {
   }
};

struct TrajectoryNode
{
   enum class Type
   {
      Invalid = -1,
      Departure,
      DSM,
      Flyby,
      Rendezvous,
      Insertion,
      Count
   };

   enum class Geometry
   {
      Invalid = -1,
      NA,
      Planet,
      Moon,
      Asteroid,
      Comet,
      Count
   };

   virtual ~TrajectoryNode() {}

   Type type;
   Geometry geometry;
};

struct DepartureNode : public TrajectoryNode
{
   Planet::PlanetId planetId;

   double julianDateMin;
   double julianDateMax;

   Vector3d departureVectorMin;
   Vector3d departureVectorMax;
};

struct FlybyNode : public TrajectoryNode
{
   Planet::PlanetId planetId;

   double timeOfFlightMin;
   double timeOfFlightMax;

   double altitudeMin;
   double altitudeMax;

   double bInclinationMin;
   double bInclinationMax;
};

struct DSMNode : public TrajectoryNode
{
   double alphaMin;
   double alphaMax;

   Vector3d vectorMin;
   Vector3d vectorMax;
};

struct RendezvousNode : public TrajectoryNode
{
   Planet::PlanetId planetId;

   double timeOfFlightMin;
   double timeOfFlightMax;
};

struct InsertionNode : public RendezvousNode
{
   OrbitalElements orbit;

   double orbitTimeMin;
   double orbitTimeMax;
};

class MGADSM
{
public:
   MGADSM();
   MGADSM(const std::vector<TrajectoryNode*>& nodes);

   void CalculateTrajectory(const std::vector<double>& states, std::vector<double>& deltaVs);

private:
   void Init(const std::vector<TrajectoryNode*>& nodes);
   void AddState(double lowerBound, double upperBound);
   void AddStates(const Vector3d& lowerBounds, const Vector3d& upperBounds);

private:
   int m_numNodes;
   int m_numStates;

   std::vector<Planet> m_itinerary;
   std::vector<TrajectoryLeg> m_legs;
   std::vector<double> m_lowerBounds;
   std::vector<double> m_upperBounds;

   std::unique_ptr<IPropagateAlgorithm> m_propagator;
   std::unique_ptr<ILambertAlgorithm> m_lambert;
   std::unique_ptr<IFlybyAlgorithm> m_flyby;

   // Temporary variables used in CalculateTrajectory()
   Epoch m_initialEpoch;
   Epoch m_finalEpoch;
   StateVector m_initialStateVector;
   StateVector m_finalStateVector;
   StateVector m_planetStateVector;
};

} // namespace otl