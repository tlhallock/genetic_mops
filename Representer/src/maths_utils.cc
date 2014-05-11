/*
 * maths_utils.cc
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "math_utils.h"

#include <float.h>
#include <limits.h>
#include <math.h>


double l_2(double *p1, double *p2, int dim)
{
	double sum = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = p1[i] - p2[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}

double get_variance(InitialSet *set, char *to_use)
{
	int size = set->size();
	int index = 0;
	double *dists = (double *) alloca(sizeof(*dists) * size);

	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (!to_use[i])
		{
			continue;
		}
		double min = DBL_MAX;
		for (int j = 0; j < size; j++)
		{
			if (!to_use[j] || i == j)
			{
				continue;
			}
			double d = set->get_distance(i, j);
			if (d < min)
			{
				min = d;
			}
		}
		dists[index++] = min;
		sum += min;
	}

	double avg = sum / index;

	double sum2 = 0;
	for (int i = 0; i < index; i++)
	{
		double d = dists[i] - avg;
		sum2 += d * d;
	}
//	double std = sqrt(sum2);

	return sum2;
}


static int count(int size, char *set)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (set[i] == 0)
		{
			continue;
		}
		count++;
	}
	return count;
}


double get_deviance_cost(InitialSet *set, char *to_use, char *to_represent)
{
	int size = set->size();
	int index = 0;

	double variance = get_variance(set, to_represent);

	double expected_distance = count(set->size(), to_represent) * sqrt(variance) / count(set->size(), to_use);

	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (!to_use[i])
		{
			continue;
		}
		double min = DBL_MAX;
		for (int j = 0; j < size; j++)
		{
			if (!to_use[j] || i == j)
			{
				continue;
			}
			double d = set->get_distance(i, j);
			if (d < min)
			{
				min = d;
			}
		}

		double c = abs(expected_distance - min);
		sum += c;
	}

	double avg = sum / index;
	return avg;
}
