/*
 * Epsilon.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Epsilon.h"

#include <float.h>

Epsilon::Epsilon(InitialSet *set, int n_) :
		RepresentationMetric(set),
		num_closest(n_),
		dists((double *) malloc (sizeof(*dists) * n_)),
		indices((int *) malloc (sizeof(*indices) * n_))
{
}

Epsilon::~Epsilon()
{
	free(dists);
	free(indices);
}

double Epsilon::get_fitness(char *mask, char *to_represent, double *costs)
{
	double max = -DBL_MAX;

	for (int i = 0; i < set->size(); i++)
	{
		set->get_n_nearest(i, num_closest, indices, dists, mask);

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

	return -max;
}
