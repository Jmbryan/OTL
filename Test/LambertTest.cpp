#include <Catch/catch.hpp>
#include <OTL/Core/LambertExponentialSinusoid.hpp>

const double TOL_PCT = 0.1;
#define OTL_APPROX(x) Approx(x, TOL_PCT)

TEST_CASE("ExponentialSinusoidLambert", "Lambert")
{
    auto lambert = std::unique_ptr<otl::ILambertAlgorithm>(new otl::ExponentialSinusoidLambert());

    otl::Vector3d initialPosition, finalPosition;
    otl::Vector3d initialVelocity, finalVelocity;
    double timeOfFlight;
    double mu;

    int maxRevolutions = 0;
    otl::Orbit::Direction direction = otl::Orbit::Direction::Prograde;

    SECTION("Evaluate")
    {
        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 7-5.
        SECTION("Truth Case: Vallado 7-5")
        {
            initialPosition = otl::Vector3d(15945.34, 0.0, 0.0);          // [km]
            finalPosition = otl::Vector3d(12214.83899, 10249.46731, 0.0); // [km]
            timeOfFlight = 76.0 * 60.0;                                   // [s]
            mu = otl::ASTRO_MU_EARTH;

            lambert->Evaluate(initialPosition,
                                finalPosition,
                                timeOfFlight,
                                direction,
                                maxRevolutions,
                                mu,
                                initialVelocity,
                                finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(2.058913));  // [km/s]
            REQUIRE(initialVelocity.y == OTL_APPROX(2.915965));  // [km/s]
            REQUIRE(initialVelocity.z == OTL_APPROX(0.0));       // [km/s]
            REQUIRE(finalVelocity.x   == OTL_APPROX(-3.451565)); // [km/s]
            REQUIRE(finalVelocity.y   == OTL_APPROX(0.910315));  // [km/s]
            REQUIRE(finalVelocity.z   == OTL_APPROX(0.0));       // [km/s]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 7-5.
        SECTION("Truth Case: Vallado 7-5 (canonical)")
        {
            initialPosition = otl::Vector3d(2.5, 0.0, 0.0);         // [ER]
            finalPosition = otl::Vector3d(1.915111, 1.606969, 0.0); // [ER]
            timeOfFlight = 5.6519;                                  // [TU]
            mu = 1.0;

            lambert->Evaluate(initialPosition,
                            finalPosition,
                            timeOfFlight,
                            direction,
                            maxRevolutions,
                            1.0,
                            initialVelocity,
                            finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(0.2604450));  // [ER/TU]
            REQUIRE(initialVelocity.y == OTL_APPROX(0.3688589));  // [ER/TU]
            REQUIRE(initialVelocity.z == OTL_APPROX(0.0));        // [ER/TU]
            REQUIRE(finalVelocity.x   == OTL_APPROX(-0.4366104)); // [ER/TU]
            REQUIRE(finalVelocity.y   == OTL_APPROX(0.1151515));  // [ER/TU]
            REQUIRE(finalVelocity.z   == OTL_APPROX(0.0));        // [ER/TU]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 5.2.
        SECTION("Truth Case: Curtis 5.2")
        {
            initialPosition = otl::Vector3d(5000.0, 10000.0, 2100.0); // [km]
            finalPosition = otl::Vector3d(-14600.0, 2500.0, 7000.0);  // [km]
            timeOfFlight = 1.0 * 3660.0;                              // [s]
            mu = 398600.0;                                            // [km^3/s^2]

            lambert->Evaluate(initialPosition,
                                finalPosition,
                                timeOfFlight,
                                direction,
                                maxRevolutions,
                                mu,
                                initialVelocity,
                                finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(-5.9925));  // [km/s]
            REQUIRE(initialVelocity.y == OTL_APPROX(1.9254));   // [km/s]
            REQUIRE(initialVelocity.z == OTL_APPROX(3.2456));   // [km/s]
            REQUIRE(finalVelocity.x   == OTL_APPROX(-3.3125));  // [km/s]
            REQUIRE(finalVelocity.y   == OTL_APPROX(-4.1966));  // [km/s]
            REQUIRE(finalVelocity.z   == OTL_APPROX(-0.38529)); // [km/s]
        }
    
        /// Test ExponentialSinusoidLambert.Evaluate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 5.3.
        SECTION("Truth Case: Curtis 5.3")
        {
            initialPosition = otl::Vector3d(273378.0, 0.0, 0.0);   // [km]
            finalPosition = otl::Vector3d(145820.0, 12758.0, 0.0); // [km]
            timeOfFlight = 13.5 * 3660.0;                          // [s]
            mu = 398600.0;                                         // [km^3/s^2]

            lambert->Evaluate(initialPosition,
                                finalPosition,
                                timeOfFlight,
                                direction,
                                maxRevolutions,
                                mu,
                                initialVelocity,
                                finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(-2.4356)); // [km/s]
            REQUIRE(initialVelocity.y == OTL_APPROX(0.26741)); // [km/s]
            REQUIRE(initialVelocity.z == OTL_APPROX(0.0));     // [km/s]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics 1st Edition, Bate Mueller & White, Example 5.3.1.
        SECTION("Truth Case: BMW 5.3.1 (short way)")
        {
            initialPosition = otl::Vector3d(0.5, 0.6, 0.7); // [DU]
            finalPosition = otl::Vector3d(0.0, 1.0, 0.0);   // [DU]
            timeOfFlight = 0.9667663;                       // [TU]
            mu = 1.0;
            direction = otl::Orbit::Direction::Prograde;

            lambert->Evaluate(initialPosition,
                                finalPosition,
                                timeOfFlight,
                                direction,
                                maxRevolutions,
                                mu,
                                initialVelocity,
                                finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(-0.361677496)); // [VU]
            REQUIRE(initialVelocity.y == OTL_APPROX(0.76973587));   // [VU]
            REQUIRE(initialVelocity.z == OTL_APPROX(-0.50634848));  // [VU]
            REQUIRE(finalVelocity.x   == OTL_APPROX(-0.60187442));  // [VU]
            REQUIRE(finalVelocity.y   == OTL_APPROX(-0.02234181));  // [VU]
            REQUIRE(finalVelocity.z   == OTL_APPROX(-0.84262419));  // [VU]
        }

        /// Test ExponentialSinusoidLambert.Evaluate() against Fundamentals of Astrodynamics 1st Edition, Bate Mueller & White, Example 5.3.1.
        SECTION("Truth Case: BMW 5.3.1 (long way)")
        {
            initialPosition = otl::Vector3d(0.5, 0.6, 0.7); // [DU]
            finalPosition = otl::Vector3d(0.0, 1.0, 0.0);   // [DU]
            timeOfFlight = 0.9667663;                       // [TU]
            mu = 1.0;
            direction = otl::Orbit::Direction::Retrograde;

            lambert->Evaluate(initialPosition,
                                finalPosition,
                                timeOfFlight,
                                direction,
                                maxRevolutions,
                                mu,
                                initialVelocity,
                                finalVelocity);

            REQUIRE(initialVelocity.x == OTL_APPROX(-0.6304918096));  // [VU]
            REQUIRE(initialVelocity.y == OTL_APPROX(-1.11392096659)); // [VU]
            REQUIRE(initialVelocity.z == OTL_APPROX(-0.8826885334));  // [VU] Assuming sign typo
            REQUIRE(finalVelocity.x   == OTL_APPROX(0.1786653974));   // [VU]
            REQUIRE(finalVelocity.y   == OTL_APPROX(1.5544139777));   // [VU]
            REQUIRE(finalVelocity.z   == OTL_APPROX(0.250135563));    // [VU]
        }
    }
}