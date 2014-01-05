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
#include <stdexcept>

#include "DE405EphemerisTest.h"
#include "../src/DE405Ephemeris.h"

// Places fixture in global registry
CPPUNIT_TEST_SUITE_REGISTRATION( DE405EphemerisTest );


/**
 * Create a reusable ephemeris querying object.
 */
void DE405EphemerisTest::setUp( void ) {

    ephem_ = new DE405Ephemeris( "/dirs/static/JPL_DE405/de405.data" ); 

}


/**
 * Clean up dynamically allocated memory.
 */
void DE405EphemerisTest::tearDown( void ) {

    delete ephem_;

}


/**
 * Test getPosition() against JPL Horizon's truth value:
 *
 *     Mercury on 2451536.5
 */
void DE405EphemerisTest::testGetPosM1( void ) {

    ephem_->getPosition( 2451536.5, DE405Ephemeris::Mercury, pos_ );

    // Truth values from JPL Horizons System
    CPPUNIT_ASSERT( std::fabs( -4.468523069712503E+07 - pos_[0] ) < 1E-3 );
    CPPUNIT_ASSERT( std::fabs( -4.796771237574679E+07 - pos_[1] ) < 1E-3 );
    CPPUNIT_ASSERT( std::fabs( -2.102675128947175E+07 - pos_[2] ) < 1E-3 );

}


/**
 * Test getVelocity() against JPL Horizon's truth value:
 *
 *     Mercury on 2451536.5
 */
void DE405EphemerisTest::testGetVelM1( void ) {

    ephem_->getVelocity( 2451536.5, DE405Ephemeris::Mercury, vel_ );

    // Truth values from JPL Horizons System
    CPPUNIT_ASSERT( std::fabs(  2.371166378484878E+06 - vel_[0] ) < 1E-3 );
    CPPUNIT_ASSERT( std::fabs( -2.136069981445719E+06 - vel_[1] ) < 1E-3 );
    CPPUNIT_ASSERT( std::fabs( -1.386759917744618E+06 - vel_[2] ) < 1E-3 );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Mercury on 2451536.5
 */
void DE405EphemerisTest::testGetPosVelM1( void ) {

    double posT[3] = { -4.468523069712503E+07,
                       -4.796771237574679E+07,
                       -2.102675128947175E+07 };
    double velT[3] = {  2.371166378484878E+06,
                       -2.136069981445719E+06,
                       -1.386759917744618E+06 };

    ephem_->getPosVel( 2451536.5, DE405Ephemeris::Mercury, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Mercury on 2451536.541666667
 */
void DE405EphemerisTest::testGetPosVelM2( void ) {

    double posT[3] = { -4.458631209613701E+07, 
                       -4.805658418653807E+07, 
                       -2.108447536726757E+07 };
    double velT[3] = {  2.376923584089826E+06,
                       -2.129775752514952E+06,
                       -1.383994884796426E+06 };

    ephem_->getPosVel( 2451536.541666667, DE405Ephemeris::Mercury, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Venus on 2451830.708333333
 */
void DE405EphemerisTest::testGetPosVelV1( void ) {

    double posT[3] = {  1.828113236438373E+07,
                       -9.789237420289995E+07,
                       -4.520935352033717E+07 };
    double velT[3] = {  2.959884955437216E+06,
                        5.412831720329996E+05,
                        5.625115666838966E+04 };

    ephem_->getPosVel( 2451830.708333333, DE405Ephemeris::Venus, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Venus on 2453916.500000000
 */
void DE405EphemerisTest::testGetPosVelV2( void ) {

    double posT[3] = {  1.017868548948139E+08,
                        3.778745048793542E+07,
                        1.056412777513236E+07 };
    double velT[3] = { -1.082962782641707E+06,
                        2.544394731634130E+06, 
                        1.213225527422473E+06 };

    ephem_->getPosVel( 2453916.500000000, DE405Ephemeris::Venus, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Venus on 2453916.895633333
 */
void DE405EphemerisTest::testGetPosVelV3( void ) {

    double posT[3] = {  1.013522509844873E+08,
                        3.879180856032939E+07,
                        1.104347931123298E+07 };
    double velT[3] = { -1.114019571792472E+06,
                        2.532770092584606E+06,
                        1.209960888124792E+06 };

    ephem_->getPosVel( 2453916.895633333, DE405Ephemeris::Venus, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2453912.500000000
 */
void DE405EphemerisTest::testGetPosVelEMB1( void ) {

    double posT[3] = {  1.160255162223222E+07,
                       -1.386945282427169E+08,
                       -6.014755936832554E+07 };
    double velT[3] = {  2.524363875445994E+06,
                        1.644257009959315E+05,
                        7.128625492841705E+04 };

    ephem_->getPosVel( 2453912.500000000, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2453912.541666667
 */
void DE405EphemerisTest::testGetPosVelEMB2( void ) {

    double posT[3] = {  1.170773072302304E+07,
                       -1.386876431433671E+08,
                       -6.014457435513011E+07 };
    double velT[3] = {  2.524232719184399E+06, 
                        1.660590504473829E+05,
                        7.199437093198286E+04 };

    ephem_->getPosVel( 2453912.541666667, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2453912.583333333
 */
void DE405EphemerisTest::testGetPosVelEMB3( void ) {

    double posT[3] = {  1.181290433168871E+07,
                       -1.386806899897597E+08,
                       -6.014155983795503E+07 };
    double velT[3] = {  2.524100328679700E+06,
                        1.676923101832697E+05,
                        7.270244804190191E+04 };

    ephem_->getPosVel( 2453912.583333333, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2453913.500000000
 */
void DE405EphemerisTest::testGetPosVelEMB4( void ) {

    double posT[3] = {  1.412523057774181E+07,
                       -1.385105107876492E+08,
                       -6.006777937999632E+07 };
    double velT[3] = {  2.520875630499936E+06,
                        2.035997982090421E+05,
                        8.826964188209367E+04 };

    ephem_->getPosVel( 2453913.500000000, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Mars on 2453917.770633333
 */
void DE405EphemerisTest::testGetPosVelMAR1( void ) {

    double posT[3] = { -2.311316059213092E+08,
                        8.153471464425707E+07,
                        4.362612596768249E+07 };
    double velT[3] = { -6.925642771902439E+05,
                       -1.613162588401733E+06,
                       -7.212299509973510E+05 };

    ephem_->getPosVel( 2453917.770633333, DE405Ephemeris::Mars, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Jupiter Barycenter on 2453917.482300000
 */
void DE405EphemerisTest::testGetPosVelJ1( void ) {

    double posT[3] = { -5.378329149993281E+08,
                       -5.596567366452917E+08,
                       -2.267963078430157E+08 };
    double velT[3] = {  8.288946753338146E+05,
                       -6.338959718226971E+05,
                       -2.918930727062295E+05 };

    ephem_->getPosVel( 2453917.482300000, 
                       DE405Ephemeris::JupiterBarycenter, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Saturn Barycenter on 2453857.648866666
 */
void DE405EphemerisTest::testGetPosVelS1( void ) {

    double posT[3] = { -8.992537078719262E+08,
                        9.356146822130651E+08,
                        4.251566818788330E+08 };
    double velT[3] = { -6.725503568321826E+05,
                       -5.205768820393867E+05,
                       -1.860668795541558E+05 };

    ephem_->getPosVel( 2453857.648866666, 
                       DE405Ephemeris::SaturnBarycenter, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Uranus Barycenter on 2451857.898866666
 */
void DE405EphemerisTest::testGetPosVelU1( void ) {

    double posT[3] = {  2.279492556746314E+09,
                       -1.753023761853392E+09,
                       -8.000209263429803E+08 };
    double velT[3] = {  3.754897151616818E+05,
                        3.884930074421310E+05,
                        1.648388367267812E+05 };

    ephem_->getPosVel( 2451857.898866666, 
                       DE405Ephemeris::UranusBarycenter, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Neptune Barycenter on 2459456.765933333
 */
void DE405EphemerisTest::testGetPosVelN1( void ) {

    double posT[3] = {  4.423862493317655E+09,
                       -5.790046107798476E+08,
                       -3.471269832106767E+08 };
    double velT[3] = {  6.713854408637992E+04,
                        4.330072062157345E+05,
                        1.755608191821165E+05 };

    ephem_->getPosVel( 2459456.765933333, 
                       DE405Ephemeris::NeptuneBarycenter, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against Java DE405 Interface truth value:
 *
 *     Pluto Barycenter on 2468456.531000000
 */
void DE405EphemerisTest::testGetPosVelP1( void ) {

    double posT[3] = {  5.2987572612258210E+09,
                       -1.9639250039214847E+09,
                       -2.2093763536666856E+09 };
    double velT[3] = {  2.4127671224431180E+05,
                        3.1649937836000810E+05,
                        2.6075400693330394E+04 };

    ephem_->getPosVel( 2468456.531000000, 
                       DE405Ephemeris::PlutoBarycenter, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Moon (geocenter) on 2445057.250000000
 */
void DE405EphemerisTest::testGetPosVelMOON1( void ) {

    double posT[3] = { -1.465596266918076E+08,
                       -1.791458391068867E+07,
                       -7.840577274711808E+06 };
    double velT[3] = {  2.430470074218781E+05,
                       -2.296654507289388E+06,
                       -9.910541646464415E+05 };

    ephem_->getPosVel( 2445057.250000000, DE405Ephemeris::Moon, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Moon (geocenter) on 2445057.254166667
 */
void DE405EphemerisTest::testGetPosVelMOON2( void ) {

    double posT[3] = { -1.465586137297902E+08,
                       -1.792415339904042E+07,
                       -7.844706692684352E+06 };
    double velT[3] = {  2.431747729212006E+05,
                       -2.296699908457866E+06,
                       -9.910664659881537E+05 };
                       

    ephem_->getPosVel( 2445057.254166667, DE405Ephemeris::Moon, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Moon (geocenter) on 2445057.291666667
 */
void DE405EphemerisTest::testGetPosVelMOON3( void ) {

    double posT[3] = { -1.465494730728933E+08,
                       -1.801028733141584E+07, 
                       -7.881873774488091E+06 };
    double velT[3] = {  2.443279016690873E+05,
                       -2.297110372215421E+06, 
                       -9.911782040252145E+05 };

    ephem_->getPosVel( 2445057.291666667, DE405Ephemeris::Moon, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Moon (geocenter) on 2445057.333333333
 */
void DE405EphemerisTest::testGetPosVelMOON4( void ) {

    double posT[3] = { -1.465392659327760E+08,
                       -1.810600983153014E+07,
                       -7.923175489817157E+06 };
    double velT[3] = {  2.456160361444108E+05,
                       -2.297570304409927E+06,
                       -9.913045068244624E+05 };

    ephem_->getPosVel( 2445057.333333333, DE405Ephemeris::Moon, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Sun on 2451631.704166667
 */
void DE405EphemerisTest::testGetPosVelSUN1( void ) {

    double posT[3] = { -9.931111717085646E+05,
                       -4.796363268433848E+05, 
                       -1.758213320162250E+05 };
    double velT[3] = {  9.143725166200063E+02,
                       -9.195022230007762E+02,
                       -4.177236549765552E+02 };

    ephem_->getPosVel( 2451631.704166667, DE405Ephemeris::Sun, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Sun on 2451631.710416667 
 */
void DE405EphemerisTest::testGetPosVelSUN2( void ) {

    double posT[3] = { -9.931054568540928E+05, 
                       -4.796420737117832E+05,
                       -1.758239427812621E+05 };
    double velT[3] = {  9.143809143798994E+02,
                       -9.194956644833652E+02,
                       -4.177211569016154E+02 };

    ephem_->getPosVel( 2451631.710416667, DE405Ephemeris::Sun, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Sun on 2451631.716666667
 */
void DE405EphemerisTest::testGetPosVelSUN3( void ) {

    double posT[3] = { -9.930997419471345E+05,
                       -4.796478205391905E+05,
                       -1.758265535306862E+05 };
    double velT[3] = {  9.143893122742664E+02,
                       -9.194891058335130E+02,
                       -4.177186587814555E+02 };

    ephem_->getPosVel( 2451631.716666667, DE405Ephemeris::Sun, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() against JPL Horizon's truth value:
 *
 *     Sun on 2451632.497916667
 */
void DE405EphemerisTest::testGetPosVelSUN4( void ) {

    double posT[3] = { -9.923849649679468E+05,
                       -4.803658508818501E+05,
                       -1.761527741601560E+05 };
    double velT[3] = {  9.154401012790977E+02,
                       -9.186682214233996E+02,
                       -4.174060322001227E+02 };

    ephem_->getPosVel( 2451632.497916667, DE405Ephemeris::Sun, pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() near DE405 extents against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2306776.791666667
 */
void DE405EphemerisTest::testGetPosVelEXT1( void ) {

    double posT[3] = {  1.365413078436597E+08,
                       -5.980324042233071E+07,
                       -2.601254089468594E+07 };
    double velT[3] = {  1.082322713948234E+06,
                        2.115024489688567E+06, 
                        9.193465610686667E+05 };

    ephem_->getPosVel( 2306776.791666667, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() near DE405 extents against JPL Horizon's truth value:
 *
 *     Earth-Moon Barycenter on 2524097.458333333
 */
void DE405EphemerisTest::testGetPosVelEXT2( void ) {

    double posT[3] = {  1.267930974753941E+08,
                       -7.394736328821346E+07,
                       -3.200699767011309E+07 };
    double velT[3] = {  1.340307058165027E+06,
                        1.981929141027588E+06,
                        8.581859016294822E+05 };

    ephem_->getPosVel( 2524097.458333333, DE405Ephemeris::EarthMoonBarycenter, 
                       pos_, vel_ );

    CPPUNIT_ASSERT( compareArrays( pos_, posT, 1E-3 ) );
    CPPUNIT_ASSERT( compareArrays( vel_, velT, 1E-3 ) );

}


/**
 * Test getPosVel() exception throwing once past DE405 extents.
 *
 */
void DE405EphemerisTest::testGetPosVelEXT3( void ) {

    CPPUNIT_ASSERT_THROW( ephem_->getPosVel( 2305424.4999, DE405Ephemeris::Sun, 
                                             pos_, vel_ ),
                          std::range_error );

}


/**
 * Test getPosVel() exception throwing once past DE405 extents.
 *
 */
void DE405EphemerisTest::testGetPosVelEXT4( void ) {

    CPPUNIT_ASSERT_THROW( ephem_->getPosVel( 2525040.5001, DE405Ephemeris::Sun, 
                                             pos_, vel_ ),
                          std::range_error );

}
