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
#include <OTL/Core/Orbit.hpp>
//#include <OTL/Core/KeplersEquations.hpp>

namespace otl
{

MGADSM::MGADSM()
{
}

MGADSM::MGADSM(const std::vector<TrajectoryNode*>& nodes)
{
   Init(nodes);
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
   StateVector initialStateVector;
   StateVector finalStateVector;

   Epoch initialEpoch;
   Epoch finalEpoch;

   int iPlanet = 0;
   int iState = 0;
   int iDeltaV = 0;
   for (auto i = 0; i < m_legs.size(); ++i)
   {
      const TrajectoryLeg& leg = m_legs[i];

      Planet& initialPlanet = m_itinerary[iPlanet];
      Planet& finalPlanet   = m_itinerary[iPlanet + 1];
      iPlanet++;

      // Handle departure event
      if (leg.departure)
      {
         // Initial departure
         if (i == 0)
         {
            initialEpoch = states[iState++];
            initialPlanet.GetStateVectorsAtEpoch(initialEpoch, initialStateVector);
         }
         // Intermediate departure
         else
         {
            initialStateVector = finalStateVector;
         }

         // Followed by a DSM
         if (leg.numDSM > 0)
         {
            double vinf  = states[iState++];
            double delta = states[iState++];
            double theta = states[iState++];

            Vector3d oldVelocity = initialStateVector.velocity;
            Vector3d deltaV; //= ConvertSpherical2Cartesian(vinf, delta, theta);
            initialStateVector.velocity = oldVelocity + deltaV;

            deltaVs.push_back(deltaV.Magnitude());
         }
      }

      double timeOfFlightLeg = states[leg.timeofFlightIndex];
      double timeOfFlightRemaining = timeOfFlightLeg;

      // Handle DSM event(s)
      if (leg.numDSM > 0)
      {
         Orbit orbit(initialStateVector, ASTRO_MU_SUN);
         
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

               Vector3d deltaV; //= ConvertSpherical2Cartesian(vinf, delta, theta);
               initialStateVector.velocity += deltaV;

               deltaVs.push_back(deltaV.Magnitude());
            }

            orbit.SetStateVector(initialStateVector);
            orbit.Propagate(timeOfFlight);

            initialStateVector = orbit.GetStateVector();
         }      
      }

      Vector3d finalPlanetVelocity = finalStateVector.velocity;


      // Handle Rendezvous event
      if (leg.rendezvous || leg.flyby || leg.insertion)
      {
         //finalEpoch = initialEpoch + timeOfFlightLeg;
         finalPlanet.GetStateVectorsAtEpoch(finalEpoch, finalStateVector);

         Vector3d oldInitialVelocity = initialStateVector.velocity;
         
         m_lambert->Evaluate(initialStateVector.position,
                             finalStateVector.position,
                             timeOfFlightRemaining,
                             Orbit::Direction::Propgrade,
                             0,
                             initialStateVector.velocity,
                             finalStateVector.velocity);

         deltaVs.push_back((initialStateVector.velocity - oldInitialVelocity).Magnitude());
         deltaVs.push_back((finalStateVector.velocity - finalPlanetVelocity).Magnitude());
      }

      // Handle flyby event
      if (leg.flyby)
      {
         double altitude = states[iState++];
         double bInclinationAngle = states[iState++];

         Vector3d approachVelocity = finalStateVector.velocity;

         m_flyby->Evaluate(approachVelocity,
                           finalPlanet,
                           altitude,
                           bInclinationAngle,
                           finalStateVector.velocity);

      }

      // Handle Insertion event
      if (leg.insertion)
      {
         double orbitTime = states[iState++];

         // Orbit insertion
         double mu = finalPlanet.GetMu();
         double h  = 0;//
         double e  = leg.insertionOrbit.eccentricity;
         double rp = h * h / mu / (1 + e);
         double vinf = (finalStateVector.velocity - finalPlanetVelocity).Magnitude();
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