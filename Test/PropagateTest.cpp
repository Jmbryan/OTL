#include <Catch/catch.hpp>
#include <OTL/Core/PropagateAnalytical.hpp>

const double TOL_PCT = 0.001;
#define OTL_APPROX(x) Approx(x, TOL_PCT)

TEST_CASE("PropagateAnalytical", "Propagate")
{
    auto propagator  = std::unique_ptr<otl::IPropagateAlgorithm>(new otl::PropagateAnalytical());

    otl::OrbitalElements orbitalElements;
    otl::StateVector stateVector;
    double mu;
    double timeOfFlight;

    SECTION("Propagate")
    {
        /// Test PropagateAnalytical.Propagate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-4.
        SECTION("Truth Case: Vallado 2-4")
        {
            stateVector.position = otl::Vector3d(1131.340, -2282.343, 6672.423); // [km]
            stateVector.velocity = otl::Vector3d(-5.64305, 4.30333, 2.42879);    // [km/s]
            mu = otl::ASTRO_MU_EARTH;                                            // [km^3/s^2]
            timeOfFlight = 40.0 * 60;                                            // [s]
            
            SECTION("OrbitalElements")
            {
                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                propagator->Propagate(orbitalElements, mu, timeOfFlight);

                otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                        stateVector,
                                                        mu);

                REQUIRE(stateVector.position.x == OTL_APPROX(-4219.7527)); // [km]
                REQUIRE(stateVector.position.y == OTL_APPROX(4363.0292));  // [km]
                REQUIRE(stateVector.position.z == OTL_APPROX(-3958.7666)); // [km]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(3.689866));   // [km/s]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-1.916735));  // [km/s]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(-6.112511));  // [km/s]
            }

            SECTION("StateVector")
            {
                propagator->Propagate(stateVector, mu, timeOfFlight);

                REQUIRE(stateVector.position.x == OTL_APPROX(-4219.7527)); // [km]
                REQUIRE(stateVector.position.y == OTL_APPROX(4363.0292));  // [km]
                REQUIRE(stateVector.position.z == OTL_APPROX(-3958.7666)); // [km]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(3.689866));   // [km/s]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-1.916735));  // [km/s]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(-6.112511));  // [km/s]
            }   
        }

        /// Test PropagateAnalytical.Propagate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-4.
        SECTION("Truth Case: Vallado 2-4 (canonical)")
        {
            stateVector.position = otl::Vector3d(0.177378, -0.357838, 1.046140); // [ER]
            stateVector.velocity = otl::Vector3d(-0.713825, 0.544356, 0.307233); // [ER/TU]
            mu = 1.0;                                                            // [ER^3/TU^2]
            timeOfFlight = 2.974674;                                             // [TU]

            SECTION("OrbitalElements")
            {
                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                propagator->Propagate(orbitalElements, mu, timeOfFlight);

                otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                        stateVector,
                                                        mu);

                REQUIRE(stateVector.position.x == OTL_APPROX(-0.661596)); // [ER]
                REQUIRE(stateVector.position.y == OTL_APPROX(0.684060));  // [ER]
                REQUIRE(stateVector.position.z == OTL_APPROX(-0.620678)); // [ER]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(0.466755));  // [ER/TU]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-0.242460)); // [ER/TU]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(-0.773210)); // [ER/TU]
            }

            SECTION("StateVector")
            {
                propagator->Propagate(stateVector, mu, timeOfFlight);

                REQUIRE(stateVector.position.x == OTL_APPROX(-0.661596)); // [ER]
                REQUIRE(stateVector.position.y == OTL_APPROX(0.684060));  // [ER]
                REQUIRE(stateVector.position.z == OTL_APPROX(-0.620678)); // [ER]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(0.466755));  // [ER/TU]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-0.242460)); // [ER/TU]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(-0.773210)); // [ER/TU]
            }
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.5.
        SECTION("Truth Case: Curtis 3.5")
        {
            double h = 100170.0;                                                // [km^2/s] 
            double e = 2.7696;
            double a = otl::SQR(h) / otl::ASTRO_MU_EARTH / (1.0 - otl::SQR(e)); // [km]

            orbitalElements.semiMajorAxis = a;                                  // [km]
            orbitalElements.eccentricity = e;                             
            orbitalElements.trueAnomaly = 100.0 * otl::MATH_DEG_TO_RAD;        // [rad]
            mu = 398600.0;                                                     // [km^3/s^2]
            timeOfFlight = 3.0 * 3600;                                         // [s]

            SECTION("OrbitalElements")
            {
                propagator->Propagate(orbitalElements, mu, timeOfFlight);

                REQUIRE(orbitalElements.trueAnomaly == OTL_APPROX(107.78 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            SECTION("StateVector")
            {
                otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                        stateVector,
                                                        mu);

                propagator->Propagate(stateVector, mu, timeOfFlight);

                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                REQUIRE(orbitalElements.trueAnomaly == OTL_APPROX(107.78 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.6.
        SECTION("Truth Case: Curtis 3.6")
        {
            double h = 95154.0;                                                 // [km^2/s] 
            double e = 1.4682;
            double a = otl::SQR(h) / otl::ASTRO_MU_EARTH / (1.0 - otl::SQR(e)); // [km]

            orbitalElements.semiMajorAxis = a;                                  // [km]
            orbitalElements.eccentricity = e;                             
            orbitalElements.trueAnomaly = 30.0 * otl::MATH_DEG_TO_RAD;          // [rad]
            mu = 398600.0;                                                      // [km^3/s^2]
            timeOfFlight = 1.0 * 3600;                                          // [s]

            SECTION("OrbitalElements")
            {
                propagator->Propagate(orbitalElements, mu, timeOfFlight);

                REQUIRE(orbitalElements.trueAnomaly == OTL_APPROX(100.04 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            SECTION("StateVector")
            {
                otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                        stateVector,
                                                        mu);

                propagator->Propagate(stateVector, mu, timeOfFlight);

                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                REQUIRE(orbitalElements.trueAnomaly == OTL_APPROX(100.04 * otl::MATH_DEG_TO_RAD)); // [rad]
            }
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.7.
        SECTION("Truth Case: Curtis 3.7")
        {
            stateVector.position = otl::Vector3d(7000.0, -12124.0, 0.0); // [km]
            stateVector.velocity = otl::Vector3d(2.6679, 4.6210, 0.0);   // [km/s]
            mu = 398600.0;                                               // [km^3/s^2]
            timeOfFlight = 60.0 * 60;                                    // [s]
            
            SECTION("OrbitalElements")
            {
                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                propagator->Propagate(orbitalElements, mu, timeOfFlight);

                otl::ConvertOrbitalElements2StateVector(orbitalElements,
                                                        stateVector,
                                                        mu);

                REQUIRE(stateVector.position.x == OTL_APPROX(-3296.8));  // [km]
                REQUIRE(stateVector.position.y == OTL_APPROX(7413.9));   // [km]
                REQUIRE(stateVector.position.z == OTL_APPROX(0.0));      // [km]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(-8.2977));  // [km/s]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-0.96309)); // [km/s]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(0.0));      // [km/s]
            }

            SECTION("StateVector")
            {
                propagator->Propagate(stateVector, mu, timeOfFlight);

                otl::ConvertStateVector2OrbitalElements(stateVector,
                                                        orbitalElements,
                                                        mu);

                REQUIRE(stateVector.position.x == OTL_APPROX(-3296.8));  // [km]
                REQUIRE(stateVector.position.y == OTL_APPROX(7413.9));   // [km]
                REQUIRE(stateVector.position.z == OTL_APPROX(0.0));      // [km]
                REQUIRE(stateVector.velocity.x == OTL_APPROX(-8.2977));  // [km/s]
                REQUIRE(stateVector.velocity.y == OTL_APPROX(-0.96309)); // [km/s]
                REQUIRE(stateVector.velocity.z == OTL_APPROX(0.0));      // [km/s]
            }      
        }
    }
}