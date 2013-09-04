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
 * \file DE405EphemerisIO.cpp
 *
 * This source implements the DE405EphemerisIO class.
 */
#include "DE405EphemerisIO.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

const double DE405EphemerisIO::intervalDuration_ = 32.0;


/**
 * Creation constructor.  Sets the nonchanging terms of the Chebyshev position 
 * and velocity polynomials.
 *
 * \param dataFilename       Binary DE405 ephemerides file.
 */
DE405EphemerisIO::DE405EphemerisIO( const std::string& dataFilename ) 
    : dataFilename_( dataFilename ), dataStream_(), coeffPerRecord_( 0 ) {

    // Open file
    dataStream_.open( dataFilename.c_str(), std::ios::binary );

    if ( !dataStream_ ) {
        std::cerr << "Unable to open coefficient file '" << dataFilename << "'"
                  << std::endl;
        exit( 1 );
    }
 
    // Yank out header information
    union BufI {
        char cdata[ sizeof(int) ];
        int intVal;
    } bufI;

    union BufD {
        char cdata[ sizeof(double) ];
        double dblVal;
    } bufD;

    for ( int idx = 0; idx < 13; idx++ ) {
        if ( !( dataStream_.read( bufI.cdata, sizeof(int) ) ) ) {
            std::cerr << "Error: unable to read layout info" << std::endl;     
            exit( 1 );
        }
        coeffOffsets_[idx] = bufI.intVal;
    }

    for ( int idx = 0; idx < 13; idx++ ) {
        if ( !( dataStream_.read( bufI.cdata, sizeof(int) ) ) ) {
            std::cerr << "Error: unable to read layout info" << std::endl;     
            exit( 1 );
        }
        numCoeffs_[idx] = bufI.intVal;
    }

    for ( int idx = 0; idx < 13; idx++ ) {
        if ( !( dataStream_.read( bufI.cdata, sizeof(int) ) ) ) {
            std::cerr << "Error: unable to read layout info" << std::endl;     
            exit( 1 );
        }
        numCoeffSets_[idx] = bufI.intVal;
    }

    if ( !( dataStream_.read( bufI.cdata, sizeof(int) ) ) ) {
        std::cerr << "Error: unable to coeffs per record" << std::endl;     
        exit( 1 );
    }
    coeffPerRecord_ = bufI.intVal;

    if ( !( dataStream_.read( bufD.cdata, sizeof(double) ) ) ) {
        std::cerr << "Error: unable to read start day" << std::endl;     
        exit( 1 );
    }
    startDay_ = bufD.dblVal;

    if ( !( dataStream_.read( bufD.cdata, sizeof(double) ) ) ) {
        std::cerr << "Error: unable to read end day" << std::endl;     
        exit( 1 );
    }
    endDay_ = bufD.dblVal;
}


/**
 * Internal helper function for pulling interpolation information.  Is 
 * responsible for returning a pointer to the entity's Chebyshev coefficients 
 * for the specified time.  It also computes the associated Chebyshev time 
 * parameter.
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
DE405EphemerisIO::getInterpolationInfo( double julianDay, 
                                        DE405Ephemeris::AstroEntity entity, 
                                        std::vector<double>& coeffs, 
                                        double& setsPerDay, 
                                        double& chebyshevTime ) const {

    // Make sure we are within date range
    if ( julianDay < startDay_ ) 
        throw std::range_error( 
            "Interpolation data request before earliest available date" );
    if ( julianDay > endDay_ ) 
        throw std::range_error( 
            "Interpolation data request after latest available date" );

    // Make sure we actually have data for that entity
    if ( coeffOffsets_[ entity ] == -1 ) 
        throw std::range_error( 
            "Interpolation data not available for requested entity" );

    // Pull number of coefficients for this entity
    int numCoeffs = numCoeffs_[ entity ];

    // number of polys for entity, divided by 32 days per record
    setsPerDay = numCoeffSets_[ entity ] / 32.0;
    double daysPerSet = 1.0 / setsPerDay;

    // Figure out which record we're in
    int recordNum = static_cast<int>(( julianDay - startDay_ ) / 32.0);
    double recordStartDay = recordNum * 32.0 + startDay_;

    // Figure out which subdivision we're in
    int subRecordNum = 
        static_cast<int>( (julianDay - recordStartDay) * setsPerDay );

    // Figure out the file offset
    static const int headerSize = (sizeof(int)*13*3) + 
                                  (sizeof(int)) + 
                                  (sizeof(double)*2); 
    long offset = headerSize +
                  (recordNum * coeffPerRecord_ * sizeof(double)) +
                  (coeffOffsets_[entity] * sizeof(double)) +
                  (subRecordNum * numCoeffs * 3 * sizeof(double));

    // Jump to offset
    dataStream_.seekg( offset, std::ios_base::beg );
    if ( !dataStream_ ) {
        std::cerr << "Error: critical error while seeking file offset " 
                  << offset << std::endl;
        exit( 1 );
    }

    int i = 0;
    union BufD {
        char cdata[ sizeof(double) ];
        double dblVal;
    } bufD;
    coeffs.clear();
    while ( dataStream_.read( bufD.cdata, sizeof(double) ) && 
            ( i < (numCoeffs*3) ) ) {
        coeffs.push_back( bufD.dblVal );
        i++;
    }
 
    // Compute chebyshev time
    chebyshevTime = 
        2.0 * ((julianDay - (recordStartDay + subRecordNum * daysPerSet)) / 
               daysPerSet) - 1;
    assert( chebyshevTime >= -1.0 );
    assert( chebyshevTime <= 1.0 );

}
