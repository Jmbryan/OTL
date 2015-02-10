#include <OTL/Test/BaseTest.h>
#include <OTL/Core/KeplerianPropagator.h>
#include <OTL/Core/Conversion.h>

const double TOL_PCT = 0.001;
#define OTL_APPROX(x) Approx(x, TOL_PCT)

TEST_CASE("KeplerianPropagator", "")
{
   auto propagator = otl::keplerian::KeplerianPropagator();

    otl::OrbitalElements initialOrbitalElements;
    otl::OrbitalElements finalOrbitalElements;
    otl::OrbitalElements finalExpectedOrbitalElements;
    otl::StateVector initialStateVector;
    otl::StateVector finalStateVector;
    otl::StateVector finalExpectedStateVector;
    otl::Time timeOfFlight = otl::Time::Days(1);
    double mu;

    SECTION("Propagate")
    {
        /// Test PropagateAnalytical.Propagate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-4.
        SECTION("Truth Case: Vallado 2-4")
        {
            initialStateVector.position = otl::Vector3d(1131.340, -2282.343, 6672.423);            // [km]
            initialStateVector.velocity = otl::Vector3d(-5.64305, 4.30333, 2.42879);               // [km/s]
            finalExpectedStateVector.position = otl::Vector3d(-4219.7527, 4363.0292, -3958.7666);  // [km]
            finalExpectedStateVector.velocity = otl::Vector3d(3.689866, -1.916735, -6.112511);     // [km/s]
            mu = otl::ASTRO_MU_EARTH;                                                              // [km^3/s^2]
            timeOfFlight = otl::Time::Minutes(40.0);                                               // [s]
            
            initialOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(initialStateVector, mu);
            finalExpectedOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(finalExpectedStateVector, mu);

            SECTION("OrbitalElements")
            {
               finalOrbitalElements = propagator.Propagate(initialOrbitalElements, timeOfFlight, mu);

                CHECK(finalOrbitalElements.semiMajorAxis      == OTL_APPROX(finalExpectedOrbitalElements.semiMajorAxis));
                CHECK(finalOrbitalElements.eccentricity       == OTL_APPROX(finalExpectedOrbitalElements.eccentricity));
                CHECK(finalOrbitalElements.inclination        == OTL_APPROX(finalExpectedOrbitalElements.inclination));
                CHECK(finalOrbitalElements.argOfPericenter    == OTL_APPROX(finalExpectedOrbitalElements.argOfPericenter));
                CHECK(finalOrbitalElements.lonOfAscendingNode == OTL_APPROX(finalExpectedOrbitalElements.lonOfAscendingNode));
                CHECK(finalOrbitalElements.trueAnomaly        == OTL_APPROX(finalExpectedOrbitalElements.trueAnomaly));

                finalStateVector = otl::ConvertOrbitalElements2CartesianStateVector(finalOrbitalElements, mu);

                CHECK(finalStateVector.position.x() == OTL_APPROX(finalExpectedStateVector.position.x()));
                CHECK(finalStateVector.position.y() == OTL_APPROX(finalExpectedStateVector.position.y()));
                CHECK(finalStateVector.position.z() == OTL_APPROX(finalExpectedStateVector.position.z()));
                CHECK(finalStateVector.velocity.x() == OTL_APPROX(finalExpectedStateVector.velocity.x()));
                CHECK(finalStateVector.velocity.y() == OTL_APPROX(finalExpectedStateVector.velocity.y()));
                CHECK(finalStateVector.velocity.z() == OTL_APPROX(finalExpectedStateVector.velocity.z()));
            }

            SECTION("StateVector")
            {
               finalStateVector = propagator.Propagate(initialStateVector, timeOfFlight, mu);

               CHECK(finalStateVector.position.x() == OTL_APPROX(finalExpectedStateVector.position.x()));
               CHECK(finalStateVector.position.y() == OTL_APPROX(finalExpectedStateVector.position.y()));
               CHECK(finalStateVector.position.z() == OTL_APPROX(finalExpectedStateVector.position.z()));
               CHECK(finalStateVector.velocity.x() == OTL_APPROX(finalExpectedStateVector.velocity.x()));
               CHECK(finalStateVector.velocity.y() == OTL_APPROX(finalExpectedStateVector.velocity.y()));
               CHECK(finalStateVector.velocity.z() == OTL_APPROX(finalExpectedStateVector.velocity.z()));
               
               finalOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(finalStateVector, mu);

               CHECK(finalOrbitalElements.semiMajorAxis      == OTL_APPROX(finalExpectedOrbitalElements.semiMajorAxis));
               CHECK(finalOrbitalElements.eccentricity       == OTL_APPROX(finalExpectedOrbitalElements.eccentricity));
               CHECK(finalOrbitalElements.inclination        == OTL_APPROX(finalExpectedOrbitalElements.inclination));
               CHECK(finalOrbitalElements.argOfPericenter    == OTL_APPROX(finalExpectedOrbitalElements.argOfPericenter));
               CHECK(finalOrbitalElements.lonOfAscendingNode == OTL_APPROX(finalExpectedOrbitalElements.lonOfAscendingNode));
               CHECK(finalOrbitalElements.trueAnomaly        == OTL_APPROX(finalExpectedOrbitalElements.trueAnomaly));
            }   
        }

        /// Test PropagateAnalytical.Propagate() against Fundamentals of Astrodynamics and Applications 3rd Edition, David Vallado, Example 2-4.
        SECTION("Truth Case: Vallado 2-4 (canonical)")
        {
           initialStateVector.position = otl::Vector3d({ 0.177378, -0.357838, 1.046140 }); // [ER]
           initialStateVector.velocity = otl::Vector3d({ -0.713825, 0.544356, 0.307233 }); // [ER/TU]
            mu = 1.0;                                                                   // [ER^3/TU^2]
            timeOfFlight = otl::Time::Seconds(2.974674);                                // [TU] 

            SECTION("OrbitalElements")
            {
               initialOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(initialStateVector, mu);

                finalOrbitalElements = propagator.Propagate(initialOrbitalElements, timeOfFlight, mu);

                finalStateVector = otl::ConvertOrbitalElements2CartesianStateVector(finalOrbitalElements, mu);

                CHECK(finalStateVector.position.x() == OTL_APPROX(-0.661596)); // [ER]
                CHECK(finalStateVector.position.y() == OTL_APPROX(0.684060));  // [ER]
                CHECK(finalStateVector.position.z() == OTL_APPROX(-0.620678)); // [ER]
                CHECK(finalStateVector.velocity.x() == OTL_APPROX(0.466755));  // [ER/TU]
                CHECK(finalStateVector.velocity.y() == OTL_APPROX(-0.242460)); // [ER/TU]
                CHECK(finalStateVector.velocity.z() == OTL_APPROX(-0.773210)); // [ER/TU]
            }

            SECTION("StateVector")
            {
               finalStateVector = propagator.Propagate(initialStateVector, timeOfFlight, mu);

                CHECK(finalStateVector.position.x() == OTL_APPROX(-0.661596)); // [ER]
                CHECK(finalStateVector.position.y() == OTL_APPROX(0.684060));  // [ER]
                CHECK(finalStateVector.position.z() == OTL_APPROX(-0.620678)); // [ER]
                CHECK(finalStateVector.velocity.x() == OTL_APPROX(0.466755));  // [ER/TU]
                CHECK(finalStateVector.velocity.y() == OTL_APPROX(-0.242460)); // [ER/TU]
                CHECK(finalStateVector.velocity.z() == OTL_APPROX(-0.773210)); // [ER/TU]
            }
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.5.
        SECTION("Truth Case: Curtis 3.5")
        {
            double h = 100170.0;                                                      // [km^2/s] 
            double e = 2.7696;
            double a = otl::SQR(h) / otl::ASTRO_MU_EARTH / (1.0 - otl::SQR(e));       // [km]

            initialOrbitalElements.semiMajorAxis = a;                                 // [km]
            initialOrbitalElements.eccentricity = e;
            initialOrbitalElements.trueAnomaly = 100.0 * otl::MATH_DEG_TO_RAD;        // [rad]
            mu = 398600.0;                                                            // [km^3/s^2]
            timeOfFlight = otl::Time::Hours(3.0);                                     // [s] 

            SECTION("OrbitalElements")
            {
               finalOrbitalElements = propagator.Propagate(initialOrbitalElements, timeOfFlight, mu);

               CHECK(finalOrbitalElements.trueAnomaly == OTL_APPROX(107.78 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            SECTION("StateVector")
            {
               initialStateVector = otl::ConvertOrbitalElements2CartesianStateVector(initialOrbitalElements, mu);

                finalStateVector = propagator.Propagate(initialStateVector, timeOfFlight, mu);

                finalOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(finalStateVector, mu);

                CHECK(finalOrbitalElements.trueAnomaly == OTL_APPROX(107.78 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.6.
        SECTION("Truth Case: Curtis 3.6")
        {
            double h = 95154.0;                                                 // [km^2/s] 
            double e = 1.4682;
            double a = otl::SQR(h) / otl::ASTRO_MU_EARTH / (1.0 - otl::SQR(e)); // [km]

            initialOrbitalElements.semiMajorAxis = a;                           // [km]
            initialOrbitalElements.eccentricity = e;
            initialOrbitalElements.trueAnomaly = 30.0 * otl::MATH_DEG_TO_RAD;   // [rad]
            mu = 398600.0;                                                      // [km^3/s^2]
            timeOfFlight = otl::Time::Hours(1.0);                               // [s] 

            SECTION("OrbitalElements")
            {
               finalOrbitalElements = propagator.Propagate(initialOrbitalElements, timeOfFlight, mu);

                CHECK(finalOrbitalElements.trueAnomaly == OTL_APPROX(100.04 * otl::MATH_DEG_TO_RAD)); // [rad]
            }

            SECTION("StateVector")
            {
               initialStateVector = otl::ConvertOrbitalElements2CartesianStateVector(initialOrbitalElements, mu);

                finalStateVector = propagator.Propagate(initialStateVector, timeOfFlight, mu);

                finalOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(finalStateVector, mu);

                CHECK(finalOrbitalElements.trueAnomaly == OTL_APPROX(100.04 * otl::MATH_DEG_TO_RAD)); // [rad]
            }
        }

        /// Test PropagateAnalytical.Propagate() against Orbital Mechanics for Engineering Students 1st Edition, Howard Curtis, Example 3.7.
        SECTION("Truth Case: Curtis 3.7")
        {
           initialStateVector.position = otl::Vector3d({ 7000.0, -12124.0, 0.0 }); // [km]
           initialStateVector.velocity = otl::Vector3d({ 2.6679, 4.6210, 0.0 });   // [km/s]
            mu = 398600.0;                                                      // [km^3/s^2]
            timeOfFlight = otl::Time::Minutes(60.0);                            // [s]
            
            SECTION("OrbitalElements")
            {
               initialOrbitalElements = otl::ConvertCartesianStateVector2OrbitalElements(initialStateVector, mu);

                finalOrbitalElements = propagator.Propagate(initialOrbitalElements, timeOfFlight, mu);

                finalStateVector = otl::ConvertOrbitalElements2CartesianStateVector(finalOrbitalElements, mu);

                CHECK(finalStateVector.position.x() == OTL_APPROX(-3296.8));  // [km]
                CHECK(finalStateVector.position.y() == OTL_APPROX(7413.9));   // [km]
                CHECK(finalStateVector.position.z() == OTL_APPROX(0.0));      // [km]
                CHECK(finalStateVector.velocity.x() == OTL_APPROX(-8.2977));  // [km/s]
                CHECK(finalStateVector.velocity.y() == OTL_APPROX(-0.96309)); // [km/s]
                CHECK(finalStateVector.velocity.z() == OTL_APPROX(0.0));      // [km/s]
            }

            SECTION("StateVector")
            {
               finalStateVector = propagator.Propagate(initialStateVector, timeOfFlight, mu);

                CHECK(finalStateVector.position.x() == OTL_APPROX(-3296.8));  // [km]
                CHECK(finalStateVector.position.y() == OTL_APPROX(7413.9));   // [km]
                CHECK(finalStateVector.position.z() == OTL_APPROX(0.0));      // [km]
                CHECK(finalStateVector.velocity.x() == OTL_APPROX(-8.2977));  // [km/s]
                CHECK(finalStateVector.velocity.y() == OTL_APPROX(-0.96309)); // [km/s]
                CHECK(finalStateVector.velocity.z() == OTL_APPROX(0.0));      // [km/s]
            }      
        }
    }
}