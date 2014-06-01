#include <iostream>
#include <assert.h>
#include <chrono>

#include <OTL/Core/MGADSMTrajectory.hpp>
#include <OTL/Core/Planet.hpp>
#include <OTL/Core/Orbit.hpp>

#include <numeric>

using namespace otl;

int main()
{
    //DE405Ephemeris* ephem = nullptr;

    //ephem = new DE405Ephemeris("E:/Dev/OTL/data/jpl_eph/de405/de405.data");

    //double pos[3];
    //double vel[3];

    //ephem->getPosition(2451536.5, DE405Ephemeris::Mercury, pos);

    //// Truth values from JPL Horizons System
    //assert( std::fabs( -4.468523069712503E+07 - pos[0] ) < 1E-3 );
    //assert( std::fabs( -4.796771237574679E+07 - pos[1] ) < 1E-3 );
    //assert( std::fabs( -2.102675128947175E+07 - pos[2] ) < 1E-3 );

    //Orbit orbit(ASTRO_MU_SUN);

    //Planet p(Planet::PlanetId::Mercury);
    //double jd = 2451536.5;
    //Epoch epoch(jd, Epoch::Type::JD);
    //StateVector sv;

    //std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();

    //for (int i = 0; i < 100000; ++i)
    //{
    //    p.GetStateVectorsAtEpoch(epoch, sv);
    //}
    //
    //std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

    //auto duration = t2 - t1;
    //auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    //auto milli   = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    //DepartureNode<double> depNode;
    //depNode.planetId = Planet::PlanetId::Earth;
    //depNode.julianDate = 1000;
    //depNode.julianDateMin = 1000;
    //depNode.julianDateMax = 1100;
    //depNode.departureVectorMin = Vector3d(0.0, 0.0, 0.0);
    //depNode.departureVectorMax = Vector3d(5.0, 1.0, 1.0);

    //DSMNode<double> dsmNode1;
    //dsmNode1.alphaMin = 0.01;
    //dsmNode1.alphaMax = 0.99;
    //
    //FlybyNode<double> flyNode;
    //flyNode.planetId = Planet::PlanetId::Venus;
    //flyNode.timeOfFlightMin = 50;
    //flyNode.timeOfFlightMax = 300;
    //flyNode.altitudeMin = 1000;
    //flyNode.altitudeMax = 10000;
    //flyNode.bInclinationMin = -MATH_PI;
    //flyNode.bInclinationMax =  MATH_PI;

    //DSMNode<double> dsmNode2;
    //dsmNode2.alphaMin = 0.01;
    //dsmNode2.alphaMax = 0.99;

    //RendezvousNode<double> renNode;
    //renNode.planetId = Planet::PlanetId::Mercury;
    //renNode.timeOfFlightMin = 50;
    //renNode.timeOfFlightMax = 300;

    //std::vector<TrajectoryNode<double>*> nodes;
    //nodes.push_back(&depNode);
    //nodes.push_back(&dsmNode1);
    //nodes.push_back(&flyNode);
    //nodes.push_back(&dsmNode2);
    //nodes.push_back(&renNode);

    //MGADSM mgadsm(nodes);
    //int numStates = mgadsm.GetNumStates();

    std::vector<double> x;
    x.push_back(1050);
    x.push_back(3.0);
    x.push_back(0.5);
    x.push_back(0.5);
    x.push_back(0.4); // alpha
    x.push_back(200.0);
    x.push_back(500.0);
    x.push_back(0.5 * MATH_PI);
    x.push_back(0.6); // alpha
    x.push_back(200.0);

    std::vector<double> f;

    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();

    for (int i = 0; i < 1; ++i)
    {
        //mgadsm.CalculateTrajectory(x, f);
    }

    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

    auto duration = t2 - t1;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto milli   = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    auto micro   = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    auto nano    = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    // Direct
    {
       keplerian::MGADSMTrajectory trajectory;
       trajectory.AddDeparture("Earth", 3867.51);
       trajectory.AddFlyby("Venus", 117.17, 3331.84, -1.62453);
       trajectory.AddDSM(0.35435);
       trajectory.AddRendezvous("Mars", 690.286);
       std::vector<double> deltaVs = trajectory.Evaluate();

       //trajectory.SetDSM(1, 0.45435);
    }
    
    // Postponed
    {
       keplerian::MGADSMTrajectory trajectory;
       trajectory.AddDeparture("Earth");
       trajectory.AddFlyby("Venus");
       trajectory.AddDSM();
       trajectory.AddRendezvous("Mars");
       //std::vector<double> states;
       //std::vector<double> deltaVs = trajectory.Evaluate(states);

       //double totalDeltaV = std::accumulate(deltaVs.begin(), deltaVs.end(), 0);
    }
    
    //MGADSMProblem prob;
    //prob.AddDeparture(


    //struct FlybyNode2
    //{
    //    std::string name;
    //    double timeOfFlight;
    //    double altitude;
    //    double bInclinationAngle;
    //};

    //trajectory.SetFlyby(1, "Venus", 200.0, 3331.84, -1.62453);
    //int flyIndex = trajectory.GetFlybyIndex(1);
    //FlybyNode2 flybyNode = trajectory.GetFlyby(1);
    //flybyNode.altitude = 4000.0;
    //trajectory.SetFlyby(1, flybyNode);

    //MGADSMItinerary itinerary;
    //itinerary.AddDeparture("Earth");
    //itinerary.AddFlyby("Venus");
    //itinerary.AddDSM();
    //itinerary.AddRendezvous("Mars");
    //MGADSM trajectory2(itinerary);

    //MGADSMTrajectory trajectory2;
    //trajectory2.AddDeparture("Earth");
    //trajectory2.AddFlyby("Venus");
    //trajectory2.AddDSM();
    //trajectory2.AddRendezvous("Mars");
    //std::vector<double> states;
    //deltaVs = trajectory.Evaluate(states);

    // JDs     = [2455412.511, 2455529.6813, 2456219.967, 2456219.967] days
    // deltaVs = [2.8049, 1.43987, 3.78799] km/s

    return 0;
}