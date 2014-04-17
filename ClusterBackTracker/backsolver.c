/*
 * backsolver.c
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include "pareto_header.h"

#include <float.h>

//static int find_min(dist_lookup *lookup, mask *msk, int index)
//{
//	int i;
//
//	int ret = -1;
//	double min = DBL_MAX;
//
//	for (i = 0; i < lookup->npoints; i++)
//	{
//		double dist = pareto_set_distance_lookup_get(lookup, index, i);
//		if (dist < min)
//		{
//			min = dist;
//			ret = i;
//		}
//	}
//
//	return min;
//}

#define DIM 2

double metric(double *p1, double *p2)
{
	double max = -DBL_MAX;

	int i;
	for (i=0;i<DIM;i++)
	{
		double diff = p1[i] - p2[i];
		if (diff < 0)
		{
			if (diff < max)
			{
				max = -diff;
			}
		}
		else
		{
			if (diff > max)
			{
				max = diff;
			}
		}
	}

	return max;
}

static void backtrack(pareto_set *set, dist_lookup *lookup, mask *msk)
{
	int idx1, idx2;
	double epsilon, delta;

	pareto_set_get_delta(lookup, msk, &delta, &idx1, &idx2);
	pareto_set_get_epsilon(lookup, msk, &epsilon, &idx1, &idx2);

	printf("%lf %lf\n", epsilon, delta);

	if (msk->size == 0)
	{
		return;
	}

	int i;
	for (i = 0; i < set->size; i++)
	{
		if (!msk->mask[i])
		{
			continue;
		}

		int j;
		for (j=0;j<lookup->npoints;j++)
		{
			if (msk->mask[j])
			{
				putchar('1');
			}
			else
			{
				putchar('0');
			}
		}
		putchar('\n');

		pareto_set_mask_on(msk, i, 0);
		msk->size--;


		backtrack(set, lookup, msk);

		msk->size++;
		pareto_set_mask_on(msk, i, 1);
	}
}

void pareto_set_backtrack(pareto_set *set)
{
	mask *msk = pareto_set_mask_new(set);
	dist_lookup *lookup = pareto_set_distance_lookup_new(set, &metric);

	backtrack(set, lookup, msk);

	pareto_set_mask_del(msk);
	pareto_set_distance_lookup_del(lookup);
}
