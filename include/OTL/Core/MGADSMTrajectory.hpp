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
#include <OTL/Core/MGADSMNodes.hpp>
#include <OTL/Core/Planet.hpp>
#include <OTL/Core/Propagate.hpp>
#include <OTL/Core/Lambert.hpp>
#include <OTL/Core/Flyby.hpp>

namespace otl
{

struct TrajectoryLeg
{
   Planet initialPlanet;
   Planet finalPlanet;

   int timeofFlightIndex;
   OrbitalElements insertionOrbit;

   bool departure;
   int numDSM;
   bool flyby;
   bool rendezvous;
   bool insertion;

   TrajectoryLeg();
};

class MGADSMTrajectory
{
public:
   MGADSMTrajectory();
   MGADSMTrajectory(const std::vector<TrajectoryNodePtr>& nodes);

   void Evaluate(const std::vector<double>& states, std::vector<double>& deltaVs);
   const std::vector<double>& Evaluate(const std::vector<double>& states);
   const std::vector<double>& Evaluate();

   // Node interface
   void AddDeparture(const DepartureNode& departureNode);
   void AddDSM(const DSMNode& dsmNode);
   void AddFlyby(const FlybyNode& flybyNode);
   void AddRendezvous(const RendezvousNode& rendezvousNode);
   void AddInsertion(const InsertionNode& insertionNode);
   void AddNode(const TrajectoryNodePtr& node);
   void AddNodes(const std::vector<TrajectoryNodePtr>& nodes);

   void SetDeparture(const DepartureNode& departureNode);
   void SetDSM(int dsm, const DSMNode& dsmNode);
   void SetFlyby(int flyby, const FlybyNode& flybyNode);
   void SetRendezvous(int rendezvous, const RendezvousNode& rendezvousNode);
   void SetInsertion(int insertion, const InsertionNode& insertionNode);
   void SetNode(int index, const TrajectoryNodePtr& node);
   void SetNodes(int index, const std::vector<TrajectoryNodePtr>& nodes);

   DepartureNode GetDeparture() const;
   DSMNode GetDSM(int dsm) const;
   FlybyNode GetFlyby(int flyby) const;
   RendezvousNode GetRendezvous(int rendezvous) const;
   InsertionNode GetInsertion(int insertion) const;

   // Parameter interface  
   void AddDeparture(const std::string& orbitalBody, Epoch epoch = 0.0, const Vector3d& deltaV = Vector3d()); 
   void AddDSM(double timeOfFlightFraction = 0.0, const Vector3d& deltaV = Vector3d()); 
   void AddFlyby(const std::string& orbitalBody, double timeOfFlight = 0.0, double altitude = 0.0, double bInclinationAngle = 0.0);
   void AddRendezvous(const std::string& orbitalBody, double timeOfFlight = 0.0);
   void AddInsertion(const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight = 0.0, double timeOfOrbit = 0.0);
    
   void SetDeparture(const std::string& orbitalBody, Epoch epoch, const Vector3d& deltaV = Vector3d());  
   void SetDSM(int dsm, double timeOfFlightFraction, const Vector3d& deltaV = Vector3d());  
   void SetFlyby(int flyby, const std::string& orbitalBody, double timeOfFlight, double altitude, double bInclinationAngle);
   void SetRendezvous(int rendezvous, const std::string& orbitalBody, double timeOfFlight);
   void SetInsertion(int insertion, const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight, double timeOfOrbit);

   // Setters 
   void SetStateVector(const std::vector<double>& states);
   void SetPropagateType(PropagateType type);
   void SetLambertType(LambertType type);
   void SetFlybyType(FlybyType type);

   // Getters
   std::vector<double> GetStateVector() const;
   std::vector<double> GetDeltaVs() const;
   int GetNumNodes() const;
   int GetNumStates() const;

private:
   void Init();
   void CalculateLegs();
   void CalculateTrajectory(const std::vector<double>& states, std::vector<double>& deltaVs);  
   void AddState(double state);

   int GetDSMIndex(int dsm) const;
   int GetFlybyIndex(int flyby) const;
   int GetInsertionIndex(int insertion) const;
   int GetRendezvousIndex(int rendezvous) const;

private: 
   int m_numNodes;
   int m_numStates;

   bool m_legsInitialized;
 
   std::vector<TrajectoryNodePtr> m_nodes;
   std::vector<TrajectoryLeg> m_legs;
   std::vector<double> m_states;
   std::vector<double> m_deltaVs;

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