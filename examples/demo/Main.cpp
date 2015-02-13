#include <iostream>
#include <assert.h>
#include <chrono>

#include <OTL/Core/MGADSMTrajectory.h>
#include <OTL/Core/Planet.h>
#include <OTL/Core/JplApproximateBody.h>
#include <OTL/Core/SpiceBody.h>
#include <OTL/Core/MpcorbBody.h>
#include <OTL/Core/Orbit.h>

#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/LagrangianPropagator.h>
#include <OTL/Core/Conversion.h>

#include <OTL/Core/LambertExponentialSinusoid.h>
#include <OTL/Core/JplApproximateEphemeris.h>
#include <OTL/Core/JplEphemeris.h>
#include <OTL/Core/MpcorbEphemeris.h>
#include <OTL/Core/SpiceEphemeris.h>

//#include <OTL/Core/Matrix.h>
//#include <OTL/Core/Mat.h>

#include <OTL/Core/System.h>
#include <OTL/Core/Logger.h>
#include <spdlog\spdlog.h>

#include <numeric>

#ifdef GetCurrentDirectory
#undef GetCurrentDirectory
#endif

//#include <Eigen/Dense>

using namespace otl;

int main()
{
    gLogger.SetLogLevel(LogLevel::Info);

    // Sizes
    if (true)
    {
       // Base
       auto sizeofbool = sizeof(bool);
       auto sizeofenum = sizeof(StateVectorType);
       auto sizeofint = sizeof(int);
       auto sizeoffloat = sizeof(float);
       auto sizeofdouble = sizeof(double);

       auto sizeofcoes = sizeof(OrbitalElements);
       auto sizeofsv = sizeof(StateVector);

       // StateVector       
       auto sizeofvector6d = sizeof(Vector6d);
       auto sizeofos = sizeof(test::StateVector);

       // Orbit
       auto sizeofprop = sizeof(PropagatorPointer);
       auto sizeoftime = sizeof(Time);
       auto sizeoforbit = sizeof(keplerian::Orbit);

       // OrbitalBody
       auto sizeofname = sizeof(std::string);
       auto sizeofphys = sizeof(PhysicalProperties);
       auto sizeofepoch = sizeof(Epoch);     
       auto sizeoforbitalbody = sizeof(OrbitalBody);

       // Planets
       auto sizeofplanet = sizeof(Planet);
       auto sizeofmpcorb = sizeof(MpcorbBody);
       auto sizeofspice = sizeof(SpiceBody);

       double d = 1.0;
    }

    if (false)
    {
       test::StateVector stateVector;
       stateVector = StateVector(
          Vector3d(6524.834, 6862.875, 6448.296),
          Vector3d(4.901327, 5.533756, -1.976341));
       double mu = otl::ASTRO_MU_EARTH;

       //OrbitalBody o("Satellite", PhysicalProperties(1000, 10), mu, stateVector);
       Planet o("Earth");

       //o.SetPropagator(std::make_shared<LagrangianPropagator>());

       auto sv1 = o.GetCartesianStateVector();
       auto coes1 = o.GetOrbitalElements();
       auto sv2 = o.GetCartesianStateVector();
       auto coes2 = o.GetOrbitalElements();
       o.Propagate(Time::Days(10));
       auto sv3 = o.GetCartesianStateVector();
       auto coes3 = o.GetOrbitalElements();
       auto sv4 = o.GetCartesianStateVector();
       auto coes4 = o.GetOrbitalElements();

       double d = 1.0;
    }

    // Eccentricity test
    if (false)
    {
       test::StateVector stateVector;
       stateVector = StateVector(
          Vector3d(6524.834, 6862.875, 6448.296),
          Vector3d(4.901327, 5.533756, -1.976341));
       double mu = otl::ASTRO_MU_EARTH;

       int numIter = 100000;

       const auto& cartesianStateVector = stateVector.GetCartesianStateVector();
       double alpha;
       double e;

       std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          
          double r = cartesianStateVector.position.norm();
          double v = cartesianStateVector.velocity.norm();
          alpha = 2.0 / r - SQR(v) / mu; // Reciprocal of semi-major axis
       }
       std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

       std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          const auto& R = cartesianStateVector.position;
          const auto& V = cartesianStateVector.velocity;
          double r = R.norm();
          double v = V.norm();
          e = ((SQR(v) / mu - 1.0 / r) * R - (R.dot(V) / mu) * V).norm();
       }
       std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();

       auto duration1 = t2 - t1;
       auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);

       auto duration2 = t4 - t3;
       auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);

       double d = 1.0;
    }

    while (true)
    {
       static int counter = 0;
       counter++;
       test::StateVector* psv = new test::StateVector();

       auto currentDirectory = gSystem.GetCurrentDirectory();
       auto dataFile = currentDirectory + "\\..\\..\\..\\data\\jpl\\de405\\de405.data";
       auto jplEphemeris = std::make_shared<JplEphemeris>(dataFile);

       auto kernalFile = currentDirectory + "\\..\\..\\..\\data\\spice\\de430.bsp";
       auto spiceEphemeris = std::make_shared<SpiceEphemeris>(kernalFile);
       spiceEphemeris->Initialize();
       spiceEphemeris->LoadDataFile(currentDirectory + "\\..\\..\\..\\data\\spice\\gm_de431.tpc");
       spiceEphemeris->LoadDataFile(currentDirectory + "\\..\\..\\..\\data\\spice\\pck00010.tpc");

       auto mpcorbDataFile = currentDirectory + "\\..\\..\\..\\data\\mpcorb\\mpcorb.data";
       auto mpcorbEphemeris = std::make_shared<MpcorbEphemeris>(mpcorbDataFile);

       //MpcorbBody p("Ceres", mpcorbEphemeris);

       SpiceBody p("Earth", spiceEphemeris);
       p.SetPropagator(std::make_shared<LagrangianPropagator>());

       //Planet p("Earth");
       //Planet p(ConvertPlanetIdentifier2Name(PlanetId::Earth));
       //p.SetEphemeris(jplEphemeris);
       //p.SetPropagator(std::make_shared<LagrangianPropagator>());

       auto prop00 = p.GetPhysicalProperties();
       auto mu00 = p.GetGravitationalParameterCentralBody();
       auto sv00 = p.GetStateVector();

       int numIter = 10000;
       auto epoch = Epoch::MJD2000(10.0);
       std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          auto sv = p.QueryStateVector(epoch);
       }
       std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

       p.QueryStateVector(Epoch::MJD2000(0.0));
       std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          p.PropagateTo(epoch);
          auto sv = p.GetStateVector();
       }
       std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();

       auto duration1 = t2 - t1;
       auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);

       auto duration2 = t4 - t3;
       auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);

       double dd = 1.0;

       //int numKernals = spiceEphemeris->GetNumKernalsLoaded();
       //double muc = spiceEphemeris->GetBodyProperty("Sun", "GM");
       //auto props = spiceEphemeris->GetBodyProperties("Sun", "RADII");

       //OrbitalElements coes0(1000, 0.1, 3.14, 0, 0, 0), coesf;
       //StateVector sv0(1, 2, 3, 4, 5, 6), svf;
       //otl::test::StateVector stateVector;

       //stateVector = coes0;
       //auto type1 = stateVector.GetType();
       ////orbitState = sv0;
       //auto type2 = stateVector.GetType();

       //auto gen = stateVector.GetGenericStateVector();
       //switch (stateVector.GetType())
       //{
       //case StateVectorType::Orbital:
       //   coesf = stateVector.GetOrbitalElements();
       //   break;

       //case StateVectorType::Cartesian:
       //   svf = stateVector.GetCartesianStateVector();
       //   break;
       //}
     
       //auto coesc = stateVector.ToOrbitalElements(ASTRO_MU_EARTH);
       //auto svc = stateVector.ToCartesianStateVector(ASTRO_MU_EARTH);

       //auto prop = p.GetPhysicalProperties();
       //auto cbmu = p.GetGravitationalParameterCentralBody();
       //auto coes = p.GetOrbit().GetOrbitalElements();
       //auto sv = p.GetOrbit().GetStateVector();

       //bool b = true;
       //auto en = keplerian::Orbit::Type::Elliptical;
       //std::function<void(const Epoch&)> myfun = nullptr;

       //

       //p.SetMaxPropagationTime(Time::Days(20));

       //p.QueryStateVector(p.GetEpoch() + Time::Days(10));
       //p.Propagate(Time::Days(10));
       //p.Propagate(Time::Days(5));
       //p.Propagate(Time::Days(10));
       //auto coes2 = p.GetOrbit().GetOrbitalElements();
       //auto sv2 = p.GetOrbit().GetStateVector();

       //

       //double d = 1.0;

       //auto mpcorbEphemeris = std::make_shared<MpcorbEphemeris>();

       //auto ceres = OrbitalBody2("Ceres", ASTRO_MU_SUN);

       //MpcorbPlanet ceres2("Ceres");


       //OrbitalBody2 ceres("Ceres", mpcorbEphemeris, Epoch::Today());
       //ceres.QueryEphemeris(Time::Days(10));
       //auto svPallas = ceres.GetStateVector();

       //auto properties = ceres.GetPhysicalProperties();
       //auto sv = ceres.GetStateVector();

       //ceres.Propagate(Time::Days(10)); // calls IPropagator
       //sv = ceres.GetStateVector();

    }

#define IsNANorINF(x) ((x * 0) != 0)
    
    GregorianDateTime date;
    date.day = 15;
    date.month = 2;
    date.year = 2015;
    date.hour = 13;
    date.min = 27;
    date.sec = 3.456;
    Epoch epoch = Epoch::Gregorian(date);
    std::cout << epoch << std::endl;
    std::cout << epoch.ToDetailedString() << std::endl;

    Time time = Time::Aggregrate(Time::AggregrateTime(0, 10, 2, 30, 15.45));
    //Time time = Time::Seconds(0);
    std::cout << time << std::endl;
    std::cout << time.ToDetailedString() << std::endl;

    OrbitalElements coes{ 10000.0, 0.8, 30.0 * MATH_DEG_TO_RAD, 5.0 * MATH_DEG_TO_RAD, 15 * MATH_DEG_TO_RAD, 135.0 * MATH_DEG_TO_RAD };
    std::cout << coes << std::endl;
    std::cout << coes.ToDetailedString() << std::endl;

    StateVector sv{ -6045, -3490, 2500, -3.457, 6.618, 2.533};
    std::cout << sv << std::endl;
    std::cout << sv.ToDetailedString() << std::endl;

    keplerian::Orbit orbit(ASTRO_MU_EARTH, test::StateVector(sv));
    //orbit.UseStateVectorForStringOutput(true);
    std::cout << orbit << std::endl;
    std::cout << orbit.ToDetailedString() << std::endl;

    //OrbitalBody orbitalBody("Ceres", 895.8e18);
    //std::cout << orbitalBody << std::endl;
    //std::cout << orbitalBody.ToDetailedString() << std::endl;

    //NaturalBody naturalBody("Ceres", 895.8e18, 476.2, epoch);
    //std::cout << naturalBody << std::endl;
    //std::cout << naturalBody.ToDetailedString() << std::endl;

    Planet planet("Earth", Epoch::Gregorian(GregorianDateTime(2015, 1, 28, 20, 58, 0.0)));
    std::cout << "Planet: " << Bracket(planet) << std::endl;
    std::cout << "Planet:" << std::endl << planet.ToDetailedString("   ") << std::endl;

    auto dayOfWeek0 = CalculateDayOfWeek(GregorianDateTime(1582, 10, 4));  // Monday
    auto dayOfWeek1 = CalculateDayOfWeek(GregorianDateTime(1600, 1, 1));   // Saturday
    auto dayOfWeek2 = CalculateDayOfWeek(GregorianDateTime(1732, 6, 18));  // Wednesday
    auto dayOfWeek3 = CalculateDayOfWeek(GregorianDateTime(1958, 11, 27)); // Thursday
    auto dayOfWeek4 = CalculateDayOfWeek(GregorianDateTime(2000, 1, 1));   // Saturday
    auto dayOfWeek5 = CalculateDayOfWeek(GregorianDateTime(2015, 1, 25));  // Sunday
    auto dayOfWeek6 = CalculateDayOfWeek(GregorianDateTime(2283, 3, 13));  // Tuesday
 
    if (true)
      OTL_INFO_IF(false, "I'm " << "number " << 10 << "!");
    else
      OTL_INFO_IF(false, "I'm " << "number " << 1 << "!");

    if (false)
      OTL_WARN_IF(false, "My fav number is " << 3.14);
    else
      OTL_WARN_IF(false, "My fav number is " << 37);


    OTL_ASSERT(true);
    //OTL_ASSERT(1 > 2);
    OTL_ASSERT(1 < 2, "The world is " << "right");
    //OTL_ASSERT(1 > 2, "The world is " << "wrong");

    if (false)
    {
       GregorianDateTime startDate;
       startDate.day = 1;
       startDate.month = 1;
       startDate.year = 2010;
       startDate.hour = startDate.min = 0;
       startDate.sec = 0.0;
       GregorianDateTime endDate = startDate;
       endDate.year = 2020;

       auto currentDirectory = gSystem.GetCurrentDirectory();
       auto dataDirectory = currentDirectory + "\\..\\..\\..\\data\\jpl\\de405";
       auto outputFilename = currentDirectory + "\\..\\..\\..\\data\\jpl\\de405\\de405_test.data";

       JplEphemeris jplEphemeris("");
       jplEphemeris.SetDataDirectory(dataDirectory);
       jplEphemeris.CreateEphemerisFile(Epoch::Gregorian(startDate), Epoch::Gregorian(endDate), outputFilename);
       jplEphemeris.SetDataFile(outputFilename);


       GregorianDateTime testDate = startDate;
       testDate.year = static_cast<int>(0.5 * (startDate.year + endDate.year));
       StateVector stateVector;
       jplEphemeris.GetStateVector("Earth", Epoch::Gregorian(testDate), stateVector);

       double d = 1.0;
    }

    if (false)
    {
       int numIter = 10000;

       auto propagator = std::make_shared<keplerian::KeplerianPropagator>();

       StateVector stateVector, finalStateVector1, finalStateVector2, finalStateVector3, finalStateVector4;
       stateVector.position = otl::Vector3d(1131.340, -2282.343, 6672.423);            // [km]
       stateVector.velocity = otl::Vector3d(-5.64305, 4.30333, 2.42879);               // [km/s]
       double mu = otl::ASTRO_MU_EARTH;                                                // [km^3/s^2]
       Time timeDelta = otl::Time::Minutes(40.0);                                      // [s]
       //Time timeDelta = otl::Time::Hours(1.0);

       //double r0 = stateVector.position.GetNorm();
       //double v0 = stateVector.velocity.GetNorm();
       //double rdotv = stateVector.position.Dot(stateVector.velocity);

       OrbitalElements orbitalElements;
       //orbitalElements.semiMajorAxis = -19655;
       //orbitalElements.eccentricity = 1.4682;
       //orbitalElements.trueAnomaly = 30.0 * MATH_DEG_TO_RAD;
       orbitalElements = ConvertCartesianStateVector2OrbitalElements(stateVector, mu);

       OrbitalElements finalOrbitalElements1, finalOrbitalElements2;

       // Propagate (COES)
       std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          propagator->PropagateK(orbitalElements, mu, timeDelta, finalOrbitalElements1);
          finalStateVector1 = ConvertOrbitalElements2CartesianStateVector(finalOrbitalElements1, mu);
       }
       std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

       // Propagate (SV)
       std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          propagator->PropagateK(stateVector, mu, timeDelta, finalStateVector2);
       }
       std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();

       // PropagateX (COES)
       std::chrono::system_clock::time_point t5 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          finalOrbitalElements2 = propagator->Propagate(orbitalElements, timeDelta, mu);
          finalStateVector3 = ConvertOrbitalElements2CartesianStateVector(finalOrbitalElements2, mu);
       }
       std::chrono::system_clock::time_point t6 = std::chrono::system_clock::now();

       // PropagateX (SV)
       std::chrono::system_clock::time_point t7 = std::chrono::system_clock::now();
       for (int i = 0; i < numIter; ++i)
       {
          finalStateVector4 = propagator->Propagate(stateVector, timeDelta, mu);
       }
       std::chrono::system_clock::time_point t8 = std::chrono::system_clock::now();

       auto duration1 = t2 - t1;
       auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);

       auto duration2 = t4 - t3;
       auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);

       auto duration3 = t6 - t5;
       auto milli3 = std::chrono::duration_cast<std::chrono::milliseconds>(duration3);

       auto duration4 = t8 - t7;
       auto milli4 = std::chrono::duration_cast<std::chrono::milliseconds>(duration4);

       bool b1 = finalOrbitalElements1 == finalOrbitalElements2;
       bool b2 = finalStateVector1.position == finalStateVector2.position;
       bool b3 = finalStateVector1.velocity == finalStateVector2.velocity;
       double d = 1.0;
    }
       

    if (true)
    {
       Vector3d v123(1.0, 2.0, 3.0);
       //Matrix3d v6(1.0, 2.0, 3.0);

       Eigen::Vector3d eigenV3(1.0, 2.0, 3.0);
       const auto& x = eigenV3.x();
       auto y = eigenV3.y();
       auto& z = eigenV3.z();

       eigenV3.x() = 4.0;
       eigenV3.y() = 5.0;
       eigenV3.z() = 6.0;

       y = 8.0;
       z = 9.0;

       double d = 1.0;

       //if (true)
       //{
       //   int numIter = 1000;

       //   Eigen::Vector3d ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8, ev9, ev10;
       //   Eigen::Matrix3d mv1;
       //   std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       //   for (int i = 0; i < numIter; ++i)
       //   {
       //      auto xprod = ev1.cross(ev2).cross(ev3).cross(ev4).cross(ev5).cross(ev6).cross(ev7).cross(ev8).cross(ev9).cross(ev10);
       //      auto row0 = mv1.row(0);
       //      auto dot = ev1.dot(ev2);
       //      auto temp = mv1.row(0).dot(ev2);
       //      auto z = ev1.cross(ev2.cross(mv1.row(0))).dot(ev4);
       //   }
       //   std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

       //   Vec3d v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;
       //   Mat3d m1;
       //   std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       //   for (int i = 0; i < numIter; ++i)
       //   {
       //      auto xprod = v1.Cross(v2).Cross(v3).Cross(v4).Cross(v5).Cross(v6).Cross(v7).Cross(v8).Cross(v9).Cross(v10);
       //      auto row0 = m1.Row(0);
       //      auto dot = v1.Dot(v2);
       //      auto temp = m1.Row(0).Dot(v2);
       //      auto z = v1.Cross(v2.Cross(m1.Row(0))).Dot(v4);
       //   }
       //   std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();

       //   auto duration1 = t2 - t1;
       //   auto seconds1 = std::chrono::duration_cast<std::chrono::seconds>(duration1);
       //   auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);
       //   auto micro1 = std::chrono::duration_cast<std::chrono::microseconds>(duration1);
       //   auto nano1 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration1);

       //   auto duration2 = t4 - t3;
       //   auto seconds2 = std::chrono::duration_cast<std::chrono::seconds>(duration2);
       //   auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);
       //   auto micro2 = std::chrono::duration_cast<std::chrono::microseconds>(duration2);
       //   auto nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration2);

       //   OTL_INFO() << "Eigen: " << milli1.count() << " ms. OTL: " << milli2.count() << "ms";
       //   double d = 1.0;
       //}

       //if (true)
       //{
       //   Eigen::Matrix3d em31;
       //   Eigen::Matrix3d em32;
       //   Eigen::Matrix3d em33;
       //   double ed = (em31 * em32).norm();
       //   //em31.cross(em32).cross(em33);

       //   Matrix3d om31;
       //   Matrix3d om32;
       //   double od = Matrix3d(om31 * om32).GetNorm();
       //}

       //auto cm1 = Matrix3d::Constant(10.0);
       //auto element0 = cm1(0);

       //Eigen::Matrix3d em3dd;
       //Eigen::Vector3d ev3dd;
       //auto erow0 = em3dd.row(0);
       ////auto xxx = em3dd[0];
       //auto yyy = ev3dd[0];

       //Eigen::Matrix3d eigenTemp3;
       //eigenTemp3.fill(3.0);
       //auto lshdfsd = eigenTemp3 * 3;
       //Matrix3d otlTemp3;
       //otlTemp3 = eigenTemp3;

       //Matrix3d m3d;
       //Matrix3d m3d2;
       //Matrixd md;
       //Matrixd md2;
       //Vector3d v3d;
       //Vector3d v3d2;

       //RowVector3d orow0 = m3d.Row(0);
       //auto ocol0 = m3d.Col(0);
       //Vector3d ocol1 = m3d.Col(2);
       ////auto oxxx = m3d[0];
       //auto oyyy = v3d[0];

       //m3d.Fill(3.0);
       //m3d2.Fill(2.0);
       //v3d.Fill(3.0);
       //v3d2.Fill(2.0);

       //Matrix3d subResult = m3d - m3d2;
       //auto addResult = m3d + m3d2;
       //auto multResult = m3d * m3d2;

       //m3d += m3d2;
       //m3d -= m3d2;
       //m3d *= m3d2;

       //auto multScalarResult1 = m3d * 10.0;
       //auto multScalarResult2 = 10.0 * m3d;
       //auto divScalarResult = m3d / 10.0;

       //auto isEqual = m3d == m3d2;
       //auto isNotEqual = m3d != m3d2;

       //auto norm = m3d.GetNorm();
       //auto squaredNorm = m3d.GetSquaredNorm();
       //m3d.TransposeInPlace();
       //m3d.NormalizeInPlace();
       //auto m3dT = m3d.Transpose();
       //auto dot = v3d.Dot(v3d2);
       //auto cross = v3d.Cross(v3d2);

       //Eigen::Vector3d ev3d;
       //auto cross2 = v3d.Cross(ev3d);

       //auto zeroMatrix6 = Matrix6d::Zero();
       //auto eyeMatrix6 = Matrix6d::Identity();

       //int numIter = 1000;

       //Eigen::MatrixXf ea(50, 50), eb(50, 50), ec(50, 50), ed(50, 50);
       //ea.fill(2.0); eb.fill(2.0); ec.fill(2.0); ed.fill(2.0);     
       //std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
       //for (int i = 0; i < numIter; ++i)
       //{
       //   ea *= eb;
       //   ea += ec;
       //   ea -= ed;
       //   ea *= 10.0;
       //   ea /= 5.0;
       //   ea = 3 * eb + 4 * (ec + ed) - (eb * ed) / 5 + ec.transpose() * 6 * (eb.transpose() - ed);
       //}     
       //std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();     
       //auto duration1 = t2 - t1;
       //auto seconds1 = std::chrono::duration_cast<std::chrono::seconds>(duration1);
       //auto milli1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration1);
       //auto micro1 = std::chrono::duration_cast<std::chrono::microseconds>(duration1);
       //auto nano1 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration1);

       //Matrixf oa(50, 50), ob(50, 50), oc(50, 50), od(50, 50);
       //oa.Fill(2.0); ob.Fill(2.0); oc.Fill(2.0); od.Fill(2.0);
       //std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();
       //for (int i = 0; i < numIter; ++i)
       //{
       //   oa *= ob;
       //   oa += oc;
       //   oa -= od;
       //   oa *= 10.0;
       //   oa /= 5.0;
       //   oa = 3 * ob + 4 * (oc + od) - (ob * od) / 5 + oc.Transpose() * 6 * (ob.Transpose() - od);
       //}    
       //std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();
       //auto duration2 = t4 - t3;
       //auto seconds2 = std::chrono::duration_cast<std::chrono::seconds>(duration2);
       //auto milli2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);
       //auto micro2 = std::chrono::duration_cast<std::chrono::microseconds>(duration2);
       //auto nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(duration2);
       //
       //OTL_INFO() << "Eigen: " << milli1.count() << " ms. OTL: " << milli2.count() << "ms";
       //double d = 1.0;
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

       auto approxDataFile = currentDirectory + "\\..\\..\\..\\data\\jpl\\approx\\approx3000_3000.data";
       auto jplApproxEphemeris = std::make_shared<JplApproximateEphemeris>(approxDataFile);
       jplApproxEphemeris->GetStateVector(planetName, epoch, stateVector1);
       jplApproxEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements1);

       auto dataFile = currentDirectory + "\\..\\..\\..\\data\\jpl\\de405\\de405.data";
       auto jplEphemeris = std::make_shared<JplEphemeris>(dataFile);
       jplEphemeris->GetStateVector(planetName, epoch, stateVector2);
       jplEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements2);

#if defined(OTL_SPICE)
       auto kernalFile = currentDirectory + "\\..\\..\\..\\data\\spice\\de430.bsp";
       auto spiceEphemeris = std::make_shared<SpiceEphemeris>(kernalFile);
       spiceEphemeris->GetStateVector(planetName, epoch, stateVector3);
       spiceEphemeris->GetOrbitalElements(planetName, epoch, orbitalElements3);
#endif

       //auto mpcorbDataFile = currentDirectory + "\\..\\..\\..\\data\\mpcorb\\mpcorb.data";
       //auto mpcorbEphemeris = std::make_shared<MpcorbEphemeris>(mpcorbDataFile);
       //mpcorbEphemeris->GetStateVector("Ceres", epoch, stateVector4);
       //mpcorbEphemeris->GetOrbitalElements("Ceres", epoch, orbitalElements4);
          
       //Planet p("Earth", Epoch::Gregorian(date));
       //p.SetEphemeris(jplEphemeris);
       //auto sv = p.GetOrbit().GetStateVector();
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

std::cout << "Initial velocity (kms/s): [" << initialVelocity.x() << ", "
                                           << initialVelocity.y() << ", "
                                           << initialVelocity.z() << "]" << std::endl;
std::cout << "Final velocity (kms/s): [" << finalVelocity.x() << ", "
                                         << finalVelocity.y() << ", "
                                         << finalVelocity.z() << "]" << std::endl;

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