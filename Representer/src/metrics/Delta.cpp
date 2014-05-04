/*
 * Delta.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Delta.h"

#include <float.h>
#include <stdlib.h>

Delta::Delta(InitialSet *set_, int num_closest_) :
		RepresentationMetric(set_),
		num_closest(num_closest_),
		dists((double *) malloc(sizeof(*dists) * num_closest_)),
		indices((int *) malloc(sizeof(*indices) * num_closest_))
{
}

Delta::~Delta()
{
	free(dists);
	free(indices);
}

double Delta::get_fitness(char *mask, char *to_represent, double *costs)
{
	double max = -DBL_MAX;

	for (int i = 0; i < set->size(); i++)
	{
		set->get_n_nearest(i, num_closest, indices, dists, set->get_all_pnts());

		double sum = 0.0;
		for (int j = 0; j < num_closest; j++)
		{
			sum += dists[j];
		}

		double avg = sum / num_closest;
		costs[i] = avg;

		if (avg > max)
		{
			max = avg;
		}
	}

	return max;
}
