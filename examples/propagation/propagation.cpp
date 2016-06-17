#include <iostream>
#include <OTL/Core/Base.h>
#include <OTL/Core/LagrangianPropagator.h>

using namespace std;
using namespace otl;

int main()
{
   cout << endl;
   cout << "-----------------------------" << endl;
   cout << "-    Propagation Example    -" << endl;
   cout << "-----------------------------" << endl;
   cout << endl;

   auto propagator = otl::keplerian::LagrangianPropagator();
   
    // Setup inputs
    StateVector stateVector;
    stateVector.position = Vector3d(1131.340, -2282.343, 6672.423);  // Absolute position (km)
    stateVector.velocity = Vector3d(-5.64305, 4.30333, 2.42879);     // Absolute velocity (km/s)
    double mu = ASTRO_MU_EARTH;                                      // Gravitational parameter of Earth
    Time timeDelta = Time::Minutes(40.0);                            // Propagate forward 40 minutes

    cout << "-----------------------------" << endl;
    cout << "-           Input           -" << endl;
    cout << "-----------------------------" << endl;
    cout << "Initial state vector:" << endl << stateVector.ToDetailedString("  ") << endl;
    cout << "Propagation time:" << endl << timeDelta.ToDetailedString("  ") << endl;
   
    auto initialStateVector = stateVector;
   
    // Propagate the state vector forwards in time
    auto finalStateVector = propagator.PropagateStateVector(initialStateVector, mu, timeDelta);
   
    // Now propagate backwards in time to verify we end up where we started
    auto initialStateVector2 = propagator.PropagateStateVector(finalStateVector, mu, -timeDelta);
   
    cout << "-----------------------------" << endl;
    cout << "-          Output           -" << endl;
    cout << "-----------------------------" << endl;
    cout << "Final state vector:" << endl << finalStateVector.ToDetailedString("  ") << endl;
    cout << "Initial state vector after backwards propgation:" << endl << initialStateVector2.ToDetailedString("  ") << endl;

   return 0;
}