#pragma once
#include <Catch/catch.hpp>
//#include <OTL/Test/fakeit_instance.h>
#include <fakeit.hpp>

const double TOL = 0.001;
#define OTL_APPROX(x) Approx(x).epsilon(TOL) // TODO determine tolerances as applicable per test