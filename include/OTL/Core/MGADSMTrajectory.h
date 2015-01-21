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
#include <OTL/Core/MGADSMNodes.h>
#include <OTL/Core/Planet.h>
#include <OTL/Core/Propagator.h>
#include <OTL/Core/Lambert.h>
#include <OTL/Core/Flyby.h>

namespace otl
{

////////////////////////////////////////////////////////////
/// \defgroup keplerian Keplerian
///
/// Classes and functions specific to Keplerian dynamics which
/// assume instantaneous changes in velocity. Lambert's problem,
/// orbit propagation, flybys, interplanetary trajectories and more.
///
////////////////////////////////////////////////////////////
namespace keplerian
{

////////////////////////////////////////////////////////////
/// \defgroup trajectory Trajectory
/// \ingroup keplerian
///
/// Classes and functions specific to interplanetary trajectories.
///
////////////////////////////////////////////////////////////
namespace trajectory
{

////////////////////////////////////////////////////////////
/// \brief Data structure representing one "leg" of a MGADSMTrajectory.
/// \ingroup trajectory
///
/// The itinerary of an otl::keplerian::MGADSMTrajectory is
/// defined by a vector of TrajectoryNodes. Before computing
/// a trajectory, the TrajectoryNodes are translated into a
/// vector of TrajectoryLegs which are more efficent data
/// structures and contain additional precomputed information
/// requried for calculating the trajectory.
/// 
////////////////////////////////////////////////////////////
struct OTL_CORE_API TrajectoryLeg
{
   Planet initialPlanet;            ///< Initial planet of the leg
   Planet finalPlanet;              ///< Final planet of the leg

   int timeofFlightIndex;           ///< Index of the state vector that contains the time of flight for the leg
   OrbitalElements insertionOrbit;  ///< Orbital elements of the insertion orbit at the end of the leg (if applicable)

   bool departure;                  ///< True if the leg contains a DepartureNode
   int numDSM;                      ///< The number of DSMNodes that the leg contains
   bool flyby;                      ///< True if the leg contains a FlybyNode
   bool rendezvous;                 ///< True if the leg contains a RendezvousNode
   bool insertion;                  ///< True if the leg contains a InsertionNode

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   TrajectoryLeg();
};

class OTL_CORE_API MGADSMTrajectory
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   MGADSMTrajectory();

   ////////////////////////////////////////////////////////////
   /// \brief Create the trajectory with the vector of TrajectoryNode pointers
   ///
   /// The first node must be a DepartureNode.
   ///
   /// \param nodes Vector of TrajectoryNodePtr
   ///
   ////////////////////////////////////////////////////////////
   MGADSMTrajectory(const std::vector<TrajectoryNodePtr>& nodes);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the trajectory using the given the state vector
   ///
   /// \param states Vector of states used throughout the trajectory
   /// \param [out] deltaVs Vector of computed deltaVs required throughout the trajectory
   ///
   ////////////////////////////////////////////////////////////
   void Evaluate(const std::vector<double>& states, std::vector<double>& deltaVs);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the trajectory using the given the state vector
   ///
   /// A valid itinerary must already be defined.
   ///
   /// \param states Vector of states used throughout the trajectory
   /// \returns Vector of computed deltaVs required throughout the trajectory
   ///
   ////////////////////////////////////////////////////////////
   const std::vector<double>& Evaluate(const std::vector<double>& states);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate the trajectory using the internal the state vector
   ///
   /// A valid itinerary and state vector must already be defined.
   ///
   /// \returns Vector of computed deltaVs required throughout the trajectory
   ///
   ////////////////////////////////////////////////////////////
   const std::vector<double>& Evaluate();

   ////////////////////////////////////////////////////////////
   /// \brief Add a DepartureNode to the trajectory itinerary
   ///
   /// A departureNode must be and can only be the first node of
   /// a trajectory.
   ///
   /// \param departureNode DepartureNode to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddDeparture(const DepartureNode& departureNode);

   ////////////////////////////////////////////////////////////
   /// \brief Add a DSMNode to the trajectory itinerary
   ///
   /// \param dsmNode DSMNode to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddDSM(const DSMNode& dsmNode);

   ////////////////////////////////////////////////////////////
   /// \brief Add a FlybyNode to the trajectory itinerary
   ///
   /// \param flybyNode FlybyNode to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddFlyby(const FlybyNode& flybyNode);

   ////////////////////////////////////////////////////////////
   /// \brief Add a RendezvousNode to the trajectory itinerary
   ///
   /// \param rendezvousNode RendezvousNode to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddRendezvous(const RendezvousNode& rendezvousNode);

   ////////////////////////////////////////////////////////////
   /// \brief Add a InsertionNode to the trajectory itinerary
   ///
   /// \param insertionNode InsertionNode to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddInsertion(const InsertionNode& insertionNode);

   ////////////////////////////////////////////////////////////
   /// \brief Add a TrajectoryNodePtr to the trajectory itinerary
   ///
   ////////////////////////////////////////////////////////////
   void AddNode(const TrajectoryNodePtr& node);

   ////////////////////////////////////////////////////////////
   /// \brief Add a vector of TrajectoryNodePtrs to the trajectory itinerary
   ///
   /// If no nodes have been previously added, then the first node
   /// must be a DepartureNode.
   ///
   /// \param nodes Vector of TrajectoryNodePtr to be added
   ///
   ////////////////////////////////////////////////////////////
   void AddNodes(const std::vector<TrajectoryNodePtr>& nodes);

   ////////////////////////////////////////////////////////////
   /// \brief Set the DepartureNode
   ///
   /// Overrides the existing DepartureNode. A DepartureNode
   /// must already be set in order to use this function.
   /// 
   /// \param departureNode New DepartureNode
   ///
   ////////////////////////////////////////////////////////////
   void SetDeparture(const DepartureNode& departureNode);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th DSMNode
   ///
   /// Overrides the existing n'th DSMNode. At least n DSMNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param N'th DSMNode to override
   /// \param dsmNode New DSMNode
   ///
   ////////////////////////////////////////////////////////////
   void SetDSM(int n, const DSMNode& dsmNode);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th FlybyNode
   ///
   /// Overrides the existing n'th FlybyNode. At least n FlybyNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param n N'th FlybyNode to override
   /// \param flybyNode New FlybyNode
   ///
   ////////////////////////////////////////////////////////////
   void SetFlyby(int n, const FlybyNode& flybyNode);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th RendezvousNode
   ///
   /// Overrides the existing n'th RendezvousNode. At least n RendezvousNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param n N'th RendezvousNode to override
   /// \param rendezvousNode New RendezvousNode
   ///
   ////////////////////////////////////////////////////////////
   void SetRendezvous(int n, const RendezvousNode& rendezvousNode);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th InsertionNode
   ///
   /// Overrides the existing n'th InsertionNode. At least n InsertionNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param n N'th InsertionNode to override
   /// \param insertionNode New InsertionNode
   ///
   ////////////////////////////////////////////////////////////
   void SetInsertion(int n, const InsertionNode& insertionNode);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th node
   ///
   /// Overrides the existing n'th TrajectoryNode. At least n nodes 
   /// must already be set in order to use this function and the
   /// existing n'th node must be of the same type as the new node.
   /// 
   /// \param n Override the n'th node
   /// \param node New TrajectoryNodePtr
   ///
   ////////////////////////////////////////////////////////////
   void SetNode(int n, const TrajectoryNodePtr& node);

   ////////////////////////////////////////////////////////////
   /// \brief Set the nodes starting at node n
   ///
   /// Overrides a series of TrajectoryNodes starting at index n.
   /// At least n + nodes.size() nodes must already be set in order
   /// to use this function and the existing nodes must be of the
   /// same type as the new nodes.
   /// 
   /// \param n Override the nodes starting at index n
   /// \param node Vector of new TrajectoryNodePtrs
   ///
   ////////////////////////////////////////////////////////////
   void SetNodes(int n, const std::vector<TrajectoryNodePtr>& nodes);

   ////////////////////////////////////////////////////////////
   /// \brief Get the DepartureNode
   ///
   /// This function requires that a DepartureNode already be
   /// added to the trajectory.
   ///
   /// \returns The DepartureNode
   ///
   ////////////////////////////////////////////////////////////
   DepartureNode GetDeparture() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the n'th DSMNode
   ///
   /// This function requires that at least n DSMNodes already be
   /// added to the trajectory.
   ///
   /// \returns The n'th DSMNode
   ///
   ////////////////////////////////////////////////////////////
   DSMNode GetDSM(int n) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the n'th FlybyNode
   ///
   /// This function requires that at least n FlybyNodes already be
   /// added to the trajectory.
   ///
   /// \returns The n'th FlybyNode
   ///
   ////////////////////////////////////////////////////////////
   FlybyNode GetFlyby(int n) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the n'th RendezvousNode
   ///
   /// This function requires that at least n RendezvousNodes already be
   /// added to the trajectory.
   ///
   /// \returns The n'th RendezvousNode
   ///
   ////////////////////////////////////////////////////////////
   RendezvousNode GetRendezvous(int n) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the n'th InsertionNode
   ///
   /// This function requires that at least n InsertionNodes already be
   /// added to the trajectory.
   ///
   /// \returns The n'th InsertionNode
   ///
   ////////////////////////////////////////////////////////////
   InsertionNode GetInsertion(int n) const;

   // Parameter interface

   ////////////////////////////////////////////////////////////
   /// \brief Add a DepartureNode to the trajectory itinerary
   ///
   /// A DepartureNode must be and can only be the first node of
   /// a trajectory.
   ///
   /// All parameters other than the name of the orbitalBody are
   /// optional and if left blank will default to their default values
   /// of zero. Defaulted parameters must be set later by calling
   /// either SetStateVector() or SetDeparture() before attempting
   /// to evaluate the trajectory.
   ///
   /// If the DepartureNode is followed by a DSMNode,
   /// then the deltaV vector must be supplied which defines the
   /// departure deltaV vector [dV, thetaNorm, deltaNorm] where
   /// dV is the deltaV magnitude and thetaNorm and deltaNorm are
   /// the normalized departure angles between zero and one.
   ///
   /// \param orbitalBody Name of the orbital body to departing from
   /// \param epoch Epoch of departure
   /// \param deltaV Vector3d which defines the deltaV vector if followed by a DSM
   ///
   ////////////////////////////////////////////////////////////
   void AddDeparture(const std::string& orbitalBody, const Epoch& epoch = Epoch(), const Vector3d& deltaV = Vector3d()); 
   
   ////////////////////////////////////////////////////////////
   /// \brief Add a DSMNode to the trajectory itinerary
   ///
   /// All parameters are optional and if left blank will default
   /// to their default values of zero. Defaulted parameters
   /// must be set later by calling either SetStateVector()
   /// or SetDSM() before attempting to evaluate the trajectory.
   ///
   /// If the DSMNode is following a previous DSMNode, then the
   /// deltaV vector [dV, thetaNorm, deltaNorm] must be supplied
   /// where dV is the deltaV magnitude and thetaNorm and deltaNorm
   /// are the normalized vector angles between zero and one.
   ///
   /// \param timeOfFlightFraction Fractional time along the trajectory leg at which the DSM occurs 
   /// \param deltaV Vector3d which defines the deltaV vector if following a DSM
   ///
   ////////////////////////////////////////////////////////////
   void AddDSM(double timeOfFlightFraction = 0.0, const Vector3d& deltaV = Vector3d()); 

   ////////////////////////////////////////////////////////////
   /// \brief Add a FlybyNode to the trajectory itinerary
   ///
   /// All parameters other than the name of the orbitalBody are
   /// optional and if left blank will default to their default values
   /// of zero. Defaulted parameters must be set later by calling
   /// either SetStateVector() or SetFlyby() before attempting
   /// to evaluate the trajectory.
   ///
   /// \param orbitalBody Name of the orbital body to flyby
   /// \param timeOfFlight Time since the previous orbital body
   /// \param altitude Altitude of the periapsis of the flyby hyperbola
   /// \param bInclinationAngle Angle which defines the 2d plane of the flyby hyperbola
   ///
   ////////////////////////////////////////////////////////////
   void AddFlyby(const std::string& orbitalBody, double timeOfFlight = 0.0, double altitude = 0.0, double bInclinationAngle = 0.0);
   
   ////////////////////////////////////////////////////////////
   /// \brief Add a RendezvousNode to the trajectory itinerary
   ///
   /// All parameters other than the name of the orbitalBody are
   /// optional and if left blank will default to their default values
   /// of zero. Defaulted parameters must be set later by calling
   /// either SetStateVector() or SetRendezvous() before attempting
   /// to evaluate the trajectory.
   ///
   /// \param orbitalBody Name of the orbital body to rendezvous with
   /// \param timeOfFlight Time since the previous orbital body
   ///
   ////////////////////////////////////////////////////////////
   void AddRendezvous(const std::string& orbitalBody, double timeOfFlight = 0.0);

   ////////////////////////////////////////////////////////////
   /// \brief Add a InsertionNode to the trajectory itinerary
   ///
   /// All parameters other than the name of the orbitalBody and
   /// the orbital elements are optional and if left blank will
   /// default to their default values of zero. Defaulted parameters
   /// must be set later by calling either SetStateVector() or
   /// SetInsertion() before attempting to evaluate the trajectory.
   ///
   /// If the InsertionNode is followed by a DSMNode,
   /// then the deltaV vector must be supplied which defines the
   /// departure deltaV vector [dV, thetaNorm, deltaNorm] where
   /// dV is the deltaV magnitude and thetaNorm and deltaNorm are
   /// the normalized departure angles between zero and one.
   ///
   /// \param orbitalBody Name of the orbital body to insert into orbit about
   /// \param orbitalElements Orbital elements of the insertion orbit
   /// \param timeOfFlight Time since the previous orbital body
   /// \param timeOfOrbit Time to stay in the insertion orbit
   /// \param deltaV Vector3d which defines the deltaV vector if followed by a DSM
   ///
   ////////////////////////////////////////////////////////////
   void AddInsertion(const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight = 0.0, double timeOfOrbit = 0.0);
   
   ////////////////////////////////////////////////////////////
   /// \brief Set the DepartureNode
   ///
   /// Overrides the existing DepartureNode. A DepartureNode
   /// must already be set in order to use this function.
   /// 
   /// \param orbitalBody Name of the orbital body to departing from
   /// \param epoch Epoch of departure
   /// \param deltaV Vector3d which defines the deltaV vector if followed by a DSM
   ///
   ////////////////////////////////////////////////////////////
   void SetDeparture(const std::string& orbitalBody, const Epoch& epoch, const Vector3d& deltaV = Vector3d());

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th DSMNode
   ///
   /// Overrides the existing n'th DSMNode. At least n DSMNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param timeOfFlightFraction Fractional time along the trajectory leg at which the DSM occurs 
   /// \param deltaV Vector3d which defines the deltaV vector if following a DSM
   ///
   ////////////////////////////////////////////////////////////
   void SetDSM(int dsm, double timeOfFlightFraction, const Vector3d& deltaV = Vector3d());

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th FlybyNode
   ///
   /// Overrides the existing n'th FlybyNode. At least n FlybyNodes
   /// must already be set in order to use this function.
   /// 
   /// \param orbitalBody Name of the orbital body to flyby
   /// \param timeOfFlight Time since the previous orbital body
   /// \param altitude Altitude of the periapsis of the flyby hyperbola
   /// \param bInclinationAngle Angle which defines the 2d plane of the flyby hyperbola
   ///
   ////////////////////////////////////////////////////////////
   void SetFlyby(int flyby, const std::string& orbitalBody, double timeOfFlight, double altitude, double bInclinationAngle);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th RendezvousNode
   ///
   /// Overrides the existing n'th RendezvousNode. At least n RendezvousNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param orbitalBody Name of the orbital body to rendezvous with
   /// \param timeOfFlight Time since the previous orbital body
   ///
   ////////////////////////////////////////////////////////////
   void SetRendezvous(int rendezvous, const std::string& orbitalBody, double timeOfFlight);

   ////////////////////////////////////////////////////////////
   /// \brief Set the n'th InsertionNode
   ///
   /// Overrides the existing n'th InsertionNode. At least n InsertionNodes 
   /// must already be set in order to use this function.
   /// 
   /// \param orbitalBody Name of the orbital body to insert into orbit about
   /// \param orbitalElements Orbital elements of the insertion orbit
   /// \param timeOfFlight Time since the previous orbital body
   /// \param timeOfOrbit Time to stay in the insertion orbit
   /// \param deltaV Vector3d which defines the deltaV vector if followed by a DSM
   ///
   ////////////////////////////////////////////////////////////
   void SetInsertion(int insertion, const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight, double timeOfOrbit, const Vector3d& deltaV = Vector3d());

   // Setters

   ////////////////////////////////////////////////////////////
   /// \brief Set the StateVector
   ///
   /// Overrides any states previously set during node creation
   /// or previous calls to SetStateVector(). The states must
   /// be defined the the correct order depending on the node
   /// configuration. If the order is not known, then it is
   /// safe to use the SetDeparture, SetDSM, etc.. function
   /// to define the state vector instead.
   ///
   /// The new states are written to the TrajectoryNodes so that
   /// proceeding calls to GetDeparture(), GetFlyby(), etc..
   /// will return the node with the updated states.
   /// 
   /// \param states Vector of states required to compute the trajectory
   ///
   ////////////////////////////////////////////////////////////
   void SetStateVector(const std::vector<double>& states);

   ////////////////////////////////////////////////////////////
   /// \brief Set the type of Propagation algorithm to be used
   ///
   /// The default type is PropagateType::StateVector.
   /// 
   /// \param type Type of propagation algorithm
   ///
   ////////////////////////////////////////////////////////////
   void SetPropagateType(PropagateType type);

   ////////////////////////////////////////////////////////////
   /// \brief Set the type of Lambert algorithm to be used
   ///
   /// The default type is LambertType::ExponentialSinusoid.
   /// 
   /// \param type Type of Lambert algorithm
   ///
   ////////////////////////////////////////////////////////////
   void SetLambertType(LambertType type);

   ////////////////////////////////////////////////////////////
   /// \brief Set the type of Flyby algorithm to be used
   ///
   /// The default type is FlybyType::Unpowered.
   /// 
   /// \param type Type of Flyby algorithm
   ///
   ////////////////////////////////////////////////////////////
   void SetFlybyType(FlybyType type);

   // Getters

   ////////////////////////////////////////////////////////////
   /// \brief Get the StateVector
   ///
   /// \returns Vector of states required to compute the trajectory
   ///
   ////////////////////////////////////////////////////////////
   std::vector<double> GetStateVector() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the DeltaV vector
   ///
   /// \returns Vector of computed deltaVs requried to achieve the trajectory
   ///
   ////////////////////////////////////////////////////////////
   std::vector<double> GetDeltaVs() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the number of nodes added to the trajectory itinerary
   ///
   /// \returns Number of nodes
   ///
   ////////////////////////////////////////////////////////////
   int GetNumNodes() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the number of states required to compute trajectory
   ///
   /// \returns Number of states
   ///
   ////////////////////////////////////////////////////////////
   int GetNumStates() const;

private:
   ////////////////////////////////////////////////////////////
   /// \brief Initialize the trajectory
   ///
   /// This function is called upon creation of the MGADSMTrajectory
   /// and performs some initialization steps such as creating the
   /// default algorithms.
   ///
   ////////////////////////////////////////////////////////////
   void Init();

   ////////////////////////////////////////////////////////////
   /// \brief Calculate the trajectory legs
   ///
   /// This function is called prior to calling CalculateTrajectory()
   /// only if the node configuration has changed (or has been
   /// newly created). The nodes are transcribed into a vector of
   /// TrajectoryLegs which precompute additional information
   /// required for calculating the trajectory.
   ///
   ////////////////////////////////////////////////////////////
   void CalculateLegs();

   ////////////////////////////////////////////////////////////
   /// \brief Calculate the trajectory
   ///
   /// This function iteratively calls the CalculateTrajectoryLeg
   /// function for each leg.
   ///
   /// \param states Vector of states used throughout the trajectory
   /// \param [out] deltaVs Vector of computed deltaVs required throughout the trajectory
   ///
   ////////////////////////////////////////////////////////////
   void CalculateTrajectory(const std::vector<double>& states, std::vector<double>& deltaVs);

   ////////////////////////////////////////////////////////////
   /// \brief Calculate a trajectory leg
   ///
   /// This function is the primary workhorse of the MGADSMTrajectory
   /// class and uses the TrajectoryNodes (transcribed to TrajectoryLegs)
   /// and the state vector to compute the deltaVs required to
   /// achieve the specified leg of the trajectory.
   ///
   /// \param iLeg Index of leg
   /// \param iState Index of first state vector variable for this leg
   /// \param initialEpoch Epoch at the beginning of the leg
   /// \param initialStateVector StateVector at the beginning of the leg
   /// \param states Vector of states used throughout the trajectory
   /// \param [out] finalEpoch Epoch at the end of the leg
   /// \param [out] finalStateVector StateVector at the end of the leg
   /// \param [out] deltaVs Vector of computed deltaVs required throughout the trajectory
   ///
   ////////////////////////////////////////////////////////////
   void CalculateTrajectoryLeg(int iLeg, int& iState, const Epoch& initialEpoch, const StateVector& initialStateVector, const std::vector<double>& states, Epoch& finalEpoch, StateVector& finalStateVector, std::vector<double>& deltaVs);

   ////////////////////////////////////////////////////////////
   /// \brief Add a state to the internal state vector
   ///
   /// This function is called from the CalcuateLegs() function
   /// as the TrajectoryNodes are iterated through.
   ///
   /// \param state State to add to the state vector
   ///
   ////////////////////////////////////////////////////////////
   void AddState(double state);

   ////////////////////////////////////////////////////////////
   /// \brief Get the index of the n'th DSMNode
   ///
   /// \returns Index of the n'th DSMNode
   ///
   ////////////////////////////////////////////////////////////
   int GetDSMIndex(int dsm) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the index of the n'th FlybyNode
   ///
   /// \returns Index of the n'th FlybyNode
   ///
   ////////////////////////////////////////////////////////////
   int GetFlybyIndex(int flyby) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the index of the n'th RendezvousNode
   ///
   /// \returns Index of the n'th RendezvousNode
   ///
   ////////////////////////////////////////////////////////////
   int GetRendezvousIndex(int rendezvous) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get the index of the n'th InsertionNode
   ///
   /// \returns Index of the n'th InsertionNode
   ///
   ////////////////////////////////////////////////////////////
   int GetInsertionIndex(int insertion) const;

private: 
   int m_numNodes;                                    ///< Number of TrajectoryNodes
   int m_numStates;                                   ///< Number of states in the state vector

   bool m_legsInitialized;                            ///< True if the node configuration or state vector have not changed since the last call to CalculateLegs()
 
   std::vector<TrajectoryNodePtr> m_nodes;            ///< Vector of TrajectoryNodes
   std::vector<TrajectoryLeg> m_legs;                 ///< Vector of TrajectoryLegs
   std::vector<double> m_states;                      ///< Vector of states
   std::vector<double> m_deltaVs;                     ///< Vector of computed deltaVs

   std::unique_ptr<IPropagator> m_propagator;         ///< Propagation algorithm smart pointer
   std::unique_ptr<ILambertAlgorithm> m_lambert;      ///< Lambert algorithm smart pointer
   std::unique_ptr<IFlybyAlgorithm> m_flyby;          ///< Flyby algorithm smart pointer

   // Temporary variables used in CalculateTrajectory()
   Epoch m_initialEpoch;                              ///< Initial epoch of the active TrajectoryLeg. Used in CalculateTrajectory()
   Epoch m_finalEpoch;                                ///< Final epoch of the active TrajectoryLeg. Used in CalculateTrajectory()
   StateVector m_initialStateVector;                  ///< Initial state vector of spacecraft during active TrajectoryLeg. Used in CalculateTrajectory()
   StateVector m_finalStateVector;                    ///< Final state vector of spacecraft during active TrajectoryLeg. Used in CalculateTrajectory()
   StateVector m_planetStateVector;                   ///< State vector of the next orbital body of the active Trajectory Leg. Used in CalculateTrajectory()
};

////////////////////////////////////////////////////////////
/// \class otl::keplerian::trajectory::MGADSMTrajectory
/// \ingroup trajectory
///
/// Multiple Gravity Assist with Deep Space Maneuvers (MGADSM) trajectory.
///
/// A MGADSM trajectory consists of an itinerary and a state vector.
/// The itinerary defines the points of interest (nodes) along the trajectory
/// and the order in which they will be encountered (e.g. Earth -> DSM -> Mars)
/// The state vector contains the specific details of the trajectory
/// such as the departure date, time of flight inbetween the nodes,
/// flyby altitudes, etc.
/// 
/// For example, say that we want to travel from Earth to Mars
/// with a Venus flyby inbetween and a Deep Space Maneuver (DSM)
/// between Venus and Mars. This is the trajectory itinerary and
/// is declared in OTL as follows:
///
/// \code
/// MGADSMTrajectory trajectory;
/// trajectory.AddDeparture("Earth");
/// trajectory.AddFlyby("Venus");
/// trajectory.AddDSM();
/// trajectory.AddRendezvous("Mars");
/// \endcode
///
/// Now, in order to calculate spacecraft's position and
/// velocity throughout this trajectory, the state vector
/// must be defined. This can be done as follows:
///
/// \code
/// std::vector<double> states;
/// states.push_back(3867.51);  // Departure date (mjd2000)
/// states.push_back(117.17);   // Time of flight from Earth to Venus (days)
/// states.push_back(3331.84);  // Altitude of Venus flyby (km)
/// states.push_back(-1.62453); // B-inclination angle of Venus flyby hyperbola (rad)
/// states.push_back(0.35435);  // Fractional time between Venus and Mars at which the DMS occurs
/// states.push_back(690.286);  // Time of flight from Venus and Mars (days)
/// trajectory.SetStateVector(states);
/// \endcode
///
/// Now we have all of the information required to calculate
/// the trajectory. What we are usually interested in is the
/// total change in velocity (deltaV) required to achieve the
/// trajectory. The total deltaV is the sum of each individual
/// deltaV required throughout the trajectory. Calling the
/// Evaluate() function returns the vector of deltaVs:
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
/// trajectory is defined. The function signatures are helpful
/// in determining which states are required by each node
/// type and and which order they must be defined.
///
////////////////////////////////////////////////////////////

} // namespace trajectory

} // namespace keplerian

} // namespace otl