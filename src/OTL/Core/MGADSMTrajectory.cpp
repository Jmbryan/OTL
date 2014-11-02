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

#include <OTL/Core/MGADSMTrajectory.hpp>
#include <OTL/Core/PropagateLagrangian.hpp>
#include <OTL/Core/LambertExponentialSinusoid.hpp>
#include <OTL/Core/FlybyUnpowered.hpp>
//#include <OTL/Core/KeplersEquations.hpp>

namespace otl
{

namespace keplerian
{

namespace trajectory
{

///////////////////////////////////////////////////////////////////////////////////
TrajectoryLeg::TrajectoryLeg() :
initialPlanet(),
finalPlanet(),
departure(false),
numDSM(0),
flyby(false),
rendezvous(false),
insertion(false)
{
}

///////////////////////////////////////////////////////////////////////////////////
MGADSMTrajectory::MGADSMTrajectory() :
m_numNodes(0),
m_numStates(0),
m_legsInitialized(false),
m_initialEpoch(Epoch()),
m_finalEpoch(Epoch()),
m_initialStateVector(StateVector()),
m_finalStateVector(StateVector()),
m_planetStateVector(StateVector())
{
   Init();
}

///////////////////////////////////////////////////////////////////////////////////
MGADSMTrajectory::MGADSMTrajectory(const std::vector<TrajectoryNodePtr>& nodes) :
m_numNodes(0),
m_numStates(0),
m_legsInitialized(false),
m_initialEpoch(Epoch()),
m_finalEpoch(Epoch()),
m_initialStateVector(StateVector()),
m_finalStateVector(StateVector()),
m_planetStateVector(StateVector())
{
   Init();
   SetNodes(0, nodes);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::Evaluate(const std::vector<double>& states, std::vector<double>& deltaVs)
{
   OTL_ASSERT(states.size() == m_numStates, "Invalid state vector. Dimension mismatch");
   CalculateTrajectory(states, deltaVs);
}

///////////////////////////////////////////////////////////////////////////////////
const std::vector<double>& MGADSMTrajectory::Evaluate(const std::vector<double>& states)
{
   OTL_ASSERT(states.size() == m_numStates, "Invalid state vector. Dimension mismatch");
   Evaluate(states, m_deltaVs);
   return m_deltaVs;
}

///////////////////////////////////////////////////////////////////////////////////
const std::vector<double>& MGADSMTrajectory::Evaluate()
{
   Evaluate(m_states, m_deltaVs);
   return m_deltaVs;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddDeparture(const DepartureNode& departureNode)
{
   AddNode(TrajectoryNodePtr(new DepartureNode(departureNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddDSM(const DSMNode& dsmNode)
{
   AddNode(TrajectoryNodePtr(new DSMNode(dsmNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddFlyby(const FlybyNode& flybyNode)
{
   AddNode(TrajectoryNodePtr(new FlybyNode(flybyNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddRendezvous(const RendezvousNode& rendezvousNode)
{
   AddNode(TrajectoryNodePtr(new RendezvousNode(rendezvousNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddInsertion(const InsertionNode& insertionNode)
{
   AddNode(TrajectoryNodePtr(new InsertionNode(insertionNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddNode(const TrajectoryNodePtr& node)
{
   OTL_ASSERT(node, "Can't add node. Invalid node.");
   if (m_nodes.empty() && node->GetType() != TrajectoryNode::Type::Departure)
   {
      OTL_ASSERT(false, "Can't add node. The first node must be of type Departure.");
   }
   if (!m_nodes.empty() && node->GetType() == TrajectoryNode::Type::Departure)
   {
      OTL_ASSERT(false, "Can't add node. Only one Departure node allowed.");
   }

   m_nodes.push_back(node);
   m_numNodes = m_nodes.size();

   m_legsInitialized = false;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddNodes(const std::vector<TrajectoryNodePtr>& nodes)
{
   for (std::size_t i = 0; i < nodes.size(); ++i)
   {
      AddNode(nodes[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetDeparture(const DepartureNode& departureNode)
{
   SetNode(0, TrajectoryNodePtr(new DepartureNode(departureNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetDSM(int n, const DSMNode& dsmNode)
{
   int index = GetDSMIndex(n);
   SetNode(index, TrajectoryNodePtr(new DSMNode(dsmNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetFlyby(int n, const FlybyNode& flybyNode)
{
   int index = GetFlybyIndex(n);
   SetNode(index, TrajectoryNodePtr(new FlybyNode(flybyNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetRendezvous(int n, const RendezvousNode& rendezvousNode)
{
   int index = GetRendezvousIndex(n);
   SetNode(index, TrajectoryNodePtr(new RendezvousNode(rendezvousNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetInsertion(int n, const InsertionNode& insertionNode)
{
   int index = GetInsertionIndex(n);
   SetNode(index, TrajectoryNodePtr(new InsertionNode(insertionNode)));
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetNode(int n, const TrajectoryNodePtr& node)
{
   OTL_ASSERT(node, "Can't set node. Invalid node");
   OTL_ASSERT(n >= 0 && n < static_cast<int>(m_nodes.size()), "Can't set node. Index out of bounds.");
   OTL_ASSERT(m_nodes[n]->GetType() == node->GetType(), "Can't set node. Type mismatch.");

   m_nodes[n] = node;

   m_legsInitialized = false;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetNodes(int n, const std::vector<TrajectoryNodePtr>& nodes)
{
   OTL_ASSERT(n >= 0, "Invalid node index");
   for (std::size_t i = 0; i < nodes.size(); ++i)
   {
      SetNode(n + i, nodes[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////////
DepartureNode MGADSMTrajectory::GetDeparture() const
{
   OTL_ASSERT(!m_nodes.empty(), "Can't set Departure node. Node not found.");

   auto depNode = dynamic_cast<DepartureNode*>(m_nodes.front().get());
   OTL_ASSERT(depNode, "Failed to convert node to Departure node.");
   return *depNode;
}

///////////////////////////////////////////////////////////////////////////////////
DSMNode MGADSMTrajectory::GetDSM(int n) const
{
   int nodeIndex = GetDSMIndex(n);
   OTL_ASSERT(nodeIndex >= 0, "Can't get DSM node. Node not found.");

   auto dsmNode = dynamic_cast<DSMNode*>(m_nodes[nodeIndex].get());
   OTL_ASSERT(dsmNode, "Failed to convert node to DSM node.");
   return *dsmNode;
}

///////////////////////////////////////////////////////////////////////////////////
FlybyNode MGADSMTrajectory::GetFlyby(int n) const
{
   int nodeIndex = GetFlybyIndex(n);
   OTL_ASSERT(nodeIndex >= 0, "Can't get Flyby node. Node not found.");

   auto flyNode = dynamic_cast<FlybyNode*>(m_nodes[nodeIndex].get());
   OTL_ASSERT(flyNode, "Failed to convert node to Flyby node.");
   return *flyNode;
}

///////////////////////////////////////////////////////////////////////////////////
RendezvousNode MGADSMTrajectory::GetRendezvous(int n) const
{
   int nodeIndex = GetRendezvousIndex(n);
   OTL_ASSERT(nodeIndex >= 0, "Can't get Rendezvous node. Node not found.");

   auto renNode = dynamic_cast<RendezvousNode*>(m_nodes[nodeIndex].get());
   OTL_ASSERT(renNode, "Failed to convert node to Rendezvous node.");
   return *renNode; 
}

///////////////////////////////////////////////////////////////////////////////////
InsertionNode MGADSMTrajectory::GetInsertion(int n) const
{
   int nodeIndex = GetInsertionIndex(n);
   OTL_ASSERT(nodeIndex >= 0, "Can't get Flyby node. Node not found.");

   auto insNode = dynamic_cast<InsertionNode*>(m_nodes[nodeIndex].get());
   OTL_ASSERT(insNode, "Failed to convert node to Insertion node.");
   return *insNode;  
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddDeparture(const std::string& orbitalBody, const Epoch& epoch, const Vector3d& deltaV)
{
   DepartureNode depNode;
   depNode.orbitalBody = orbitalBody;
   depNode.epoch = epoch;
   depNode.deltaV = deltaV;
   AddDeparture(depNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddDSM(double timeOfFlightFraction, const Vector3d& deltaV)
{
   DSMNode dsmNode;
   dsmNode.alpha = timeOfFlightFraction;
   dsmNode.deltaV = deltaV;
   AddDSM(dsmNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddFlyby(const std::string& orbitalBody, double timeOfFlight, double altitude, double bInclinationAngle)
{
   FlybyNode flyNode;
   flyNode.orbitalBody = orbitalBody;
   flyNode.timeOfFlight = timeOfFlight;
   flyNode.altitude = altitude;
   flyNode.bInclinationAngle = bInclinationAngle;
   AddFlyby(flyNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddRendezvous(const std::string& orbitalBody, double timeOfFlight)
{
   RendezvousNode renNode;
   renNode.orbitalBody = orbitalBody;
   renNode.timeOfFlight = timeOfFlight;
   AddRendezvous(renNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddInsertion(const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight, double timeOfOrbit)
{
   InsertionNode insNode;
   insNode.orbitalBody = orbitalBody;
   insNode.orbitalElements = orbitalElements;
   insNode.timeOfFlight = timeOfFlight;
   insNode.timeOfOrbit = timeOfOrbit;
   AddInsertion(insNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetDeparture(const std::string& orbitalBody, const Epoch& epoch, const Vector3d& deltaV)
{
   DepartureNode depNode;
   depNode.orbitalBody = orbitalBody;
   depNode.epoch = epoch;
   depNode.deltaV = deltaV;
   SetDeparture(depNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetDSM(int dsm, double timeOfFlightFraction, const Vector3d& deltaV)
{
   DSMNode dsmNode;
   dsmNode.alpha = timeOfFlightFraction;
   dsmNode.deltaV = deltaV;
   SetDSM(dsm, dsmNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetFlyby(int flyby, const std::string& orbitalBody, double timeOfFlight, double altitude, double bInclinationAngle)
{
   FlybyNode flyNode;
   flyNode.orbitalBody = orbitalBody;
   flyNode.timeOfFlight = timeOfFlight;
   flyNode.altitude = altitude;
   flyNode.bInclinationAngle = bInclinationAngle;
   SetFlyby(flyby, flyNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetRendezvous(int rendezvous, const std::string& orbitalBody, double timeOfFlight)
{
   RendezvousNode renNode;
   renNode.orbitalBody = orbitalBody;
   renNode.timeOfFlight = timeOfFlight;
   SetRendezvous(rendezvous, renNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetInsertion(int insertion, const std::string& orbitalBody, const OrbitalElements& orbitalElements, double timeOfFlight, double timeOfOrbit, const Vector3d& deltaV)
{
   InsertionNode insNode;
   insNode.orbitalBody = orbitalBody;
   insNode.orbitalElements = orbitalElements;
   insNode.timeOfFlight = timeOfFlight;
   insNode.timeOfOrbit = timeOfOrbit;
   insNode.deltaV = deltaV;
   SetInsertion(insertion, insNode);
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetStateVector(const std::vector<double>& states)
{
   if (!m_legsInitialized)
   {
      CalculateLegs();
   }

   if (states.size() != m_states.size())
   {
      OTL_ASSERT(false, "Can't set state vector. Dimension mismatch.");
   }
   else
   {
      // UpdateNodeStates(states); [TODO]
      m_states = states;
   }
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetPropagateType(PropagateType type)
{
   switch (type)
   {
      case PropagateType::Analytical:
         m_propagator = std::unique_ptr<IPropagateAlgorithm>(new PropagateLagrangian());
         break;

      case PropagateType::Invalid:
      default:
         OTL_ASSERT(false, "Can't set Propagate algorithm. Uknown or invalid type.");
         break;
   }
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetLambertType(LambertType type)
{
   switch (type)
   {
      case LambertType::ExponentialSinusoid:
         m_lambert = std::unique_ptr<ILambertAlgorithm>(new LambertExponentialSinusoid());
         break;

      case LambertType::Invalid:
      default:
         OTL_ASSERT(false, "Can't set Lambert algorithm. Uknown or invalid type.");
         break;
   }
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::SetFlybyType(FlybyType type)
{
   switch (type)
   {
      case FlybyType::Izzo:
         m_flyby = std::unique_ptr<IFlybyAlgorithm>(new FlybyUnpowered());
         break;

      case FlybyType::Invalid:
      default:
         OTL_ASSERT(false, "Can't set Flyby algorithm. Uknown or invalid type.");
         break;
   }
}

///////////////////////////////////////////////////////////////////////////////////
std::vector<double> MGADSMTrajectory::GetStateVector() const
{
   return m_states;
}

///////////////////////////////////////////////////////////////////////////////////
std::vector<double> MGADSMTrajectory::GetDeltaVs() const
{
   return m_deltaVs;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetNumNodes() const
{
   return m_numNodes;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetNumStates() const
{
   return m_numStates;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::Init()
{
   m_numNodes = 0;
   m_numStates = 0;

   m_legsInitialized = false;

   // Default algorithms
   m_propagator = std::unique_ptr<IPropagateAlgorithm>(new PropagateLagrangian());
   m_lambert    = std::unique_ptr<ILambertAlgorithm>(new LambertExponentialSinusoid());
   m_flyby      = std::unique_ptr<IFlybyAlgorithm>(new FlybyUnpowered());
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::CalculateLegs()
{
   // Detect a valid node configuration
   if (m_nodes.size() <= 1)
   {
      OTL_ASSERT(false, "Invalid trajectory. Trajectory must consist of at least two nodes.");
   }
   if (m_nodes.front()->GetType() != TrajectoryNode::Type::Departure)
   {
      OTL_ASSERT(false, "Invalid trajectory. Trajectory must start with a Departure node.");
   }
   if (m_nodes.back()->GetType() != TrajectoryNode::Type::Flyby &&
       m_nodes.back()->GetType() != TrajectoryNode::Type::Rendezvous &&
       m_nodes.back()->GetType() != TrajectoryNode::Type::Insertion)
   {
      OTL_ASSERT(false, "Invalid trajectory. Trajectory must end with a Flyby, Rendezvous, or Insertion node.");
   }

   m_numStates = 0;

   m_legs.clear();
   TrajectoryLeg leg;

   for (int i = 0; i < m_numNodes; ++i)
   {
      // Departure
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::Departure)
      {
         auto depNode = dynamic_cast<DepartureNode*>(m_nodes[i].get());
         OTL_ASSERT(depNode, "Failed to convert node to Departure node.");

         leg.departure = true;
         leg.initialPlanet = Planet(depNode->orbitalBody);

         AddState(depNode->epoch.GetMJD2000());

         // Only if followed by a DSM
         if (m_nodes[i+1]->GetType() == TrajectoryNode::Type::DSM)
         {
            AddState(depNode->deltaV.x);
            AddState(depNode->deltaV.y);
            AddState(depNode->deltaV.z);
         }
      }

      // DSM
      else if (m_nodes[i]->GetType() == TrajectoryNode::Type::DSM)
      {
         auto dsmNode = dynamic_cast<DSMNode*>(m_nodes[i].get());
         OTL_ASSERT(dsmNode, "Failed to convert node to DSM node.");

         leg.numDSM++;
         AddState(dsmNode->alpha);

         // Only for more than one DSM
         if (leg.numDSM > 1)
         {
            AddState(dsmNode->deltaV.x);
            AddState(dsmNode->deltaV.y);
            AddState(dsmNode->deltaV.z);
         }
      }

      // Flyby
      else if (m_nodes[i]->GetType() == TrajectoryNode::Type::Flyby)
      {
         auto flyNode = dynamic_cast<FlybyNode*>(m_nodes[i].get());
         OTL_ASSERT(flyNode, "Failed to convert node to Flyby node.");

         leg.flyby = true;
         leg.finalPlanet = Planet(flyNode->orbitalBody);
         leg.timeofFlightIndex = m_numStates;

         AddState(flyNode->timeOfFlight * MATH_DAY_TO_SEC);
         AddState(flyNode->altitude);
         AddState(flyNode->bInclinationAngle);
      }

      // Rendezvous
      else if (m_nodes[i]->GetType() == TrajectoryNode::Type::Rendezvous)
      {
         auto renNode = dynamic_cast<RendezvousNode*>(m_nodes[i].get());
         OTL_ASSERT(renNode, "Failed to convert node to Rendezvous node.");

         leg.rendezvous = true;
         leg.finalPlanet = Planet(renNode->orbitalBody);
         leg.timeofFlightIndex = m_numStates;

         AddState(renNode->timeOfFlight * MATH_DAY_TO_SEC);
      }

      // Insertion
      else if (m_nodes[i]->GetType() == TrajectoryNode::Type::Insertion)
      {
         auto insNode = dynamic_cast<InsertionNode*>(m_nodes[i].get());
         OTL_ASSERT(insNode, "Failed to convert node to Insertion node.");

         leg.insertion = true;
         leg.insertionOrbit = insNode->orbitalElements;
         leg.finalPlanet = Planet(insNode->orbitalBody);
         leg.timeofFlightIndex = m_numStates;

         AddState(insNode->timeOfFlight * MATH_DAY_TO_SEC);
         AddState(insNode->timeOfOrbit * MATH_DAY_TO_SEC);

         if (m_nodes[i+1]->GetType() == TrajectoryNode::Type::DSM)
         {
            AddState(insNode->deltaV.x);
            AddState(insNode->deltaV.y);
            AddState(insNode->deltaV.z);
         }
      }

      // Finished a leg, save it and start a new one
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::Flyby ||
          m_nodes[i]->GetType() == TrajectoryNode::Type::Rendezvous || 
          m_nodes[i]->GetType() == TrajectoryNode::Type::Insertion)
      {
         m_legs.push_back(leg);
         leg = TrajectoryLeg();

         // Set the initial planet of the new leg to the final planet of the previous leg
         leg.initialPlanet = m_legs.back().finalPlanet;
      }
   }

   m_legsInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::CalculateTrajectory(const std::vector<double>& states, std::vector<double>& deltaVs)
{
   if (!m_legsInitialized)
   {
      CalculateLegs();
   }

    int iState = 0;

    // Trajectory initial conditions
    m_finalEpoch.SetMJD2000(states[iState++]);
    m_legs.front().initialPlanet.GetStateVectorAtEpoch(m_finalEpoch, m_finalStateVector);

   // Calculate the trajectory one leg at a time
   for (std::size_t iLeg = 0; iLeg < m_legs.size(); ++iLeg)
   {
      CalculateTrajectoryLeg(iLeg, iState, m_finalEpoch, m_finalStateVector, states, m_finalEpoch, m_finalStateVector, deltaVs);
   }
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::CalculateTrajectoryLeg(int iLeg, int& iState,  const Epoch& initialEpoch, const StateVector& initialStateVector, const std::vector<double>& states, Epoch& finalEpoch, StateVector& finalStateVector, std::vector<double>& deltaVs)
{
   if (!m_legsInitialized)
   {
      CalculateLegs();
   }

   int numLegs = static_cast<int>(m_legs.size());

   const TrajectoryLeg& leg = m_legs[iLeg];

   // Leg destination orbital body
   Planet& planet = const_cast<Planet&>(leg.finalPlanet);

   // Leg initial conditions
   m_initialEpoch = initialEpoch;
   m_initialStateVector = initialStateVector;

   // Leg flight time
   double timeOfFlightLeg = states[leg.timeofFlightIndex];
   double timeOfFlightRemaining = timeOfFlightLeg;

   // Leg final conditions
   m_finalEpoch = m_initialEpoch + Time::Seconds(timeOfFlightLeg);
   planet.GetStateVectorAtEpoch(m_finalEpoch, m_planetStateVector);
   m_finalStateVector = m_planetStateVector;

   // Handle departure event
   if (leg.departure)
   {
      // Followed by a DSM
      if (leg.numDSM > 0)
      {
         double vinf  = states[iState++];
         double delta = states[iState++];
         double theta = states[iState++];

         Vector3d deltaV = ConvertNormalizedSpherical2Cartesian(vinf, delta, theta);
         m_initialStateVector.velocity += deltaV;

         deltaVs.push_back(deltaV.Magnitude());
      }
   }

   // Handle DSM event(s) (Propagate)
   if (leg.numDSM > 0)
   {
      for (int j = 0; j < leg.numDSM; ++j)
      {
         double alpha = states[iState++];
         double timeOfFlight = alpha * timeOfFlightRemaining;
         timeOfFlightRemaining -= timeOfFlight;

         // Additional DSMs after the first
         if (j > 0)
         {
            double vinf = states[iState++];
            double delta = states[iState++];
            double theta = states[iState++];

            Vector3d deltaV = ConvertNormalizedSpherical2Cartesian(vinf, delta, theta);
            m_initialStateVector.velocity += deltaV;

            deltaVs.push_back(deltaV.Magnitude());
         }

         m_propagator->Propagate(m_initialStateVector, ASTRO_MU_SUN, Time::Seconds(timeOfFlight), m_initialStateVector);
      }      
   }

   // Handle Rendezvous event (Lambert)
   {
         Vector3d preLambertVelocity = m_initialStateVector.velocity;

         m_lambert->Evaluate(m_initialStateVector.position,
                           m_finalStateVector.position,
                           Time::Seconds(timeOfFlightRemaining),
                           Orbit::Direction::Prograde,
                           0,
                           ASTRO_MU_SUN,
                           m_initialStateVector.velocity,
                           m_finalStateVector.velocity);

         deltaVs.push_back((m_initialStateVector.velocity - preLambertVelocity).Magnitude());

         // This is the end of the trajectory
         if (!leg.flyby && !leg.insertion && (iLeg == numLegs - 1))
         {
            deltaVs.push_back((m_finalStateVector.velocity - m_planetStateVector.velocity).Magnitude());
         }
   }

   // Handle flyby event (Flyby)
   if (leg.flyby)
   {
      iState++;
      double altitude = states[iState++];
      double bInclinationAngle = states[iState++];

      Vector3d approachVelocity = m_finalStateVector.velocity;

      m_flyby->Evaluate(approachVelocity,
                        planet,
                        altitude,
                        bInclinationAngle,
                        m_finalStateVector.velocity);
   }

   // Handle Insertion event
   if (leg.insertion)
   {
      double orbitTime = states[iState++];

      // Orbit insertion
      double mu = planet.GetMu();
      double h  = 0;//
      double e  = leg.insertionOrbit.eccentricity;
      double rp = h * h / mu / (1 + e);
      double vinf = (m_finalStateVector.velocity -  m_planetStateVector.velocity).Magnitude();
      double vper = sqrt(vinf * vinf + 2.0 * mu / rp);
      double vper2 = sqrt(mu / rp * (1 + e));

      deltaVs.push_back(fabs(vper - vper2));

      // Orbit propagation
      double a = leg.insertionOrbit.semiMajorAxis;
      double T = 2.0 * MATH_PI * pow(a, 1.5) / sqrt(mu) / 24.0 / 3600.0;
      double M2 = 2.0 * MATH_PI * orbitTime / T;
      double E2 = 0.0;//SolveKeplersEquationElliptical(e, M2);
      double TA = 2.0 * atan(sqrt((1.0 + e) / (1.0 - e)) * tan(0.5 * E2));

      double r = h * h / mu / (1.0 + e * cos(TA));

      double v1 = sqrt(mu / r * (1.0 + e));


      // Orbit departure (if followed by a DSM)
      if ((iLeg < numLegs - 1) && m_legs[iLeg + 1].numDSM > 0)
      {
         double vinf  = states[iState++];
         double delta = states[iState++];
         double theta = states[iState++];

         Vector3d deltaV = ConvertNormalizedSpherical2Cartesian(vinf, delta, theta);
         m_finalStateVector.velocity += deltaV;

         deltaVs.push_back(deltaV.Magnitude());
      }
   }

   finalEpoch = m_finalEpoch;
   finalStateVector = m_finalStateVector;
}

///////////////////////////////////////////////////////////////////////////////////
void MGADSMTrajectory::AddState(double state)
{
   m_states.push_back(state);
   m_numStates++;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetDSMIndex(int dsm) const
{
   OTL_ASSERT(dsm >= 0, "Invalid DSM node index");
   int nodeIndex = -1;
   int numDSMs = 0;
   for (std::size_t i = 0; i < m_nodes.size(); ++i)
   {
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::DSM)
      {
         numDSMs++;
      }
      if (dsm == numDSMs)
      {
         nodeIndex = i;
         break;
      }
   }
   return nodeIndex;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetFlybyIndex(int flyby) const
{
   OTL_ASSERT(flyby >= 0, "Invalid Flyby node index");
   int nodeIndex = -1;
   int numFlybys = 0;
   for (std::size_t i = 0; i < m_nodes.size(); ++i)
   {
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::Flyby)
      {
         numFlybys++;
      }
      if (flyby == numFlybys)
      {
         nodeIndex = i;
         break;
      }
   }
   return nodeIndex;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetRendezvousIndex(int rendezvous) const
{
   OTL_ASSERT(rendezvous >= 0, "Invalid Rendezvous node index");
   int nodeIndex = -1;
   int numRendezvous = 0;
   for (std::size_t i = 0; i < m_nodes.size(); ++i)
   {
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::Rendezvous)
      {
         numRendezvous++;
      }
      if (rendezvous == numRendezvous)
      {
         nodeIndex = i;
         break;
      }
   }
   return nodeIndex;
}

///////////////////////////////////////////////////////////////////////////////////
int MGADSMTrajectory::GetInsertionIndex(int insertion) const
{
   OTL_ASSERT(insertion >= 0, "Invalid Insertion node index");
   int nodeIndex = -1;
   int numInsertions = 0;
   for (std::size_t i = 0; i < m_nodes.size(); ++i)
   {
      if (m_nodes[i]->GetType() == TrajectoryNode::Type::Insertion)
      {
         numInsertions++;
      }
      if (insertion == numInsertions)
      {
         nodeIndex = i;
         break;
      }
   }
   return nodeIndex;
}

} // namespace trajectory

} // namespace keplerian

} // namespace otl