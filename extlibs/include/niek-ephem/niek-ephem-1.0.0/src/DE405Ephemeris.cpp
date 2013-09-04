/*
 * Copyright (c) 2007, Niek Sanders
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Rochester Institute of Technology nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file DE405Ephemeris.cpp
 *
 * This source implements the DE405Ephemeris class.
 */
#include <cassert>
#include <vector>

#include "DE405Ephemeris.h"
#include "DE405EphemerisIO.h"

const double DE405Ephemeris::emrat_ = 0.813005600000000044E+02;


/**
 * Creation constructor.  Sets the nonchanging terms of the Chebyshev position 
 * and velocity polynomials.  Also indicates which file the IO object 
 * should open.
 *
 * \param dataFilename       Binary DE405 ephemeris file
 */
DE405Ephemeris::DE405Ephemeris( const std::string& dataFilename ) 
    : io( new DE405EphemerisIO( dataFilename ) ) {

    // First term of chebyshev position poly is always 1.0
    posPoly_[0] = 1.0;

    // First and second terms of chebyshev velocity poly always 0.0 and 1.0
    velPoly_[0] = 0.0;
    velPoly_[1] = 1.0;

}


/**
 * Destructor.  Defined explicitly so that the auto pointer to the IO can be 
 * destructed without having add include dependencies to this class's interface 
 * file.
 */
DE405Ephemeris::~DE405Ephemeris( void ) {
}


/**
 * Compute the position for a given entity and time.  This function fills the 
 * three element pos array with the position (ICRF) in km.  If the velocity is 
 * also needed, use the getPosVel() function instead.
 *
 * \param julianDay          The Julian Day.
 * \param entity             Entity for which position is being computed.
 * \param pos                Three element array in which to store resulting 
 *                           position (km using ICRF).
 */
void 
DE405Ephemeris::getPosition( double julianDay, AstroEntity entity, 
                             double* pos ) const {

    std::vector<double> coeffs;
    double setsPerDay;
    double chebyshevTime;

    getInterpolationInfo( julianDay, entity, 
                          coeffs, setsPerDay, chebyshevTime );
    computePosPoly( chebyshevTime, coeffs.size() / 3 );
    computePos( coeffs, pos );

    // The moon is given relative to the EMB.  Needs conversion to ICRF
    if ( entity == Moon ) {

        // Compute EMB position
        double embPos[3];
        getPosition( julianDay, EarthMoonBarycenter, embPos );
        
        // Compute Moon position
        for ( int i = 0; i < 3; i++ )
            pos[i] += embPos[i] - ( pos[i] / (emrat_ + 1) );
    }
}


/**
 * Compute the velocity for a given entity and time.  This function fills the 
 * three element vel array with the velocity in km/sec.  If the position is 
 * also needed, use the getPosVel() function instead.
 *
 * \param julianDay          The Julian Day.
 * \param entity             Entity for which velocity is being computed.
 * \param vel                Three element array in which to store resulting 
 *                           velocity (km/day using ICRF).
 */
void 
DE405Ephemeris::getVelocity( double julianDay, AstroEntity entity, 
                             double* vel ) const {

    std::vector<double> coeffs;
    double setsPerDay;
    double chebyshevTime;

    getInterpolationInfo( julianDay, entity, 
                          coeffs, setsPerDay, chebyshevTime );
    computePosPoly( chebyshevTime, coeffs.size() / 3 );
    computeVelPoly( chebyshevTime, coeffs.size() / 3 );
    computeVel( coeffs, setsPerDay, chebyshevTime, vel );

    // The moon is given relative to the EMB.  Needs conversion to ICRF
    if ( entity == Moon ) {

        // Compute EMB velocity
        double embVel[3];
        getVelocity( julianDay, EarthMoonBarycenter, embVel );
        
        // Compute Moon velocity
        for ( int i = 0; i < 3; i++ )
            vel[i] += embVel[i] - ( vel[i] / (emrat_ + 1) );
    }

}


/**
 * Compute the position and velocity for a given entity and time.  This function
 * is more efficient than computing these two values in two separate calls.  The
 * results are in the International Celestial Reference Frame and have units of 
 * [km] and [km/day].
 *
 * \param julianDay          The Julian Day.
 * \param entity             Entity for which position and velocity are being 
 *                           computed.
 * \param pos                Three element array in which to store resulting 
 *                           position (km using ICRF).
 * \param vel                Three element array in which to store resulting 
 *                           velocity (km/day using ICRF).
 */
void 
DE405Ephemeris::getPosVel( double julianDay, AstroEntity entity, 
                           double* pos, double* vel ) const {

    std::vector<double> coeffs;
    double setsPerDay;
    double chebyshevTime;

    getInterpolationInfo( julianDay, entity, 
                          coeffs, setsPerDay, chebyshevTime );
    computePosPoly( chebyshevTime, coeffs.size() / 3 );
    computeVelPoly( chebyshevTime, coeffs.size() / 3 );
    computePos( coeffs, pos );
    computeVel( coeffs, setsPerDay, chebyshevTime, vel );

    // The moon is given relative to the EMB.  Needs conversion to ICRF
    if ( entity == Moon ) {

        // Compute EMB pos/vel
        double embPos[3];
        double embVel[3];
        getPosVel( julianDay, EarthMoonBarycenter, embPos, embVel );
        
        // Compute Moon pos/vel
        for ( int i = 0; i < 3; i++ ) {
            pos[i] += embPos[i] - ( pos[i] / (emrat_ + 1) );
            vel[i] += embVel[i] - ( vel[i] / (emrat_ + 1) );
        }
    }
}


/**
 * Internal helper function for pulling interpolation information.  It passes 
 * along the request the internal IO object.
 *
 * \param julianDay          Julian day (fractional) for interpolation.
 * \param entity             Name of entity being interpolated for.
 * \param coeffs             Vector in which to store coeffs.
 * \param setsPerDay         Indicates the number of sets (normally fractional) 
 *                           per day in the record.  This is needed in the 
 *                           velocity calculations to convert from Chebyshev 
 *                           time back to days.
 * \param chebyshevTime      Double in which to store computed Chebyshev time.
 * \throw std::range_error   If interpolation data not available for target.
 */
void 
DE405Ephemeris::getInterpolationInfo( double julianDay, 
                                      AstroEntity entity, 
                                      std::vector<double>& coeffs, 
                                      double& setsPerDay, 
                                      double& chebyshevTime ) const {

    io->getInterpolationInfo( julianDay, entity, 
                              coeffs, setsPerDay, chebyshevTime );

}
