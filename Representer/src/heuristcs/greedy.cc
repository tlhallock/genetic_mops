/*
 * greedy.cc
 *
 *  Created on: May 6, 2014
 *      Author: thallock
 */

#include "../common.h"

#define ANOTHER_VERBOSE 0

#define NUM_CLOSE_TO_USE 5
static bool greedy_improve(RepresentationMetric *metric, std::vector<int> *indices, char *mask, int index)
{
	int point = indices->at(index);

	InitialSet *iset = metric->get_set();

	int nearest[NUM_CLOSE_TO_USE];
	double dists[NUM_CLOSE_TO_USE];
	double *costs = (double *) malloc(sizeof(*costs) * iset->size());

	iset->get_n_nearest(point, NUM_CLOSE_TO_USE, nearest, dists, iset->get_all_pnts());

	double cval = metric->get_fitness(mask, iset->get_all_pnts(), costs);

	if (ANOTHER_VERBOSE)
	{
		printf("fitness for %d is %lf\n", index, cval);
	}

	mask[point] = false;

	int min = -1;
	double maxVal = cval;
	for (int i = 0; i < NUM_CLOSE_TO_USE; i++)
	{
		if (mask[nearest[i]])
		{
			if (ANOTHER_VERBOSE)
			{
				printf("index %d is already used\n", nearest[i]);
			}
			continue;
		}

		mask[nearest[i]] = true;
		double val = metric->get_fitness(mask, iset->get_all_pnts(), costs);

		if (ANOTHER_VERBOSE)
		{
			printf("Alternate fitness: %lf\n", val);
		}
		if (val > maxVal)
		{
			min = i;
			maxVal = val;
		}
		mask[nearest[i]] = false;
	}

	bool ret_val;
	if (min < 0)
	{
		mask[point] = true;
		ret_val = false;
	}
	else
	{
		mask[nearest[min]] = true;
		indices->at(index) = nearest[min];
		ret_val = true;

		if (ANOTHER_VERBOSE)
		{
			printf("increased %d(%lf) -> %d(%lf)\n", point, cval, nearest[min], maxVal);
		}
	}

	free(costs);
	return ret_val;
}

void greedy_improve(RepresentationMetric *metric, std::vector<int> *indices, char *mask)
{
	int length = indices->size();
	int start_index = rand() % length;

	int index = start_index;
	bool changed = false;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < length; j++, index = (index + 1) % length)
		{
			changed |= greedy_improve(metric, indices, mask, j);
		}

		if (!changed)
		{
			break;
		}
		else
		{
			changed = false;
		}
	}
}
