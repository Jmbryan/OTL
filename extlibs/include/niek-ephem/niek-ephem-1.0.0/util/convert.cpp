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
 * A utility to convert the ASCII DE405 ephemeris files to a single binary file.
 *
 * Uses a FORTRAN exponent translation code stolen (and cleaned up) from:
 *   http://experts.about.com/q/C-1040/C-FORTRAN-communication.htm
 *
 */
#include <cassert>
#include <cmath> 
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>


// Sub-class template of the std::num_get facet class template
// used to parse numeric input character sequences
template < class charT, class InputIterator = std::istreambuf_iterator<charT> >
class DFormatNumGet : public std::num_get< charT, InputIterator > {

    public:
        // Ensure we have base types available
        typedef typename std::num_get< charT, InputIterator > base_type;
        typedef typename base_type::iter_type iter_type;
        typedef typename base_type::char_type char_type;

        // Replicate the base constructor. Pass on most of the work to the base 
        // constructor. Get and save reference to current global num_get facet
        DFormatNumGet( size_t refs = 0 ) 
            : base_type( refs ), 
              iPrevNumGet( std::use_facet<base_type>( std::locale() ) ) {
        }

        protected:
            // Override the base class virtual member function that handles 
            // parsing doubles type character sequences
            virtual iter_type do_get( iter_type in, iter_type end, 
                                      std::ios_base& ib, 
                                      std::ios_base::iostate& err, 
                                      double& v ) const;

        private:
            // Reference to global num_get facet in effect when instance created
            base_type const &     iPrevNumGet;
};


// Implementation of overriden double value parsing function
template < class charT, class InputIterator >
typename DFormatNumGet<charT,InputIterator>::iter_type
DFormatNumGet<charT,InputIterator>::do_get( iter_type in, iter_type end, 
                                            std::ios_base& ib, 
                                            std::ios_base::iostate& err, 
                                            double& v ) const {

    // Initially parse sequence using previous num_get facet
    iter_type next( iPrevNumGet.get(in, end, ib, err, v) );

    // Continue if there was no error
    if ( 0==err || std::ios::goodbit==err ) {

        // If we are not at the end of the sequence and the current character is
        // the D format exponent character then we have some work to do...
        if ( next != end && *next == 'D' ) {

            // skip exponent character
            ++next; 

            // Get the exponent value: This is a integer value so we use the 
            // stashed num_get facet reference to parse the integer value as a 
            // long:
            err = std::ios::goodbit;
            long exponent;
            next= iPrevNumGet.get( in, end, ib, err, exponent );

            if ( std::ios::goodbit == err ) {

                // If all well check exponent value is in range.  
                if ( std::numeric_limits<double>::min_exponent10 <= exponent && 
                     std::numeric_limits<double>::max_exponent10 >= exponent ) {

                    // If the exponent value is in range apply it to the double 
                    // value we already obtained:
                    v *= pow(10.0, exponent);

                } 

                // exponent out of range 
                else {    
                    err = std::ios::failbit;
                }
            }
        }
    }

    // next character position in sequence
    return next; 
}


int main( int argc, char* argv[] ) {

    /*
     * BEGIN USER CONFIGURABLE PARAMETERS
     */
    // Directory in which ASCII data files are stashed
    const std::string dataDir = "/dirs/static/JPL_DE405/";

    // Name of file to output
    const std::string outputFilename = "de405.data";

    // Which entities to include ephemeris info for
    bool includeEntity[] = {
        true,                    // Mercury
        true,                    // Venus
        true,                    // Earth-Moon Barycenter
        true,                    // Mars
        true,                    // Jupiter Barycenter
        true,                    // Saturn Barycenter
        true,                    // Uranus Barycenter
        true,                    // Neptune Barycenter
        true,                    // Pluto Barycenter
        true,                    // Moon Geocenter
        true,                    // Sun
        true,                    // Terrestrial Nutations
        true                     // Lunar Librations
    };
    
    // You can cut down the temporal coverage of the ephemerides here
    double startDay = 2305424.50;
    double endDay = 2525008.50;


    /*
     * NO USER CONFIGURABLE PARAMETERS BELOW THIS POINT
     */
    assert( startDay < endDay );

    // Hardcoded data (could theoretically be read from header.405)
    int polyDegree[] =      
        { 14, 10,  13,  11,  8,   7,   6,   6,   6,   13,  11,  10,  10 };
    int numSubdivisions[] = 
        { 4,  2,   2,   1,   1,   1,   1,   1,   1,   8,   2,   4,   4 };
    int numDims[] = 
        { 3,  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   2,   3 };


    // Compute coefficient offsets and total
    int coeffPerRecord = 0;
    int coeffOffsets[ 13 ];
    for ( int i = 0; i < 13; i++ ) {
        if ( includeEntity[i] ) {
            int numCoeffs = polyDegree[i] * numSubdivisions[i] * numDims[i];
            coeffOffsets[i] = coeffPerRecord;
            coeffPerRecord += numCoeffs;
        } 
        else {
            coeffOffsets[i] = -1;
        }
    }


    // Open binary output file for results 
    std::ofstream ofs( outputFilename.c_str(), 
                       std::ios::binary | std::ios::trunc );
    if ( !ofs ) {
        std::cerr << "Error: unable to open output file '" << outputFilename 
                  << "'" << std::endl;
        return 1;
    }


    // Dump the layout information
    for ( int i = 0; i < 13; i++ ) 
        ofs.write( reinterpret_cast<char*>( coeffOffsets + i ), sizeof( int ) );
    for ( int i = 0; i < 13; i++ ) 
        ofs.write( reinterpret_cast<char*>( polyDegree + i ), sizeof( int ) );
    for ( int i = 0; i < 13; i++ ) 
        ofs.write( reinterpret_cast<char*>( numSubdivisions + i ), sizeof( int ) );

    // Dump the number of coeffs per record
    ofs.write( reinterpret_cast<char*>( &coeffPerRecord ), sizeof( int ) );

    // Dump the start/stop day
    ofs.write( reinterpret_cast<char*>( &startDay ), sizeof( double ) );
    ofs.write( reinterpret_cast<char*>( &endDay ), sizeof( double ) );


    // Create translator facet handling FORTRAN doubles with 'D' instead of 'E'
    DFormatNumGet<char> dFormatNumGet(1);

    // Grab a copy of the global locale and replace in our translator facet
    std::locale dFormatLocale( std::locale(), &dFormatNumGet );


    // Open the ASCII file associated with each year, dump out coeffs
    int bufI;
    double bufD;

    // track total number of records written to binary
    int totalRecords = 0;

    // records may be duplicated across 20 yr blocks
    double lastWrittenStart = -1.0;       
    
    for ( int fnum = 1600; fnum <= 2200; fnum += 20 ) {

        // Forge the filename
        std::ostringstream fnameStr;
        fnameStr << dataDir << "ascp" << fnum << ".405";

        // Open the ASCII data file
        std::ifstream ifs( fnameStr.str().c_str() );
        if ( !ifs ) {
            std::cerr << "Error: unable to open input file '" << fnameStr.str() 
                      << "'" << std::endl;
            return 1;
        }

        // Imbue stream with our FORTRAN exponent translator
        ifs.imbue( dFormatLocale );

        // Pull every record, failure to get record number implies eof
        int recordsWritten = 0;
        while( ifs >> bufI ) {

            // Read and ignore num coeffs
            ifs >> bufI;
 
            // Read start/end day at beginning of record
            double currentStart;
            ifs >> currentStart >> bufD;

            // Read coeffs and write to binary output file 
            for ( int i = 0; i < 1016; i++ ) {

                // Suck in ascii coeff value
                ifs >> bufD;

                // Check if we are writing record (not dup) and this 
                // entity is enabled
                int tmp = i;
                int curEnt;
                for ( curEnt = 0; curEnt < 13; curEnt++ ) {
                    tmp -= polyDegree[curEnt] * numSubdivisions[curEnt] * 
                           numDims[curEnt];
                    if ( tmp < 0 )
                        break;
                }

                if ( ( currentStart > lastWrittenStart ) && 
                     ( currentStart >= startDay ) &&
                     ( currentStart <= endDay ) &&
                     ( includeEntity[curEnt] ) ) {
                    ofs.write( reinterpret_cast<char*>( &bufD ), 
                               sizeof( double ) );
                }
            }

            // Last two vals in coeff block always 0.0 and should be discarded
            ifs >> bufD >> bufD;

            if ( ( currentStart > lastWrittenStart ) &&
                 ( currentStart >= startDay ) &&
                 ( currentStart <= endDay ) ) {
                lastWrittenStart = currentStart;
                recordsWritten++;
            }

        }
        std::cout << "Wrote " << recordsWritten << " records from " 
                  << fnameStr.str() << std::endl;
        totalRecords += recordsWritten;
    }

    std::cout << "\nTotal of " << totalRecords << " records written to " 
              << outputFilename << std::endl;

    return 0;
}
