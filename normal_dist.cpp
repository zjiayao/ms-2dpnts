//////////////////////////////////
//		Points Generator 		//	
// 	For generating 2D points 	//
// 								//
// 	(C) 2017 Jiayao Zhang		//
// 		v0.1 27-Apr-2017		//
//////////////////////////////////
// normal_dist.cpp

#include <cstdio>
#include <random>

int main()
{
	unsigned N = 0;
	double sigma = 0;
	double mu1 = 0, mu2 = 0;

	while( (scanf("%u %lf %lf %lf ", &N, &mu1, &mu2, &sigma) == 4) && N != -1)
	{  
		std::default_random_engine generator;
		std::normal_distribution<double> d1(mu1,sigma);
		std::normal_distribution<double> d2(mu2, sigma);
		for(unsigned i = 0; i < 2 * N; ++i)
		{
		  printf("%lf %lf\n", d1(generator), d2(generator));
		}
	}
  return 0;
}
