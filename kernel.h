//////////////////////////////////
//		Kernel Library			//	
// 	Commonly used kernels		//
// 								//
// 	(C) 2017 Jiayao Zhang		//
// 		v0.1 27-Apr-2017		//
//////////////////////////////////
// kernel.h

#ifndef KERNEL_H
#define KERNEL_H

#include <cmath>
namespace kernel
{
	typedef double (*kernel_ptr)(unsigned, double*, double);

	double gauss(unsigned argc, double *argv, double val);

	double uniform(unsigned argc, double *argv, double val);

	double epanechnikov(unsigned argc, double *argv, double val);
}
#endif