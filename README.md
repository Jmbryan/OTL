OTL - Orbital Trajectory Library
================================
| Linux/Mac OSX                   | Windows |
| ------------------------------- | ------- |
| [![Build Status][master]][repo] |         |
[![codecov](https://codecov.io/gh/Jmbryan/OTL/branch/master/graph/badge.svg)](https://codecov.io/gh/Jmbryan/OTL)

[master]: https://travis-ci.org/Jmbryan/OTL.svg?branch=master
[repo]: https://travis-ci.org/Jmbryan/OTL

OTL is a simple, fast, cross-platform and object-oriented Orbital Mechanics library for general orbital trajectory computation.

This project is very much a work in progress. Feel free to fork and do with it as you like. The license is very liberal. Some of the design and configuration is inspired by [SFML](http://www.sfml-dev.org/).

Features
--------
- Cross-platform
- Clean, object-oriented, and well documented C++
- Epoch system supporting multiple formats (MJD2000, Gregorian, etc.)
- State vector & orbital elements conversions
- Lamberts Solver (Exponential Sinusoids)
- Propagation (Lagrangian with universal variable)
- Planetary Ephemeris
   - JPL approximate (analytical)
   - JPL SPICE
   - SGP4
   - MPCORB
- Flybys (unpowered)
- MATLAB interface

Build Instructions (Windows)
----------------------------
1. Get the repository. i.e. open a command prompt and type "git clone https://github.com/Jmbryan/OTL". These instructions assume you download the repository to "C:/OTL"
2. Download [CMake](https://cmake.org/download/) (last built using version 3.5.2 using "cmake-3.5.2-win32-x86.msi")
    - Note: Recommend adding CMake to system path (not done by default in installer)
3. Launch CMake dialog
4. Set source directory to root directory e.g. "C:/OTL"
5. Create a build directory e.g. "C:/OTL/build"
6. Set build directory created in step 5
7. Click Configure
8. Select compiler when prompted (last built using Visual Studio 14 2015)
9. Uncheck the following unless you downloaded the external dependencies (see below)
      - OTL_BUILD_SGP4
      - OTL_BUILD_SPICE
10. Click Generate
11. Open the generated Visual Studio solution file "C:/OTL/build/OTL.sln"
12. Build solution in Debug
13. The built static libraries can be found at "C:/OTL/build/lib".
14. Run the unit tests by opening a command prompt at "C:/OTL/build/bin" and typing "otl-unit-test-d"

Build Instructions (Mac OSX)
----------------------------

1. First, acquire the source code by cloning the git repository (these instructions assume you download the source to /Users/username/OTL):
```
git clone https::/github.com/Jmbryan/OTL.git
```
2. Download [CMake](https://cmake.org/download/) (last built using version 3.5.2 using "cmake-3.5.2-Darwin-x86_64.dmg")
3. Launch CMake dialog
4. Set source directory to root directory e.g. "/Users/username/OTL"
5. Create a build directory e.g. "/Users/username/OTL/build"
6. Set build directory created in step 5
7. Click Configure
8. Select compiler when prompted (last built using XCode 7.3)
9. Uncheck the following unless you downloaded the external dependencies (see below)
      - OTL_BUILD_SGP4
      - OTL_BUILD_SPICE
10. Click Generate
11. Open the generated XCode project "/Users/username/OTL/build/OTL.xcode"
12. Build solution in Debug
13. The built static libraries can be found at "/Users/username/OTL/build/lib".
14. Run the unit tests by opening a command prompt at "/Users/username/OTL/build/bin" and typing "otl-unit-test-d"

3rd Party Libraries (included)
------------------------------
- [Eigen](http://eigen.tuxfamily.org/) (v3.2.8)
- [Spdlog](https://github.com/gabime/spdlog)
- [niek-ephem](https://www.openhub.net/p/niek-ephem)
- [Catch](https://github.com/philsquared/Catch) (v1.5.6)
- [FakeIt](https://github.com/eranpeer/FakeIt) (v2.0.2)

3rd Party Libraries (not included / experimental)
-------------------------------------------------
- [CSPICE](https://naif.jpl.nasa.gov/naif/toolkit_C.html) (last tested using "PC, Windows, MS Visual C, 32bit")
- [SGP4](https://www.danrw.com/sgp4/) (source is located at the bottom of the page)

Authors
-------
- Jason Bryan - main developer (Jmbryan10@gmail.com)
