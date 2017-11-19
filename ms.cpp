//////////////////////////////////
//          Mean Shift          //
//  For 2D points clustering    //
//                              //
//  (C) 2017 Jiayao Zhang       //
//      v0.1 27-Apr-2017        //
//////////////////////////////////
// ms.cpp
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <ctime>
#include <vector>
#include <ANN/ANN.h> // k-d tree and nearest neighbor
#include "p2d.h"
#include "kernel.h"

typedef std::vector< unsigned > vu;
typedef std::vector< p2d > vp2d;
typedef std::vector< vu* > lvp2d;
typedef vp2d::iterator vpitr;
typedef vp2d::const_iterator cvpitr;
typedef vu::const_iterator cvuitr;
typedef lvp2d::const_iterator clvpitr;

/* global parameters */
unsigned EPOCH = 25;	// num of iterations
double TOLERANCE = 0.01;// tolerance for convergence 
double MODE_PRUNE = 10;	// tolerance for pruning modes

/* global variables */
vp2d points;
lvp2d clusters;			
ANNpointArray ann_points;
ANNkd_tree* ann_tree;
unsigned global_bandwidth = 10;
unsigned epoch_stat = 0;
FILE *fout, *fin, *fcmd;

/* calculate mean shift vector */
double shift_vector(const p2d &point, p2d &msv, const vp2d &points, kernel::kernel_ptr kernel,
				unsigned kernel_argc, double* kernel_argv)
{
	double carry = 0;
	p2d res(0,0);
	double tmp = 0;

	/* search nearest neighbors */
	ANNidxArray ann_idx =  new ANNidx[global_bandwidth];
	ANNdistArray ann_dist = new ANNdist[global_bandwidth]; 
	ANNpoint ann_point = annAllocPt(2);
	ann_point[0] = point.x;
	ann_point[1] = point.y;
	ann_tree->annkSearch(ann_point, global_bandwidth, ann_idx, ann_dist);

	/* use avg. distance of nearest neighbors as local bandwidth */
	double width = 0;
	for(unsigned i = 0; i < global_bandwidth; ++i)
	{
		width += ann_dist[i]; 
	}

	width /= global_bandwidth;

	/* calculate the weighted sum */
	for(unsigned i = 0; i < points.size(); ++i)
	{
		const p2d &ptmp = points[i];
		tmp = kernel(kernel_argc, kernel_argv, (ptmp * point) / ( width * width));
		res += ptmp * tmp;
		carry += tmp;
	}

	/* store result */
	res /= carry;
	msv.set(res);

	/* clean up */
	delete ann_point;
	delete [] ann_idx;
	delete [] ann_dist;
	return msv * point;
}


/* determine number of clusters, i.e., connected compoenets */
void dfs(unsigned idx, int** adjMat, int* visited, vu& vp, const vp2d &points)
{
	for(unsigned i = 0; i < points.size(); ++i)
	{
		if(adjMat[idx][i])
		{
			if(!visited[i])
			{
				visited[i] = 1;
				vp.push_back(i);
				dfs(idx, adjMat, visited, vp, points);
			}
		}

	}
}

/* perform mean shift */
void mean_shift(const vp2d &points, vp2d &pr_modes, lvp2d &clusters, kernel::kernel_ptr kernel,
				unsigned kernel_argc, double* kernel_argv)
{
	unsigned N = points.size();

	// execute as least once

	vp2d modes;

	// main loop
	// FILE *ftmp = fopen("ms_log", "w");

	for(cvpitr itr = points.cbegin(); itr != points.cend(); ++itr)
	{
		unsigned counter = 0;
		double error = 	TOLERANCE + 1;
		p2d msv(itr->x, itr->y), tmp(msv);
		// fprintf(ftmp, "point: %lf,%lf\n", itr->x, itr->y);
		// fprintf(ftmp, "%u, %u, %lf, %lf\n", counter, EPOCH, error, TOLERANCE);
		while( (counter++) < EPOCH && (error > TOLERANCE || error < -TOLERANCE) )
		{
			tmp.set(msv);
			error = shift_vector(tmp, msv, points, kernel, kernel_argc, kernel_argv);
			//fprintf(ftmp, "point: %lf,%lf --> %lf,%lf, diff_norm: %lf ", itr->x, itr->y, msv.x, msv.y, (*itr) * msv);
			//fprintf(ftmp, "%lf\n", error);
			// printf("Check: %lf\n", error);
		}
		epoch_stat += counter;

		modes.push_back(msv);
	}
	// fclose(ftmp);
	// store modes to a graph
	int *visited = new int[N];
	int **adjMat = new int*[N];
	for(unsigned i = 0; i < N; ++i)
	{
		visited[i] = 0;
		adjMat[i] = new int[N];
		for(unsigned j = 0; j < N; ++j)
		{
			adjMat[i][j] = 0;
		}
	}

	// book-keeping the modes
	fout = fopen("log", "w");
	double MODE_PRUNE_SQ = MODE_PRUNE * MODE_PRUNE;
	for(unsigned i = 0; i < N; ++i)
	{
		fprintf(fout, "%lf %lf\n", modes[i].x, modes[i].y);

		for(unsigned j = i; j < N; ++j)
		{
			if(modes[i] * modes[j] < MODE_PRUNE_SQ )
			{
				adjMat[i][j] = adjMat[j][i] = 1;
			}
		}
	}
	fclose(fout);

	// prune modes and clustering points
	// accordingly
	for(unsigned i = 0; i < N; ++i)
	{
		if(!visited[i])
		{
			visited[i] = 1;
			vu *vp = new vu;
			vp->push_back(i);
			dfs(i, adjMat, visited, *vp, points);
			clusters.push_back(vp);
			pr_modes.push_back(modes[i]);
		}
	}

}

int main(int argc, char **argv)
{
	bool interactive = true;
	char script[64];
	script[0] = '\0';
	if(argc == 2) 
	{ 
		fcmd = fopen(argv[1], "r");
		if(fcmd) { interactive = false; }
	}


	/* read input file */
	char file[64];
	if(interactive)
	{
		printf("data file:\n> ");
		scanf("%s", file);
	} else { fscanf(fcmd, " %s ", file); }

	fin = fopen(file, "r");

	if(!fin)
	{
		printf("Error opening file\n");
		return 0;
	}

	p2d_type u, v;
	while( (fscanf(fin, "%lf%lf", &u, &v) == 2) )
	{
		points.push_back(p2d(u, v));
	}
	fclose(fin);

	printf("Data read successfully from \"%s\"\n", file);


	/* use ANN to build k-d tree */
	ann_points = annAllocPts(points.size(), 2);

	for(unsigned i = 0; i < points.size(); ++i)	
	{
		double *tmp = new double[2];
		*tmp = points[i].x;
		*(tmp + 1) = points[i].y;
		ann_points[i] = tmp;
	}

	ann_tree = new ANNkd_tree(ann_points, points.size(), 2);


	/* set kernel parameters */

	unsigned kernel_argc = 0;
	double *kernel_argv = nullptr;

	// default: gauss kernel
	char kernel_name[3][32] = {"Uniform", "Gauss", "Epanechnikov"};
	kernel::kernel_ptr kernel = kernel::gauss;

	unsigned choice = 0, kernel_choice = 0;
	if(interactive)
	{
		printf("Select kernel:\n\t[1] Uniform\n\t[2] Gauss\n\t[3] Epanechnikov\n\n> ");
		scanf("%u", &kernel_choice);
	} else { fscanf(fcmd, "%u", &kernel_choice);}

	switch(kernel_choice)
	{
		case 1:
			kernel = kernel::uniform;
			kernel_argc = 0;
			break;
		case 2:
			kernel = kernel::gauss;
			kernel_argc = 0;

/*			kernel_argv = new double[1];
			if(interactive)
			{
				printf("Input sigma:\n> ");
				scanf("%lf", kernel_argv);
			} else { fscanf(fcmd, "%lf", kernel_argv); }
*/
			break;

		case 3:
			kernel = kernel::epanechnikov;
			kernel_argc = 0;
			break;

		default:
			printf("invalid choice, exit\n");
			return 0;
			break;
	}

	/* set bandwidth (nearest neighbors to be examined */
	if(interactive)
	{
		printf("Input global bandwidth (possitive integer):\n> ");
		scanf("%u", &global_bandwidth);
		printf("\t[1] Default Paramters\n\t[2] Custom Parameters\n> ");
		scanf("%u", &choice);	
	} else { fscanf(fcmd, "%u%u", &global_bandwidth, &choice); }


	switch(choice)
	{
		case 2:
			if(interactive)
			{			
				printf("Maximum Iterations (Integer):\n> ");
				scanf("%u", &EPOCH);
				printf("Convergence Tolerance (Real):\n> ");
				scanf("%lf", &TOLERANCE);
				printf("Mode Pruning Tolerance (Real):\n> ");
				scanf("%lf", &MODE_PRUNE);
			} else {fscanf(fcmd, "%u%lf%lf", &EPOCH, &TOLERANCE, &MODE_PRUNE); }
			break;
		default:
			break;
	}

	/* perform mean shift */
	printf("Mean shift started with:\n\tKernel: %s\n\tBandwidth: %u\n\tMax. Iteration: %u\n\tConvergence Toleration: %.8f\n\tMode Pruning Diameter: %.3f\n", 
								kernel_name[kernel_choice-1], global_bandwidth, EPOCH, TOLERANCE, MODE_PRUNE);
	vp2d modes; lvp2d clusters;

	clock_t ms_time = clock();
	mean_shift(points, modes, clusters, kernel, kernel_argc, kernel_argv);
	ms_time = clock() - ms_time;

	// ms on modes
	// vp2d pnts;
	// for(clvpitr itr = clusters.cbegin(); itr != cluster.cend(); ++itr)
	// {
	// 	for(cvpitr it = (*itr)->cbegin(); it != (*itr)->cend(); ++it)
	// 	{
	// 		pnts.push_back(*it);
	// 	}
	// }
	// vp2d mds; lvp2d clst;
	// mean_shift(points, mds, clusters, kernel, kernel_argc, kernel_argv);


	/* post-process */
	printf("Clustering completed:\n\tClusters: %lu\n\tTime Elpased: %.3f sec\n\tAvg. Iteration: %.3f\n", 
			clusters.size(), (float)(ms_time) / CLOCKS_PER_SEC, double(epoch_stat) / points.size() - 1);
	if(interactive)
	{
		printf("\t[1] View Result and Exit\n\t[2] Save Result and Exit\n\t[3] Exit\n> ");
		scanf("%u", &choice);
	} else { fscanf(fcmd, "%u", &choice); }

	unsigned counter = 0;
	clvpitr itr = clusters.cbegin();
	cvpitr mitr = modes.cbegin();	

	switch(choice)
	{
		case 1:

			for(; itr != clusters.cend(); ++itr, ++mitr)
			{
				printf("Cluster %u, centered at (%5f,%5f):\n", ++counter, mitr->x, mitr->y);
				for(cvuitr it = (*itr)->cbegin(); it != (*itr)->cend(); ++it)
				{
					printf("(%5f,%5f)\n", points[*it].x, points[*it].y);
				}
			}
			break;
		case 2:
			for(; itr != clusters.cend(); ++itr, ++mitr)
			{
				char buffer[64];
				sprintf(buffer, "./cluster/cluster_%u", counter++);
				fout = fopen(buffer, "w");
				fprintf(fout, "x,y\n%lf,%lf\n", mitr->x, mitr->y); // prepend the mode before each cluster

				for(cvuitr it = (*itr)->cbegin(); it != (*itr)->cend(); ++it)
				{
					fprintf(fout, "%lf,%lf\n", points[*it].x, points[*it].y);
				}
				fclose(fout);
			}
			break;

		default:
			break;
	}

	fclose(fcmd);

	/* clean up */
	delete ann_tree;
	for(unsigned i = 0; i < points.size(); ++i)
	{
		delete ann_points[i];
	}
	delete [] ann_points;

	return 0;
}
