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

////////////////////////////////////////////////////////////
/// \class otl::MGADSMTrajectory
/// \ingroup core
///
/// otl::MGADSMTrajectory is a class that defines a Multiple
/// Gravity Assist trajectory with Deep Space Maneuvers (MGADSM).
///
/// A trajectory generally consists of a itinerary which defines
/// the high level overview, and the state vector which defines
/// the low level details that are needed to compute the position
/// and velocity of the spacecraft throught the trajectory.
/// 
/// For example, say that we want to travel from Earth to Mars
/// with a Venus flyby inbetween and a Deep Space Maneuver (DSM)
/// between Venus and Mars. What we have defined here is the
/// itinerary of the trajectory. In OTL, this is simply done as follows:
///
/// \code
/// MGADSMTrajectory trajectory;
/// trajectory.AddDeparture("Earth");
/// trajectory.AddFlyby("Venus");
/// trajectory.AddDSM();
/// trajectory.AddRendezvous("Mars");
/// \endcode
///
/// In order to actually calculate the spacecraft's position
/// and velocity throughout this trajectory, we need more
/// detailed information such as the departure date, time of
/// flight between the planets, flyby altitude, etc. These
/// details are stored in a state vector. Using our example above,
/// we can specify the state vector as follows:
///
/// \code
/// std::vector<double> states;
/// states.push_back(3867.51);  // Departure date (mjd2000)
/// states.push_back(117.17);   // Time of flight from Earth to Venus (days)
/// states.push_back(3331.84);  // Altitude of Venus flyby (km)
/// states.push_back(-1.62453); // B-inclination angle of Venus flyby (rad)
/// states.push_back(0.35435);  // Time of flight fraction of DSM between Venus and Mars
/// states.push_back(690.286);  // Time of flight from Venus and Mars
/// trajectory.SetStateVector(states);
/// \endcode
///
/// Now we have all of the information required to calculate
/// the trajectory. What we are usually interested in is the
/// total change in velocity (deltaV) required to achieve the
/// trajectory. The total deltaV is the sum of each individual
/// deltaV required throughout the trajectory. Calling the
/// Evaluate() function returns this vector of deltaVs:
///
/// \code
/// std::vector<double> deltaVs = trajectory.Evaluate();
/// double totalDeltaV = std::accumulate(deltaVs.begin(), deltaVs.end(), 0);
/// \endcode
///
/// Note that the order of states in the state vector matters, and
/// as such it is often easier to define the states at the same time
/// as the itinerary. For example, the above trajectory could have
/// been defined all at once as follows:
///
/// \code
/// MGADSMTrajectory trajectory;
/// trajectory.AddDeparture("Earth", 3867.51);
/// trajectory.AddFlyby("Venus", 117.17, 3331.84, -1.62453);
/// trajectory.AddDSM(0.35435);
/// trajectory.AddRendezvous("Mars", 690.286); 
/// std::vector<double> deltaVs = trajectory.Evaluate();
/// double totalDeltaV = std::accumulate(deltaVs.begin(), deltaVs.end(), 0);
/// \endcode
///
/// Here, the state vector is constructed internally as the
/// trajectory is defined. The function signatures make it
/// easy to understand what additional information is needed
/// in the state vector.
///
////////////////////////////////////////////////////////////

} // namespace otl