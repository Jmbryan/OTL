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
 * This will become a simple Read-Eval-Print-Loop (REPL) interface to the 
 * ephemeris library.  It is intended to demonstrate proper usage of the 
 * supplied routines.
 *
 */
#include <iostream>
#include <string>
#include <vector>

#include "../src/DE405Ephemeris.h"

int main( int argc, char* argv[] ) {

    // Check command line args
    if ( argc != 2 ) {
        std::cerr << "Usage: ephem_demo <ephemerides>" << std::endl;
        return 1;
    }


    // Ephemeris object we can query
    DE405Ephemeris ephem( argv[ 1 ] );

    // The entities the user may query
    std::vector<std::string> names;
    names.push_back( "Mercury" );
    names.push_back( "Venus" );
    names.push_back( "EarthMoonBarycenter" );
    names.push_back( "Mars" );
    names.push_back( "JupiterBarycenter" );
    names.push_back( "SaturnBarycenter" );
    names.push_back( "UranusBarycenter" );
    names.push_back( "NeptuneBarycenter" );
    names.push_back( "PlutoBarycenter" );
    names.push_back( "Moon" );
    names.push_back( "Sun" );

    std::vector<DE405Ephemeris::AstroEntity> handles;
    handles.push_back( DE405Ephemeris::Mercury );
    handles.push_back( DE405Ephemeris::Venus );
    handles.push_back( DE405Ephemeris::EarthMoonBarycenter );
    handles.push_back( DE405Ephemeris::Mars );
    handles.push_back( DE405Ephemeris::JupiterBarycenter );
    handles.push_back( DE405Ephemeris::SaturnBarycenter );
    handles.push_back( DE405Ephemeris::UranusBarycenter );
    handles.push_back( DE405Ephemeris::NeptuneBarycenter );
    handles.push_back( DE405Ephemeris::PlutoBarycenter );
    handles.push_back( DE405Ephemeris::Moon );
    handles.push_back( DE405Ephemeris::Sun );

    // Dump instructions
    std::cout << "Interactive interface for niek-ephem" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "Query format: [entity] [julian day]\n" << std::endl;
    std::cout << "Entities:" << std::endl;
    for ( int i = 0; i < names.size(); i++ )
        std::cout << "    " << names[i] << std::endl;
    std::cout << "====================================" << std::endl;

    // Holds results from an ephem call
    double pos[3];
    double vel[3];

    // Loop till we're terminated
    while ( 1 ) {
        try {
            DE405Ephemeris::AstroEntity ehandle;
            std::string ename;
            double eday;

            // Show sexy prompt
            std::cout << "---> ";

            // Grab name from user
            std::cin >> ename;
            bool foundName = false;
            for ( int i = 0; i < names.size(); i++ ) {
                if ( names[i] == ename ) {
                    ehandle = handles[i];
                    foundName = true;
                } 
            } 

            // Check for valid name
            if ( !foundName ) {
                std::cerr << "ERROR: Invalid name specified" << std::endl;
                std::cin.clear();
                continue;
            }

            // Grab julian day from user
            std::cin >> eday;
            if ( !std::cin ) {
                std::cerr << "ERROR: Invalid day specified" << std::endl;
                std::cin.clear();
                continue;
            }

            // Make query
            ephem.getPosVel( eday, ehandle, pos, vel );

            // Print results
            std::cout.setf( std::ios_base::scientific );
            std::cout << "Position: ";
            for ( int i = 0; i < 3; i++ ) {
                std::cout.precision( 12 );
                std::cout << pos[i] << "\t";
            }
            std::cout << std::endl;

            std::cout << "Velocity: ";
            for ( int i = 0; i < 3; i++ ) {
                std::cout.precision( 12 );
                std::cout << vel[i] << "\t";
            }
            std::cout << std::endl;
            std::cout << std::endl;

        }
        catch( std::exception& e ) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
