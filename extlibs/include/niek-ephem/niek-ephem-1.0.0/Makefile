################################################################################
##
## Copyright (c) 2007, Niek Sanders
## 
## All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
## 
##     * Redistributions of source code must retain the above copyright notice,
##       this list of conditions and the following disclaimer.
##     * Redistributions in binary form must reproduce the above copyright
##       notice, this list of conditions and the following disclaimer in the
##       documentation and/or other materials provided with the distribution.
##     * Neither the name of the Rochester Institute of Technology nor the names
##       of its contributors may be used to endorse or promote products derived
##       from this software without specific prior written permission.
## 
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
## LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
##
##
################################################################################
## niek-ephem
##
## This Makefile handles development for niek-ephem.  The following options are
## available:
##
##         make              - library, demo program, unit tester
##         make all          - library, demo program, unit tester
##         make clean        - removes all generated files
##         make doc          - html documentation
##         make ephem_demo   - demo program
##         make ephem_tester - unit tester
##
## Note that this Makefile searches for cpp files and builds accordingly.
## Adding cpp files to the source directories may have interesting effects.
################################################################################
SHELL = /bin/sh

# Compiler being used
CXX = g++

# Optional flags for compiler 
CXXFLAGS = -g 

# Flags needed for automatic dependency generation with compiler
#     -MM   for GNU's gcc and Intel' icc
#     -xM1  for Sun's CC
CXXDEPEND = -MM

# Path to CppUnit includes and libraries
CPPUNIT_INC = /dirs/pkg/cppunit/include
CPPUNIT_LIB = /dirs/pkg/cppunit/lib


################################################################################
## NO USER CONFIGURATION SETTINGS BELOW THIS POINT                            ##
################################################################################
.SUFFIXES:
.SUFFIXES: .cpp .h .o

VPATH = src:test:demo:bin

LIB_OBJS := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
TEST_OBJS := $(patsubst %.cpp,%.o,$(wildcard test/*.cpp))
DEMO_OBJS := $(patsubst %.cpp,%.o,$(wildcard demo/*.cpp))
SRCS := $(wildcard src/*.cpp) $(wildcard test/*.cpp) $(wildcard demo/*.cpp)

######
# Build everything
######
.PHONY: all
all: ephem_lib.a ephem_demo ephem_tester doc

######
# Assemble HTML documentation from source code using doxygen
######
.PHONY: doc
doc:
	doxygen doxygen.template

######
# Remove all generated files
######
.PHONY: clean
clean:
	-rm -f ./lib/ephem_lib.a ./bin/ephem_demo ./bin/ephem_tester ${LIB_OBJS} ${TEST_OBJS} ${DEMO_OBJS}
	-rm -rf ./doc/html

######
# Automatic dependency generation using C++ compiler
######
%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(CXXDEPEND) $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(SRCS:.cpp=.d)

######
# Build the Ephemeris (static) library
######
ephem_lib.a: $(LIB_OBJS)
	ar rcs ./lib/ephem_lib.a $(LIB_OBJS)

######
# Build the demonstration Ephemeris utility
######
ephem_demo: ephem_lib.a $(DEMO_OBJS)
	${CXX} ${CXXFLAGS} -o ./bin/ephem_demo $(DEMO_OBJS) ./lib/ephem_lib.a

######
# Build the Ephemeris unit testing code
######
ephem_tester: ephem_lib.a $(TEST_OBJS)
	${CXX} ${CXXFLAGS} -o ./bin/ephem_tester $(TEST_OBJS) ./lib/ephem_lib.a -L${CPPUNIT_LIB} -lcppunit -ldl

$(TEST_OBJS): %.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@ -I${CPPUNIT_INC}
