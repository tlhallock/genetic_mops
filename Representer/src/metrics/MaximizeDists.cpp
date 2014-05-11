/*
 * Maximize.cpp
 *
 *  Created on: May 6, 2014
 *      Author: rever
 */

#include "MaximizeDists.h"

#include <math.h>

MaximizeDists::MaximizeDists(InitialSet *set, int n_) :
		RepresentationMetric(set),
		num_nearest(n_),
		nearest((int *) malloc (sizeof(*nearest) *n_)),
		dists((double *) malloc (sizeof(*dists) * n_))
{

}

MaximizeDists::~MaximizeDists()
{
	free(nearest);
	free(dists);
}

double MaximizeDists::get_fitness(char *mask, char *to_represent, double *costs)
{
	int size = set->size();
	int index = 0;

	double sum = 0.0;
	for (int i = 0; i < size; i++)
	{
		if (!mask[i])
		{
			continue;
		}

		set->get_n_nearest(i, num_nearest, nearest, dists, mask);

		double isum = 0.0;
		for (int i = 0; i < num_nearest; i++)
		{
			isum += dists[i];
		}
		double avg = isum / num_nearest;
		costs[index++] = avg;
		sum += avg;
	}

	return log(sum);
}
