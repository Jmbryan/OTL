#include <OTL/Test/BaseTest.h>
#include <OTL/Core/LambertExponentialSinusoid.h>

TEST_CASE("ExponentialSinusoidLambert", "Lambert")
{
   auto lambert = otl::keplerian::LambertExponentialSinusoid();

    otl::Vector3d initialPosition, finalPosition;
    otl::Vector3d initialVelocity, finalVelocity;
    otl::Time timeOfFlight = otl::Time::Days(1);
    double mu;

    int maxRevolutions = 0;
    otl::keplerian::Orbit::Direction direction = otl::keplerian::Orbit::Direction::Prograde;

    SECTION("Evaluate")
    {
        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 7-5.
        SECTION("Truth Case: Vallado 7-5")
        {
           initialPosition = otl::Vector3d({ 15945.34, 0.0, 0.0 });          // [km]
           finalPosition = otl::Vector3d({ 12214.83899, 10249.46731, 0.0 }); // [km]
            timeOfFlight = otl::Time::Minutes(76.0);                      // [s]
            mu = otl::ASTRO_MU_EARTH;

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             mu,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(2.058913));  // [km/s]
            CHECK(initialVelocity.y() == OTL_APPROX(2.915965));  // [km/s]
            CHECK(initialVelocity.z() == OTL_APPROX(0.0));       // [km/s]
            CHECK(finalVelocity.x()   == OTL_APPROX(-3.451565)); // [km/s]
            CHECK(finalVelocity.y()  == OTL_APPROX(0.910315));  // [km/s]
            CHECK(finalVelocity.z()   == OTL_APPROX(0.0));       // [km/s]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 7-5.
        SECTION("Truth Case: Vallado 7-5 (canonical)")
        {
           initialPosition = otl::Vector3d({ 2.5, 0.0, 0.0 });         // [ER]
           finalPosition = otl::Vector3d({ 1.915111, 1.606969, 0.0 }); // [ER]
            timeOfFlight = otl::Time::Seconds(5.6519);              // [TU]
            mu = 1.0;

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             1.0,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(0.2604450));  // [ER/TU]
            CHECK(initialVelocity.y() == OTL_APPROX(0.3688589));  // [ER/TU]
            CHECK(initialVelocity.z() == OTL_APPROX(0.0));        // [ER/TU]
            CHECK(finalVelocity.x()   == OTL_APPROX(-0.4366104)); // [ER/TU]
            CHECK(finalVelocity.y()  == OTL_APPROX(0.1151515));  // [ER/TU]
            CHECK(finalVelocity.z()   == OTL_APPROX(0.0));        // [ER/TU]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 5.2.
        SECTION("Truth Case: Curtis 5.2")
        {
           initialPosition = otl::Vector3d({ 5000.0, 10000.0, 2100.0 }); // [km]
           finalPosition = otl::Vector3d({ -14600.0, 2500.0, 7000.0 });  // [km]
            timeOfFlight = otl::Time::Hours(1.0);                     // [s]
            mu = 398600.0;                                            // [km^3/s^2]

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             mu,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(-5.9925));  // [km/s]
            CHECK(initialVelocity.y() == OTL_APPROX(1.9254));   // [km/s]
            CHECK(initialVelocity.z() == OTL_APPROX(3.2456));   // [km/s]
            CHECK(finalVelocity.x()   == OTL_APPROX(-3.3125));  // [km/s]
            CHECK(finalVelocity.y()  == OTL_APPROX(-4.1966));  // [km/s]
            CHECK(finalVelocity.z()   == OTL_APPROX(-0.38529)); // [km/s]
        }
    
        /// Test ExponentialSinusoidLambert.Evaluate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 5.3.
        SECTION("Truth Case: Curtis 5.3")
        {
           initialPosition = otl::Vector3d({ 273378.0, 0.0, 0.0 });   // [km]
           finalPosition = otl::Vector3d({ 145820.0, 12758.0, 0.0 }); // [km]
            timeOfFlight = otl::Time::Hours(13.5);                 // [s]
            mu = 398600.0;                                         // [km^3/s^2]

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             mu,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(-2.4356)); // [km/s]
            CHECK(initialVelocity.y() == OTL_APPROX(0.26741)); // [km/s]
            CHECK(initialVelocity.z() == OTL_APPROX(0.0));     // [km/s]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics 1st Edition, Bate Mueller & White, Example 5.3.1.
        SECTION("Truth Case: BMW 5.3.1 (short way)")
        {
           initialPosition = otl::Vector3d({ 0.5, 0.6, 0.7 }); // [DU]
           finalPosition = otl::Vector3d({ 0.0, 1.0, 0.0 });   // [DU]
            timeOfFlight = otl::Time::Seconds(0.9667663);   // [TU]
            mu = 1.0;
            direction = otl::keplerian::Orbit::Direction::Prograde;

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             mu,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(-0.361677496)); // [VU]
            CHECK(initialVelocity.y() == OTL_APPROX(0.76973587));   // [VU]
            CHECK(initialVelocity.z() == OTL_APPROX(-0.50634848));  // [VU]
            CHECK(finalVelocity.x()   == OTL_APPROX(-0.60187442));  // [VU]
            CHECK(finalVelocity.y()  == OTL_APPROX(-0.02234181));  // [VU]
            CHECK(finalVelocity.z()   == OTL_APPROX(-0.84262419));  // [VU]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics 1st Edition, Bate Mueller & White, Example 5.3.1.
        SECTION("Truth Case: BMW 5.3.1 (long way)")
        {
           initialPosition = otl::Vector3d({ 0.5, 0.6, 0.7 }); // [DU]
           finalPosition = otl::Vector3d({ 0.0, 1.0, 0.0 });   // [DU]
            timeOfFlight = otl::Time::Seconds(0.9667663);   // [TU]
            mu = 1.0;
            direction = otl::keplerian::Orbit::Direction::Retrograde;

            lambert.Evaluate(initialPosition,
                             finalPosition,
                             timeOfFlight,
                             direction,
                             maxRevolutions,
                             mu,
                             initialVelocity,
                             finalVelocity);

            CHECK(initialVelocity.x() == OTL_APPROX(-0.6304918096));  // [VU]
            CHECK(initialVelocity.y() == OTL_APPROX(-1.11392096659)); // [VU]
            CHECK(initialVelocity.z() == OTL_APPROX(-0.8826885334));  // [VU] Assuming sign typo
            CHECK(finalVelocity.x()   == OTL_APPROX(0.1786653974));   // [VU]
            CHECK(finalVelocity.y()  == OTL_APPROX(1.5544139777));   // [VU]
            CHECK(finalVelocity.z()   == OTL_APPROX(0.250135563));    // [VU]
        }
    }
}