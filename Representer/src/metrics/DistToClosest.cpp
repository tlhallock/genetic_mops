/*
 * DistToClosest.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "DistToClosest.h"

#include <float.h>

DistToClosest::DistToClosest(InitialSet *set, int n_) :
		RepresentationMetric(set),
		num_closest(n_),
		nearest((int *) malloc(sizeof(*nearest) * n_)),
		dists((double *) malloc(sizeof(*dists) * n_))
{

}

DistToClosest::~DistToClosest()
{
	free(nearest);
	free(dists);
}

double DistToClosest::get_fitness(char *mask, char *to_represent, double *costs)
{
	double fitness = 0.0;

	for (int i = 0; i < set->size(); i++)
	{
		if (!to_represent[i])
		{
			continue;
		}
		if (mask[i])
		{
			continue;
		}

		set->get_n_nearest(i, num_closest, nearest, dists, mask);

		double sum = 0.0;
		for (int j = 0; j < num_closest; j++)
		{
			if (dists[j] == DBL_MAX)
			{
				for (int k = 0; k < set->size(); k++)
				{
					fputc(mask[k] ? '1' : '0', stdout);
				}
				fputc('\n', stdout);
				puts("Error 17099974297455552");
				exit(1);
			}
			sum += dists[j];
		}
		double cost = sum;

		fitness -= cost;

//		for (int j = 0; j < num_closest; j++)
//		{
			// add the cost to each of the points...
//		}
	}

	return fitness;
}
