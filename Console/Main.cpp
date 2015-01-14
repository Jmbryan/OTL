#include <iostream>
#include <assert.h>
#include <chrono>

#include <OTL/Core/MGADSMTrajectory.h>
#include <OTL/Core/Planet.h>
#include <OTL/Core/Orbit.h>

#include <OTL/Core/LambertExponentialSinusoid.h>
#include <OTL/Core/JplApproximateEphemeris.h>
#include <OTL/Core/JplEphemeris.h>
#include <OTL/Core/MpcorbEphemeris.h>
#include <OTL/Core/SpiceEphemeris.h>

#include <OTL/Core/Matrix.h>

#include <OTL/Core/System.h>
#include <OTL/Core/Logger.h>

#include <numeric>

//#include <Eigen/Dense>

using namespace otl;

int main()
{
    gLogger.SetLogLevel(LogLevel::Fatal);
    gLogger.SetThrowLevel(LogLevel::Error);

    if (true)
    {
       Vector3d v123(1.0, 2.0, 3.0);
       //Matrix3d v6(1.0, 2.0, 3.0);

       auto cm1 = Matrix3d::Constant(10.0);
       auto element0 = cm1(0);

       Eigen::Matrix3d em3dd;
       Eigen::Vector3d ev3dd;
       auto erow0 = em3dd.row(0);
       //auto xxx = em3dd[0];
       auto yyy = ev3dd[0];

       Eigen::Matrix3d eigenTemp3;
       eigenTemp3.fill(3.0);
       auto lshdfsd = eigenTemp3 * 3;
       Matrix3d otlTemp3;
       otlTemp3 = eigenTemp3;

       Matrix3d m3d;
       Matrix3d m3d2;
       Matrixd md;
       Matrixd md2;
       Vector3d v3d;
       Vector3d v3d2;

       RowVector3d orow0 = m3d.Row(0);
       auto ocol0 = m3d.Col(0);
       Vector3d ocol1 = m3d.Col(2);
       //auto oxxx = m3d[0];
       auto oyyy = v3d[0];

       m3d.Fill(3.0);
       m3d2.Fill(2.0);
       v3d.Fill(3.0);
       v3d2.Fill(2.0);

       Matrix3d subResult = m3d - m3d2;
       auto addResult = m3d + m3d2;
       auto multResult = m3d * m3d2;

       m3d += m3d2;
       m3d -= m3d2;
       m3d *= m3d2;

       auto multScalarResult1 = m3d * 10.0;
       auto multScalarResult2 = 10.0 * m3d;
       auto divScalarResult = m3d / 10.0;

       auto isEqual = m3d == m3d2;
       auto isNotEqual = m3d != m3d2;

       auto norm = m3d.GetNorm();
       auto squaredNorm = m3d.GetSquaredNorm();
       m3d.TransposeInPlace();
       m3d.NormalizeInPlace();
       auto m3dT = m3d.Transpose();
       auto dot = v3d.Dot(v3d2);
       auto cross = v3d.Cross(v3d2);

       Eigen::Vector3d ev3d;
       auto cross2 = v3d.Cross(ev3d);

       auto zeroMatrix6 = Matrix6d::Zero();
       auto eyeMatrix6 = Matrix6d::Identity();

       int numIter = 1000;

       Eigen::MatrixXf ea(50, 50), eb(50, 50), ec(50, 50), ed(50, 50);
       ea.fill(2.0); eb.fill(2.0); ec.fill(2.0); ed.fill(2.0);     
       std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          ea *= eb;
          ea += ec;
          ea -= ed;
          ea *= 10.0;
          ea /= 5.0;
          ea = 3 * eb + 4 * (ec + ed) - (eb * ed) / 5 + ec.transpose() * 6 * (eb.transpose() - ed);
       }     
       std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();     
       auto duration1 = t2 - t1;
       auto seconds1 = std::chrono::duration_cast<std::chrono::seconds>(duration1);
       auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);
       auto micro1 = std::chrono::duration_cast<std::chrono::microseconds>(duration1);
       auto nano1 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration1);

       Matrixf oa(50, 50), ob(50, 50), oc(50, 50), od(50, 50);
       oa.Fill(2.0); ob.Fill(2.0); oc.Fill(2.0); od.Fill(2.0);
       std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          oa *= ob;
          oa += oc;
          oa -= od;
          oa *= 10.0;
          oa /= 5.0;
          oa = 3 * ob + 4 * (oc + od) - (ob * od) / 5 + oc.Transpose() * 6 * (ob.Transpose() - od);
       }    
       std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();
       auto duration2 = t4 - t3;
       auto seconds2 = std::chrono::duration_cast<std::chrono::seconds>(duration2);
       auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);
       auto micro2 = std::chrono::duration_cast<std::chrono::microseconds>(duration2);
       auto nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration2);
       
       OTL_INFO() << "Eigen: " << milli1.count() << " ms. OTL: " << milli2.count() << "ms";
       double d = 1.0;
    }

    if (false)
    {
       //Time currentTime = gSystem.GetCurrentTime();
       //_sleep(1000);
       //Time currentTime2 = gSystem.GetCurrentTime();

       OTL_LOG("This is a log message!", LogLevel::Info);
       OTL_INFO() << "Hello.";
       try
       {
          OTL_WARN() << "This is" << " my warning!" << 1;
          OTL_ERROR() << "This " << 1 << " is " << 2.5f << " my " << 3.55 << " error!!";
       }
       catch (std::runtime_error re)
       {
          std::cout << "Runtime Error caught: " << re.what() << std::endl;
       }

       try
       {
          OTL_INFO() << "Hello.";
          OTL_WARN() << "This is" << " a warning!";
          //OTL_ERROR() << "This is " << "an error!!";
          OTL_FATAL() << "This is a fatal error!!!";
       }
       catch (std::runtime_error re)
       {
          std::cout << "Runtime Error caught: " << re.what() << std::endl;
       }

       //Planet q("Quantix");
       try
       {
          Planet p("Krypton");
       }
       catch (std::runtime_error re)
       {
          std::cout << "Runtime Error caught: " << re.what() << std::endl;
       }
    }

    if (true)
    {
       GregorianDateTime date;
       date.day = 1;
       date.month = 1;
       date.year = 2015;
       date.hour = date.min = 0;
       date.sec = 0.0;
       Epoch epoch = Epoch::Gregorian(date);

       Vector3d position;
       Vector3d velocity;
       StateVector stateVector1, stateVector2, stateVector3, stateVector4;
       OrbitalElements orbitalElements1, orbitalElements2, orbitalElements3, orbitalElements4;

       auto planetName = "Earth";

       auto currentDirectory = gSystem.GetCurrentDirectory();

       auto approxDataFile = currentDirectory + "\\..\\data\\jpl\\approx\\approx3000_3000.data";
       auto jplApproxEphemeris = std::make_shared<JplApproximateEphemeris>(approxDataFile);
       jplApproxEphemeris->GetStateVector(planetName, epoch, stateVector1);
       jplApproxEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements1);

       auto dataFile = currentDirectory + "\\..\\data\\jpl\\de405\\de405.data";
       auto jplEphemeris = std::make_shared<JplEphemeris>(dataFile);
       jplEphemeris->GetStateVector(planetName, epoch, stateVector2);
       jplEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements2);

       auto kernalFile = currentDirectory + "\\..\\data\\spice\\de430.bsp";
       auto spiceEphemeris = std::make_shared<SpiceEphemeris>(kernalFile);
       spiceEphemeris->GetStateVector(planetName, epoch, stateVector3);
       spiceEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements3);

       auto mpcorbDataFile = currentDirectory + "\\..\\data\\mpcorb\\mpcorb.data";
       auto mpcorbEphemeris = std::make_shared<MpcorbEphemeris>(mpcorbDataFile);
       mpcorbEphemeris->GetStateVector("Ceres", epoch, stateVector4);
       mpcorbEphemeris->GetOrbitalElements("Ceres", epoch, orbitalElements4);
          
       Planet p("Earth");
       p.SetEphemeris(jplEphemeris);
       p.SetEpoch(Epoch::Gregorian(date));
       auto sv = p.GetStateVector();
    }

    double d = 1.0;

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

   {
otl::keplerian::LambertExponentialSinusoid lambert;

//Setup the inputs
otl::Vector3d initialPosition({ 15945.34, 0.0, 0.0 });
otl::Vector3d finalPosition({ 12214.83899, 10249.46731, 0.0 });
otl::Time timeDelta = Time::Minutes(76.0);
otl::keplerian::Orbit::Direction orbitDirection = otl::keplerian::Orbit::Direction::Prograde;
int maxRevolutions = 0;
double mu = otl::ASTRO_MU_EARTH;

// Setup the outputs
otl::Vector3d initialVelocity;
otl::Vector3d finalVelocity;

// Evaluate Lambert's Problem
lambert.Evaluate(initialPosition,
                 finalPosition,
                 timeDelta,
                 orbitDirection,
                 maxRevolutions,
                 mu,
                 initialVelocity,
                 finalVelocity);

std::cout << "Initial velocity (kms/s): [" << initialVelocity.X() << ", "
                                           << initialVelocity.Y() << ", "
                                           << initialVelocity.Z() << "]" << std::endl;
std::cout << "Final velocity (kms/s): [" << finalVelocity.X() << ", "
                                         << finalVelocity.Y() << ", "
                                         << finalVelocity.Z() << "]" << std::endl;

   }


   otl::Epoch myEpoch = Epoch::JD(2449877.3458762);
   GregorianDateTime dateTime = myEpoch.GetGregorian();
   double jd = myEpoch.GetJD();

   otl::Epoch myEpoch1 = Epoch::Gregorian(dateTime);
   double jd1 = myEpoch1.GetJD();

   GregorianDateTime dateTime2;
   dateTime2.year = 1996;
   dateTime2.month = 10;
   dateTime2.day = 26;
   dateTime2.hour = 14;
   dateTime2.min = 20;
   dateTime2.sec = 0.0;
   otl::Epoch myEpoch2 = Epoch::Gregorian(dateTime2);
   double jd2 = myEpoch2.GetJD();

   double mjd2000 = otl::ConvertGregorian2MJD2000(myEpoch.GetGregorian());

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
       keplerian::trajectory::MGADSMTrajectory trajectory;
       trajectory.AddDeparture("Earth", Epoch::MJD2000(3867.51));
       trajectory.AddFlyby("Venus", 117.17, 3331.84, -1.62453);
       trajectory.AddDSM(0.35435);
       trajectory.AddRendezvous("Mars", 690.286);
       std::vector<double> deltaVs = trajectory.Evaluate();

       //trajectory.SetDSM(1, 0.45435);
    }
    
    // Postponed
    {
       keplerian::trajectory::MGADSMTrajectory trajectory;
       trajectory.AddDeparture("Earth");
       trajectory.AddFlyby("Venus");
       trajectory.AddDSM();
       trajectory.AddRendezvous("Mars");
       //std::vector<double> states;
       //std::vector<double> deltaVs = trajectory.Evaluate(states);

       //double totalDeltaV = std::accumulate(deltaVs.begin(), deltaVs.end(), 0);
    }

    //otl::keplerian::trajectory::DepartureNode depNode("Earth", myEpoch, Vector3d(0.0, 0.0, 1.0));
    
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