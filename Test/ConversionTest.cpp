#include <Catch/catch.hpp>
#include <OTL/Core/Conversion.hpp>

const double TOL_PCT = 0.0001;
#define OTL_APPROX(x) Approx(x, TOL_PCT)

TEST_CASE("StateVector2OrbitalElements, Conversion")
{
    otl::StateVector stateVector;
    otl::OrbitalElements orbitalElements;
    double mu;

    /// Test StateVector2OrbitalElements() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-5.
    SECTION("Truth Case: Vallado 2-5")
    {
        stateVector.position = otl::Vector3d(6524.834, 6862.875, 6448.296);  // [km]
        stateVector.velocity = otl::Vector3d(4.901327, 5.533756, -1.976341); // [km/s]
        mu = otl::ASTRO_MU_EARTH;                                            // [km^3/s^2]
        
        otl::ConvertStateVector2OrbitalElements(stateVector,
                                                orbitalElements,
                                                mu);

        REQUIRE(orbitalElements.semiMajorAxis      == OTL_APPROX(36127.343));                     // [km]
        REQUIRE(orbitalElements.eccentricity       == OTL_APPROX(0.832853));
        REQUIRE(orbitalElements.inclination        == OTL_APPROX(87.870 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.argOfPericenter    == OTL_APPROX(53.38 * otl::MATH_DEG_TO_RAD));  // [rad]
        REQUIRE(orbitalElements.lonOfAscendingNode == OTL_APPROX(227.89 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.trueAnomaly        == OTL_APPROX(92.335 * otl::MATH_DEG_TO_RAD)); // [rad]
    }

    /// Test StateVector2OrbitalElements() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-5.
    SECTION("Truth Case: Vallado 2-5 (canonical)")
    {
        stateVector.position = otl::Vector3d(1.023, 1.076, 1.011); // [ER]
        stateVector.velocity = otl::Vector3d(0.62, 0.7, -0.25);    // [ER/TU]
        mu = 1.0;                                                  // [ER^3/TU^2]
        
        otl::ConvertStateVector2OrbitalElements(stateVector,
                                                orbitalElements,
                                                mu);

        REQUIRE(orbitalElements.semiMajorAxis      == OTL_APPROX(5.664247));                      // [ER]
        REQUIRE(orbitalElements.eccentricity       == OTL_APPROX(0.832853));
        REQUIRE(orbitalElements.inclination        == OTL_APPROX(87.870 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.argOfPericenter    == OTL_APPROX(53.38 * otl::MATH_DEG_TO_RAD));  // [rad]
        REQUIRE(orbitalElements.lonOfAscendingNode == OTL_APPROX(227.89 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.trueAnomaly        == OTL_APPROX(92.335 * otl::MATH_DEG_TO_RAD)); // [rad]
    }

    /// Test StateVector2OrbitalElements() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 4.3.
    SECTION("Truth Case: Curtis 4.3")
    {
        stateVector.position = otl::Vector3d(-6045.0, -3490.0, 2500.0); // [km]
        stateVector.velocity = otl::Vector3d(-3.457, 6.618, 2.533);     // [km/s]
        mu = 398600.0;                                                  // [km^3/s^2]
        
        otl::ConvertStateVector2OrbitalElements(stateVector,
                                                orbitalElements,
                                                mu);

        REQUIRE(orbitalElements.semiMajorAxis      == OTL_APPROX(8788.1));                         // [km]
        REQUIRE(orbitalElements.eccentricity       == OTL_APPROX(0.171212));
        REQUIRE(orbitalElements.inclination        == OTL_APPROX(153.249 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.argOfPericenter    == OTL_APPROX(20.0683 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.lonOfAscendingNode == OTL_APPROX(255.279 * otl::MATH_DEG_TO_RAD)); // [rad]
        REQUIRE(orbitalElements.trueAnomaly        == OTL_APPROX(28.4456 * otl::MATH_DEG_TO_RAD)); // [rad]
    }
}

TEST_CASE("OrbitalElements2StateVector, Conversion")
{
    otl::StateVector stateVector;
    otl::OrbitalElements orbitalElements;
    double mu;

    /// Test OrbitalElements2StateVector() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-6.
    SECTION("Truth Case: Vallado 2-6")
    {
        orbitalElements.semiMajorAxis      = 36127.343;                     // [km]
        orbitalElements.eccentricity       = 0.83285;
        orbitalElements.inclination        = 87.87 * otl::MATH_DEG_TO_RAD;  // [rad]
        orbitalElements.argOfPericenter    = 53.38 * otl::MATH_DEG_TO_RAD;  // [rad]
        orbitalElements.lonOfAscendingNode = 227.89 * otl::MATH_DEG_TO_RAD; // [rad]
        orbitalElements.trueAnomaly        = 92.335 * otl::MATH_DEG_TO_RAD; // [rad]
        mu = otl::ASTRO_MU_EARTH;                                           // [km^3/s^2]
        
        otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                stateVector,
                                                mu);

        REQUIRE(stateVector.position.x == OTL_APPROX(6525.344));  // [km]
        REQUIRE(stateVector.position.y == OTL_APPROX(6861.535));  // [km]
        REQUIRE(stateVector.position.z == OTL_APPROX(6449.125));  // [km]
        REQUIRE(stateVector.velocity.x == OTL_APPROX(4.902276));  // [km/s]
        REQUIRE(stateVector.velocity.y == OTL_APPROX(5.533124));  // [km/s]
        REQUIRE(stateVector.velocity.z == OTL_APPROX(-1.975709)); // [km/s]
    }

    /// Test OrbitalElements2StateVector() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-6.
    SECTION("Truth Case: Vallado 2-6 (canonical)")
    {
        orbitalElements.semiMajorAxis      = 5.664247;                      // [ER]
        orbitalElements.eccentricity       = 0.83285;
        orbitalElements.inclination        = 87.87 * otl::MATH_DEG_TO_RAD;  // [rad]
        orbitalElements.argOfPericenter    = 53.38 * otl::MATH_DEG_TO_RAD;  // [rad]
        orbitalElements.lonOfAscendingNode = 227.89 * otl::MATH_DEG_TO_RAD; // [rad]
        orbitalElements.trueAnomaly        = 92.335 * otl::MATH_DEG_TO_RAD; // [rad]
        mu = 1.0;                                                           // [ER^3/TU^2]
        
        otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                stateVector,
                                                mu);

        REQUIRE(stateVector.position.x == OTL_APPROX(1.02308));  // [ER]
        REQUIRE(stateVector.position.y == OTL_APPROX(1.07579));  // [ER]
        REQUIRE(stateVector.position.z == OTL_APPROX(1.01113));  // [ER]
        REQUIRE(stateVector.velocity.x == OTL_APPROX(0.62012));  // [ER/TU]
        REQUIRE(stateVector.velocity.y == OTL_APPROX(0.69992));  // [ER/TU]
        REQUIRE(stateVector.velocity.z == OTL_APPROX(-0.24992)); // [ER/TU]
    }

    /// Test OrbitalElements2StateVector() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 4.5.
    SECTION("Truth Case: Curtis 4.5")
    {
        double h = 80000.0;
        double e = 1.4;
        double a = otl::SQR(h) / otl::ASTRO_MU_EARTH / (1.0 - otl::SQR(e)); 

        orbitalElements.semiMajorAxis      = a;                           // [km]
        orbitalElements.eccentricity       = e;
        orbitalElements.inclination        = 30.0 * otl::MATH_DEG_TO_RAD; // [rad]
        orbitalElements.argOfPericenter    = 60.0 * otl::MATH_DEG_TO_RAD; // [rad]
        orbitalElements.lonOfAscendingNode = 40.0 * otl::MATH_DEG_TO_RAD; // [rad]
        orbitalElements.trueAnomaly        = 30.0 * otl::MATH_DEG_TO_RAD; // [rad]
        mu = 398600.0;                                                    // [km^3/s^2]
        
        otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                stateVector,
                                                mu);

        REQUIRE(stateVector.position.x == OTL_APPROX(-4039.9));  // [km]
        REQUIRE(stateVector.position.y == OTL_APPROX(4814.56));  // [km]
        REQUIRE(stateVector.position.z == OTL_APPROX(3628.62));  // [km]
        REQUIRE(stateVector.velocity.x == OTL_APPROX(-10.386));  // [km/s]
        REQUIRE(stateVector.velocity.y == OTL_APPROX(-4.77192)); // [km/s]
        REQUIRE(stateVector.velocity.z == OTL_APPROX(1.74388));  // [km/s]
    }
}