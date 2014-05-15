/*
 * compare_dfs.cc
 *
 *  Created on: May 13, 2014
 *      Author: rever
 */


#include "../common.h"
#include "../image/VectorImageSet.h"
#include "../solver/DepthFirstRecursiveStencil.h"



void compare_dfs()
{
	VectorImageSet image(2);

	// unit square
	double xmin[2];
	xmin[0] = 0;
	xmin[1] = 0;
	double xmax[2];
	xmax[0] = 1;
	xmax[1] = 1;

	BoundedMopStats board(2, 2, (double *) &xmin, (double *) &xmax, &image, &objective_function_4, &is_feasible_function_2);

	DepthFirstRecursiveStencil solver(.01, &l_inf, 7, 3);
	solver.solve(&board);
}
