//////////////////////////////////
//          Kernel Library      //
//      Commonly used kernels   //
//                              //
//      (C) 2017 Jiayao Zhang   //
//          v0.1 27-Apr-2017    //
//////////////////////////////////
// kernel.cpp
#include <cassert>
#include <cmath>
#include "kernel.h"

double kernel::gauss(unsigned argc, double *argv, double val)
{
	assert(argc == 0);
	// double sigma = *argv;
	return exp( -0.5 * val );
}

double kernel::uniform(unsigned argc, double *argv, double val)
{
	assert(argc == 0);
	val = sqrt(fabs(val));
	return val <= 1.0 ? 1.0 : 0.0;
}

double kernel::epanechnikov(unsigned argc, double *argv, double val)
{
	assert(argc == 0);
	val = sqrt(fabs(val));
	return val <= 1.0 ? (0.75 * (1 - val)) : 0.0;
}
