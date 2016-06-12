#include <OTL/Core/Planet.h>
#include <OTL/Core/LambertExponentialSinusoid.h>
#include <OTL/Core/Orbit.h>
#include <iostream>

using namespace std;
using namespace otl;

int main()
{
   cout << endl;
   cout << "-----------------------------" << endl;
   cout << "- Lambert's Problem Example -" << endl;
   cout << "-----------------------------" << endl;
   cout << endl;

   keplerian::Orbit::Direction orbitDirection = keplerian::Orbit::Direction::Prograde;
   int maxRevolutions = 2;

   Epoch epoch1 = Epoch::Gregorian(GregorianDateTime(2000, 1, 1, 6, 30, 0.0));
   Epoch epoch2 = Epoch::Gregorian(GregorianDateTime(2002, 10, 2, 20, 0, 0));
   Time timeDelta = epoch2 - epoch1;

   Planet p1 = Planet("Earth", epoch1);
   Planet p2 = Planet("Mars", epoch2);
  
   cout << "-----------------------------" << endl;
   cout << "-           Input           -" << endl;
   cout << "-----------------------------" << endl;
   cout << "Source planet: " << p1.ToString() << endl;
   cout << "Destination planet: " << p2.ToString() << endl;
   cout << "Lambert Options:" << endl;
   cout << "  Flight time: " << endl << timeDelta.ToDetailedString("    ");
   cout << "  Orbit Direction: " << "Prograde" << endl;
   cout << "  Max revolutions: " << maxRevolutions << endl;
   cout << endl;

   // Get the position vector of each planet at the departure and arrival time
   const auto& R1 = p1.GetStateVector().position;
   const auto& R2 = p2.GetStateVector().position;

   // Evaluate Lambert's Problem
   auto lambert = keplerian::LambertExponentialSinusoid();
   std::vector<Vector3d> V1, V2;
   lambert.Evaluate(
      R1,
      R2,
      timeDelta,
      orbitDirection,
      maxRevolutions,
      ASTRO_MU_SUN,
      V1,
      V2);

   // Print the departure and arrival state vectors for each solution found
   cout << "-----------------------------" << endl;
   cout << "-          Output           -" << endl;
   cout << "-----------------------------" << endl;
   cout << "Lambert Solutions:" << endl;
   for (size_t i = 0; i < V1.size(); ++i)
   {
      StateVector sv1(R1, V1[i]);
      StateVector sv2(R2, V2[i]);

      keplerian::Orbit transferOrbit(ASTRO_MU_SUN, sv1);

      double period = 10 * transferOrbit.GetOrbitProperties().period; // [TODO] off by a factor of 10 somewhere??

      int nRevs = static_cast<int>(timeDelta.Seconds() / period);

      cout << "  Solution #: " << i + 1 << endl;
      cout << "    Number of revolutions: " << nRevs << endl;
      cout << "    Departure State Vector: " << endl << sv1.ToDetailedString("      ");
      cout << "    Arrival State Vector: " << endl << sv2.ToDetailedString("      ") << endl;
      //cout << "    Transfer Orbit: " << endl << transferOrbit.ToString("      ") << endl;
   }

   return 0;
}