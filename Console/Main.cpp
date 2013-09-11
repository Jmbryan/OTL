#include <iostream>
#include "DE405Ephemeris.h"
#include <assert.h>

int main()
{
    DE405Ephemeris* ephem = nullptr;

    ephem = new DE405Ephemeris("E:/Dev/OTL/data/jpl_eph/de405/de405.data");

    double pos[3];
    double vel[3];

    ephem->getPosition(2451536.5, DE405Ephemeris::Mercury, pos);

    // Truth values from JPL Horizons System
    assert( std::fabs( -4.468523069712503E+07 - pos[0] ) < 1E-3 );
    assert( std::fabs( -4.796771237574679E+07 - pos[1] ) < 1E-3 );
    assert( std::fabs( -2.102675128947175E+07 - pos[2] ) < 1E-3 );

    return 0;
}