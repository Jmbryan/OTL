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

#include <OTL/Core/MGADSM.hpp>
#include <OTL/Core/PropagateAnalytical.hpp>
#include <OTL/Core/LambertExponentialSinusoid.hpp>
#include <OTL/Core/FlybyIzzo.hpp>
//#include <OTL/Core/KeplersEquations.hpp>

namespace otl
{

MGADSM::MGADSM()
{
}

MGADSM::MGADSM(const std::vector<TrajectoryNode*>& nodes)
{
   Init(nodes);

   m_propagator = std::unique_ptr<IPropagateAlgorithm>(new PropagateAnalytical());
   m_lambert    = std::unique_ptr<ILambertAlgorithm>(new ExponentialSinusoidLambert());
   m_flyby      = std::unique_ptr<IFlybyAlgorithm>(new FlybyIzzo());
}

void MGADSM::Init(const std::vector<TrajectoryNode*>& nodes)
{
   m_numNodes = nodes.size();
   m_numStates = 0;

   TrajectoryLeg leg;

   for (auto i = 0; i < m_numNodes; ++i)
   {
      // Departure
      if (nodes[i]->type == TrajectoryNode::Type::Departure)
      {
         auto depNode = dynamic_cast<DepartureNode*>(nodes[i]);
         OTL_ASSERT(depNode);

         leg.departure = true;
         AddState(depNode->julianDateMin, depNode->julianDateMax);

         // Followed by a DSM
         if (nodes[i+1]->type == TrajectoryNode::Type::DSM)
         {
            AddStates(depNode->departureVectorMin, depNode->departureVectorMax);
         }

         m_itinerary.push_back(Planet(depNode->planetId));
      }

      // Flyby
      else if (nodes[i]->type == TrajectoryNode::Type::Flyby)
      {
         auto flyNode = dynamic_cast<FlybyNode*>(nodes[i]);
         OTL_ASSERT(flyNode);

         leg.flyby = true;
         leg.timeofFlightIndex = m_numStates;
         AddState(flyNode->timeOfFlightMin, flyNode->timeOfFlightMax);
         AddState(flyNode->altitudeMin, flyNode->altitudeMax);
         AddState(flyNode->bInclinationMin, flyNode->bInclinationMax);

         m_itinerary.push_back(Planet(flyNode->planetId));
      }

      // DSM
      else if (nodes[i]->type == TrajectoryNode::Type::DSM)
      {
         auto dsmNode = dynamic_cast<DSMNode*>(nodes[i]);
         OTL_ASSERT(dsmNode);

         leg.numDSM++;
         AddState(dsmNode->alphaMin, dsmNode->alphaMax);

         // Only for more than one DSM
         if (leg.numDSM > 1)
         {
            AddStates(dsmNode->vectorMin, dsmNode->vectorMax);
         }
      }

      // Rendezvous
      else if (nodes[i]->type == TrajectoryNode::Type::Rendezvous)
      {
         auto renNode = dynamic_cast<RendezvousNode*>(nodes[i]);
         OTL_ASSERT(renNode);

         leg.rendezvous = true;
         leg.timeofFlightIndex = m_numStates;
         AddState(renNode->timeOfFlightMin, renNode->timeOfFlightMax);

         m_itinerary.push_back(Planet(renNode->planetId));
      }

      // Insertion
      else if (nodes[i]->type == TrajectoryNode::Type::Insertion)
      {
         auto insNode = dynamic_cast<InsertionNode*>(nodes[i]);
         OTL_ASSERT(insNode);

         leg.insertion = true;
         leg.insertionOrbit = insNode->orbit;
         leg.timeofFlightIndex = m_numStates;
         AddState(insNode->timeOfFlightMin, insNode->timeOfFlightMax);
         AddState(insNode->orbitTimeMin, insNode->orbitTimeMax);

         m_itinerary.push_back(Planet(insNode->planetId));
      }

      // Finished a leg, save it and start a new one
      if (nodes[i]->type == TrajectoryNode::Type::Flyby ||
          nodes[i]->type == TrajectoryNode::Type::Rendezvous || 
          nodes[i]->type == TrajectoryNode::Type::Insertion)
      {
         m_legs.push_back(leg);
         leg = TrajectoryLeg();
      }
   }
}

void MGADSM::CalculateTrajectory(const std::vector<double>& states, std::vector<double>& deltaVs)
{
    int iState = 0;
    int iPlanet = 0;

    // Trajectory initial conditions
    m_finalEpoch = states[iState++];
    m_itinerary[iPlanet++].GetStateVectorsAtEpoch(m_finalEpoch, m_finalStateVector);

   // Calculate the trajectory one leg at a time
   for (auto i = 0; i < m_legs.size(); ++i)
   {
      const TrajectoryLeg& leg = m_legs[i];

      // Leg destination
      Planet& planet = m_itinerary[iPlanet++];

      // Leg initial conditions
      m_initialEpoch = m_finalEpoch;
      m_initialStateVector = m_finalStateVector;

      // Leg flight time
      double timeOfFlightLeg = states[leg.timeofFlightIndex];
      double timeOfFlightRemaining = timeOfFlightLeg;

      // Leg final conditions
      m_finalEpoch = m_initialEpoch + timeOfFlightLeg;
      planet.GetStateVectorsAtEpoch(m_finalEpoch, m_planetStateVector);
      m_finalStateVector = m_planetStateVector;

      // Handle departure event
      if (leg.departure)
      {
         // Followed by a DSM
         if (leg.numDSM > 0)
         {
            double vinf  = states[iState++];
            double theta = states[iState++];
            double phi = states[iState++];

            Vector3d deltaV = ConvertSpherical2Cartesian(vinf, theta, phi);
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
               double theta = states[iState++];
               double phi = states[iState++];

               Vector3d deltaV = ConvertSpherical2Cartesian(vinf, theta, phi);
               m_initialStateVector.velocity += deltaV;

               deltaVs.push_back(deltaV.Magnitude());
            }

            m_propagator->Propagate(m_initialStateVector, ASTRO_MU_SUN, timeOfFlight);
         }      
      }

      // Handle Rendezvous event (Lambert)
      {
          Vector3d preLambertVelocity = m_initialStateVector.velocity;

          m_lambert->Evaluate(m_initialStateVector.position,
                              m_finalStateVector.position,
                              timeOfFlightRemaining,
                              Orbit::Direction::Prograde,
                              0,
                              ASTRO_MU_SUN,
                              m_initialStateVector.velocity,
                              m_finalStateVector.velocity);

          deltaVs.push_back((m_initialStateVector.velocity - preLambertVelocity).Magnitude());

          if (!leg.flyby && !leg.insertion)
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
      }
   }
}

int MGADSM::GetNumStates() const
{
    return m_numStates;
}

void MGADSM::AddState(double lowerBound, double upperBound)
{
   m_lowerBounds.push_back(lowerBound);
   m_upperBounds.push_back(upperBound);
   m_numStates += 1;
}

void MGADSM::AddStates(const Vector3d& lowerBounds, const Vector3d& upperBounds)
{
   AddState(lowerBounds.x, upperBounds.x);
   AddState(lowerBounds.y, upperBounds.y);
   AddState(lowerBounds.z, upperBounds.z);
}

} // namespace otl