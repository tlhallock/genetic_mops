/*
 * RandomSampleSolver.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: thallock
 */

#include "RandomSampleSolver.h"

#include <ctime>
#include <stdlib.h>
#include <stdio.h>

void RandomSampleSolver::solve(BoundedMopStats *board, int num_turns, long timeout)
{
	time_t start_time = time(0);

	int xdim = board->get_x_dimension();
	int ydim = board->get_y_dimension();

	double *x = (double *) malloc(sizeof (*x) * xdim);
	double *y = (double *) malloc(sizeof (*y) * ydim);

	while ((time(0) - start_time) < timeout
			&& board->get_num_points() < num_turns)
	{
		board->sample_feasible(x);
		board->make_guess(x, y);
	}

	free(x);
	free(y);
}
