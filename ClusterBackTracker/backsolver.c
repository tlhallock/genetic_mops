/*
 * backsolver.c
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include "pareto_header.h"

#include <float.h>

#define DIM 2

#define MIN_POINTS 4
#define MAX_POINTS 20

typedef struct
{
	pareto_set *set;
	dist_lookup *lookup;
	mask *msk;
	int depth;
	double epsilon;
	double delta;
	int *masked;
	int masked_index;
} recursion_object;

double metric(double *p1, double *p2)
{
	double max = -DBL_MAX;

	bool unable = true;

	int i;
	for (i=0;i<DIM;i++)
	{
		double diff = p1[i] - p2[i];
		if (diff < 0)
		{
			diff = -diff;
		}
		if (diff > max)
		{
			max = diff;
			unable = false;
		}
	}

	if (unable)
	{
		puts("unable");
		exit(1);
	}

	return max;
}

static void print_current_bracktrace(mask *msk, int npoints)
{
	int j;
			for (j = 0; j < npoints; j++)
			{
				if (msk->mask[j])
				{
					putchar('1');
				} else
				{
					putchar('0');
				}
			}
			putchar('\n');
}

static void backtrack(recursion_object *r)
{
	int idx1, idx2;
	double epsilon = 0, delta = 0;

	// base case...
	if (r->depth >= r->set->size)
	{
		if (r->msk->size > MAX_POINTS)
		{
			return;
		}

		if (!pareto_set_get_delta(r->lookup, r->msk, &delta, &idx1, &idx2))
		{
			return; // too few points
		}
		if (!pareto_set_get_epsilon(r->lookup, r->msk, &epsilon, &idx1, &idx2))
		{
			return; // too few points
		}
	//			printf("d=%lf\n", delta);
	//			printf("e=%lf\n", epsilon);

		return;
	}
	if (!pareto_set_get_delta(r->lookup, r->msk, &delta, &idx1, &idx2))
	{
		return; // too few points
	}
	if (!pareto_set_get_epsilon(r->lookup, r->msk, &epsilon, &idx1, &idx2))
	{
		return; // too few points
	}

	// must satisfy the epsilon/delta contraints...

	// unable to continue if already fewer points than needed...
	if (r->msk->size < MIN_POINTS)
	{
		return;
	}

	if (!r->msk[r->depth])
	{
		r->depth++;
		backtrack(r);
		return;
	}


	// try with bit on
	r->depth++;
	backtrack(r);
	r->depth--;

	// try with bit off
	r->msk->size--;
	r->depth++;
	pareto_set_mask_on(r->msk, r->depth, 0);
	backtrack(r);
	r->depth--;
	r->msk->size++;
}

void pareto_set_backtrack(pareto_set *set, double delta, double epsilon)
{
	mask *msk = pareto_set_mask_new(set);
	dist_lookup *lookup = pareto_set_distance_lookup_new(set, &metric);

	recursion_object obj;
	obj.set = set;
	obj.depth = 0;
	obj.lookup = lookup;
	obj.msk = msk;
	obj.epsilon = epsilon;
	obj.delta = delta;
	obj.masked = (int *) malloc (sizeof (*obj.masked) * set->size);
	obj.masked_index = 0;

	backtrack(&obj);

	pareto_set_mask_del(msk);
	pareto_set_distance_lookup_del(lookup);
}
