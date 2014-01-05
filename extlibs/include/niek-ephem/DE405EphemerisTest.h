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
#ifndef DE405EPHEMERISTEST_H
#define DE405EPHEMERISTEST_H

#include <cmath>
#include <cppunit/extensions/HelperMacros.h>

class DE405Ephemeris;

class DE405EphemerisTest : public CppUnit::TestFixture {

        // Place all testcases in to suite
        CPPUNIT_TEST_SUITE( DE405EphemerisTest );
        CPPUNIT_TEST( testGetPosM1 );
        CPPUNIT_TEST( testGetVelM1 );
        CPPUNIT_TEST( testGetPosVelM1 );
        CPPUNIT_TEST( testGetPosVelM2 );
        CPPUNIT_TEST( testGetPosVelV1 );
        CPPUNIT_TEST( testGetPosVelV2 );
        CPPUNIT_TEST( testGetPosVelV3 );
        CPPUNIT_TEST( testGetPosVelEMB1 );
        CPPUNIT_TEST( testGetPosVelEMB2 );
        CPPUNIT_TEST( testGetPosVelEMB3 );
        CPPUNIT_TEST( testGetPosVelEMB4 );
        CPPUNIT_TEST( testGetPosVelMAR1 );
        CPPUNIT_TEST( testGetPosVelJ1 );
        CPPUNIT_TEST( testGetPosVelS1 );
        CPPUNIT_TEST( testGetPosVelU1 );
        CPPUNIT_TEST( testGetPosVelN1 );
        CPPUNIT_TEST( testGetPosVelP1 );
        CPPUNIT_TEST( testGetPosVelMOON1 );
        CPPUNIT_TEST( testGetPosVelMOON2 );
        CPPUNIT_TEST( testGetPosVelMOON3 );
        CPPUNIT_TEST( testGetPosVelMOON4 );
        CPPUNIT_TEST( testGetPosVelSUN1 );
        CPPUNIT_TEST( testGetPosVelSUN2 );
        CPPUNIT_TEST( testGetPosVelSUN3 );
        CPPUNIT_TEST( testGetPosVelSUN4 );
        CPPUNIT_TEST( testGetPosVelEXT1 );
        CPPUNIT_TEST( testGetPosVelEXT2 );
        CPPUNIT_TEST( testGetPosVelEXT3 );
        CPPUNIT_TEST( testGetPosVelEXT4 );
        CPPUNIT_TEST_SUITE_END();

    private:
        DE405Ephemeris* ephem_;
        double pos_[3];
        double vel_[3];

    public:
        // fixture start/end functs
        void setUp( void );
        void tearDown( void );

        // helper function for testing (compares two 3 element arrays)
        bool compareArrays( double* arr1, double* arr2, double threshold ) {
            bool result = true;
            for ( int i = 0; i < 3; i++ )
                result = result && 
                         ( std::fabs( arr1[i] - arr2[i] ) < threshold );
            return result;
        }

        // Test cases
        void testGetPosM1( void );
        void testGetVelM1( void );
        void testGetPosVelM1( void );
        void testGetPosVelM2( void );
        void testGetPosVelV1( void );
        void testGetPosVelV2( void );
        void testGetPosVelV3( void );
        void testGetPosVelEMB1( void );
        void testGetPosVelEMB2( void );
        void testGetPosVelEMB3( void );
        void testGetPosVelEMB4( void );
        void testGetPosVelMAR1( void );
        void testGetPosVelJ1( void );
        void testGetPosVelS1( void );
        void testGetPosVelU1( void );
        void testGetPosVelN1( void );
        void testGetPosVelP1( void );
        void testGetPosVelMOON1( void );
        void testGetPosVelMOON2( void );
        void testGetPosVelMOON3( void );
        void testGetPosVelMOON4( void );
        void testGetPosVelSUN1( void );
        void testGetPosVelSUN2( void );
        void testGetPosVelSUN3( void );
        void testGetPosVelSUN4( void );
        void testGetPosVelEXT1( void );
        void testGetPosVelEXT2( void );
        void testGetPosVelEXT3( void );
        void testGetPosVelEXT4( void );

};

#endif // DE405EPHEMERISTEST_H
