/*
 * Variation.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Variation.h"
#include "math.h"

double Variation::get_fitness(char *mask, char *to_represent, double *costs)
{
	int size = set->size();

	double sum = 0.0;
	int count = 0;

	for (int i = 0; i < size; i++)
	{
		if (mask[i])
		{
			count++;
			continue;
		}

		int index;
		double dist;

		set->get_n_nearest(i, 1, &index, &dist, mask);

		sum += dist;
	}

	double avg = sum / count;

	sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (mask[i])
		{
			continue;
		}

		int index;
		double dist;

		set->get_n_nearest(i, 1, &index, &dist, mask);

		double dev = avg - dist;
		sum += dev * dev;
	}

	double ret_val = sqrt(sum) / avg;

	return -ret_val;
}
