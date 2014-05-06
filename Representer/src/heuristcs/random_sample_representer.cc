/*
 * random_sample_representer.cc
 *
 *  Created on: May 5, 2014
 *      Author: thallock
 */

#include "../common.h"

static void randomly_select(char *mask, int length, int num_to_use)
{
	for (int i = 0; i < length; i++)
	{
		mask[i] = 0;
	}

	for (int i = 0; i < num_to_use; i++)
	{
		int index;
		do
		{
			index = rand() % length;
		} while (mask[index]);
		mask[index] = 1;
	}
}

void random_sample(RepresentationMetric *metric, int num_points, char *mask_out, int num_samples)
{
	double fit_max = -DBL_MAX;

	InitialSet *set = metric->get_set();
	int length = set->size();

	char *tmp_mask = (char *) malloc(sizeof(*tmp_mask) * length);
	double *costs = (double *) malloc(sizeof(*costs) * num_points);

	for (int i = 0; i < num_samples; i++)
	{
		randomly_select(tmp_mask, length, num_points);
		double fitness = metric->get_fitness(tmp_mask, set->get_all_pnts(), costs);
		if (fitness > fit_max)
		{
			fit_max = fitness;
			for (int j = 0; j < length; j++)
			{
				mask_out[j] = tmp_mask[j];
			}
		}
	}

	free(tmp_mask);
	free(costs);
}
