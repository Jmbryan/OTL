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
 * \class DE405Ephemeris
 *
 * Ephemeris using JPL DE405.  This class provides a series of routines for 
 * computing planetary positions and velocities.  It also provides for 
 * terrestrial nutation and lunar libration angles.
 *
 * This code is not thread safe.
 */
#ifndef DE405EPHEMERIS_H
#define DE405EPHEMERIS_H

#include <memory>
#include <string>
#include <vector>

class DE405EphemerisIO;

class DE405Ephemeris {

    public:
        /**
         * Astronomical entities for which position and velocity can be 
         * calculated.  Note that Terrestrial Nutations and Lunar Librations 
         * are not included, since they are angular measures and are computing 
         * with a unique calculation.
         */
        enum AstroEntity {
            Mercury = 0,
            Venus = 1,
            EarthMoonBarycenter = 2,
            Mars = 3,
            JupiterBarycenter = 4, 
            SaturnBarycenter = 5,
            UranusBarycenter = 6,
            NeptuneBarycenter = 7,
            PlutoBarycenter = 8,
            Moon = 9,
            Sun = 10 };


    private:
        // Scratch space for chebyshev position and velocity polys
        mutable double posPoly_[ 14 ];
        mutable double velPoly_[ 14 ];

        // IO object for Ephemeris coefficient data
        std::auto_ptr<DE405EphemerisIO> io;

        // Ratio of masses between Earth and Moon
        static const double emrat_;


    public:
        DE405Ephemeris( const std::string& dataFilename );
        ~DE405Ephemeris(void);

        void getPosition( double julianDay, AstroEntity entity, 
                          double* pos ) const;

        void getVelocity( double julianDay, AstroEntity entity, 
                          double* vel ) const;

        void getPosVel( double julianDay, AstroEntity entity, 
                        double* pos, double* vel ) const;


    private:
        void getInterpolationInfo( double julianDay, 
                                   AstroEntity entity, 
                                   std::vector<double>& coeffs,
                                   double& setsPerDay, 
                                   double& chebyshevTime ) const;

        void computePosPoly( double chebyshevTime, 
                             unsigned int numCoeffs ) const;

        void computeVelPoly( double chebyshevTime, 
                             unsigned int numCoeffs ) const;

        void computePos( const std::vector<double>& coeffs, double* pos ) const;

        void computeVel( const std::vector<double>& coeffs,
                         double setsPerDay,
                         double& chebyshevTime, 
                         double* vel ) const;


    private:
        // disable default ctor
        DE405Ephemeris( void );

};


/**
 * Internal helper routine for computing the position polynomial.  The 
 * polynomial is solely a function of Chebyshev time.  Note that the first term,
 * always equal to 1.0, is set in the constructor.
 *
 * \param chebyshevTime      The Chebyshev time parameter
 * \param numCoeffs          Order of the polynomial being generated
 */
inline void 
DE405Ephemeris::computePosPoly( double chebyshevTime, 
                                unsigned int numCoeffs ) const {

    // Note that first term already set in ctor
    posPoly_[1] = chebyshevTime;

    for ( unsigned int i = 2; i < numCoeffs; i++ ) {
        posPoly_[i] = 2.0 * chebyshevTime * posPoly_[i-1] - posPoly_[i-2];
    }
}


/**
 * Internal helper routine for computing the velocity polynomial.  The 
 * polynomial is solely a function of Chebyshev time.  However, it uses the 
 * values of the associated Chebyshev position polynomial.  Therefore, 
 * computePosPoly() must always be called before this function.  Note that the 
 * first term, always equal to 1.0, is set in the constructor.
 *
 * \param chebyshevTime      The Chebyshev time parameter
 * \param numCoeffs          Order of the polynomial being generated
 */
inline void 
DE405Ephemeris::computeVelPoly( double chebyshevTime, 
                                unsigned int numCoeffs ) const {

    // Note that first two terms already set in ctor
    velPoly_[2] = 4.0 * chebyshevTime;

    for ( unsigned int i = 3; i < numCoeffs; i++ ) {
        velPoly_[i] = 2.0 * chebyshevTime * velPoly_[i-1] +
                      2.0 * posPoly_[i-1] -
                      velPoly_[i-2];
    }
}


/**
 * Internal helper routine for computing the position of a body.  Before using 
 * this function, computePosPoly() must be called.
 *
 * \param coeffs             Vector of Chebyshev coefficients for the entity and
 *                           time being computed.
 * \param pos                A three element array in which to store the 
 *                           resulting position in km using the ICRF as the 
 *                           coordinate system.
 */
inline void
DE405Ephemeris::computePos( const std::vector<double>& coeffs, double* pos ) 
    const {

    int coeffsIdx = 0;

    // Simply multiply the coefficients against the chebyshev position poly
    for ( unsigned int d = 0; d < 3; d++ ) {

        // First position poly term is 1 so we can skip multiplication
        pos[d] = coeffs[ coeffsIdx++ ];

        for ( unsigned int i = 1; i < coeffs.size() / 3; i++ ) {
            pos[d] += coeffs[ coeffsIdx++ ] * posPoly_[i];
        }
    }
}


/**
 * Internal helper routine for computing the velocity of a body.  Before using 
 * this function, computeVelPoly() must be called.
 *
 * \param coeffs             Vector of Chebyshev coefficients for the entity and
 *                           time being computed.
 * \param setsPerDay         Indicates how the subinterval size of each 32 day 
 *                           block.
 * \param chebyshevTime      The Chebyshev time parameter.
 * \param vel                A three element array in which to store the 
 *                           resulting velocity in km/day using the ICRF.
 */
inline void
DE405Ephemeris::computeVel( const std::vector<double>& coeffs, 
                            double setsPerDay, double& chebyshevTime, 
                            double* vel ) const {

    int coeffsIdx = 0;

    // Simply multiply the coefficients against the chebyshev velocity poly
    for ( unsigned int d = 0; d < 3; d++ ) {

        // Because first velocity polynomial coeff is 0, we skip first coeffs
        coeffsIdx++;

        // Second velocity poly term is 1 so we can skip multiplication
        vel[d] = coeffs[ coeffsIdx++ ];

        for ( unsigned int i = 2; i < coeffs.size() / 3; i++ ) {
            vel[d] += coeffs[ coeffsIdx++ ] * velPoly_[i];
        }
    }

    // This converts our units from chebyshev time to days
    double chebyshevTimeToDays = 2.0 * setsPerDay;
    vel[0] *= chebyshevTimeToDays;
    vel[1] *= chebyshevTimeToDays;
    vel[2] *= chebyshevTimeToDays;
    
}


#endif // DE405EPHEMERIS_H
