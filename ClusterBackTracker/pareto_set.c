/*
 * pareto_set.c
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include "pareto_header.h"
#include "memory_manager.h"

#include <float.h>

pareto_set *pareto_set_new(unsigned int capacity, unsigned int dim)
{
	pareto_set *ret = (pareto_set *) allocate(sizeof(*ret));

	ret->size = 0;
	ret->capacity = capacity;
	ret->dim = dim;
	ret->points = (double **) allocate(sizeof(*ret->points) * capacity);

	int i;
	for (i = 0; i < capacity; i++)
	{
		ret->points[i] = NULL;
	}

	return ret;
}
void pareto_set_del(pareto_set *set)
{
	int i;
	for (i = 0; i < set->size; i++)
	{
		destroy(set->points[i]);
	}
	destroy(set->points);
	destroy(set);
}

void pareto_set_add(pareto_set *set, double *point)
{
	if (set->size == set->capacity)
	{
		puts("full!");
		return;
	}

	set->points[set->size] = (double *) allocate(sizeof(*set->points[set->size]) * set->dim);
	int i;
	for (i = 0; i < set->dim; i++)
	{
		set->points[set->size][i] = point[i];
	}
	set->size++;
}

mask *pareto_set_mask_new(pareto_set *set)
{
	mask *ret = (mask *) allocate(sizeof(*ret));
	ret->size = set->size;
	ret->mask = (char *) allocate(sizeof(*ret->mask) * set->size);

	int i;
	for (i = 0; i < ret->size; i++)
	{
		ret->mask[i] = 1;
	}

	return ret;
}

void pareto_set_mask_del(mask *msk)
{
	destroy(msk->mask);
	destroy(msk);
}

void pareto_set_mask_on(mask *msk, int i, bool on)
{
	msk->mask[i] = on;
}

bool pareto_set_get_delta(dist_lookup *lookup, mask *msk, double *out_delta, int *p1, int *p2)
{
	bool ret = false;

	*out_delta = DBL_MAX;
	int i, j;
	for (i = 0; i < lookup->npoints; i++)
	{
		if (!msk->mask[i])
		{
			continue;
		}
		for (j = 0; j < i; j++)
		{
			if (!msk->mask[j])
			{
				continue;
			}

			double dist = pareto_set_distance_lookup_get(lookup, i, j);

			if (*out_delta > dist)
			{
				*out_delta = dist;
				*p1 = i;
				*p2 = j;

				ret = true;
			}
		}
	}

	return ret;
}

bool pareto_set_get_epsilon(dist_lookup *lookup, mask *msk, double *out_epsilon, int *out_p1, int *out_p2)
{
	bool ret = false;

	*out_epsilon = -DBL_MAX;
	int i, j;
	for (i = 0; i < lookup->npoints; i++)
	{
		if (!msk->mask[i])
		{
			continue;
		}

		double cmin = DBL_MAX;
		int ci = -1;
		int cj = -1;

		for (j = 0; j < i; j++)
		{
			if (!msk->mask[j])
			{
				continue;
			}

			double dist = pareto_set_distance_lookup_get(lookup, i, j);

			if (cmin > dist)
			{
				cmin = dist;
				ci = i;
				cj = j;
			}
		}

		if (cmin == DBL_MAX)
		{
			continue;
		}

		if (*out_epsilon < cmin)
		{
			*out_epsilon = cmin;
			*out_p1 = ci;
			*out_p2 = cj;

			ret = true;
		}
	}

	return ret;
}

dist_lookup *pareto_set_distance_lookup_new(pareto_set *set, double (*metric)(double *p1, double *p2))
{
	dist_lookup *ret = (dist_lookup *) allocate(sizeof(*ret));

	ret->npoints = set->size;
	ret->dim = set->dim;
	ret->distances = (double **) allocate (sizeof (*ret->distances) * ret->npoints);

	int i;
	for (i = 0; i < ret->npoints; i++)
	{
		ret->distances[i] = (double *) allocate(i * sizeof(*ret->distances[i]));
	}

	for (i = 0; i < set->size; i++)
	{
		int j;
		for (j = 0; j < i; j++)
		{
			pareto_set_distance_lookup_set(ret, i, j, metric(set->points[i], set->points[j]));
		}
	}

	return ret;
}

void pareto_set_distance_lookup_del(dist_lookup *lookup)
{
	int i;

	for (i = 0; i < lookup->npoints; i++)
	{
		destroy(lookup->distances[i]);
	}
	destroy(lookup->distances);
	destroy(lookup);
}

double pareto_set_distance_lookup_get(dist_lookup *lookup, int i, int j)
{
	if (i == j)
	{
		return 0;
	}

	if (i < j)
	{
		int tmp = i;
		i = j;
		j = tmp;
	}

	return lookup->distances[i][j];
}

void pareto_set_distance_lookup_set(dist_lookup *lookup, int i, int j, double value)
{
	if (i == j)
	{
		return;
	}

	if (i < j)
	{
		int tmp = i;
		i = j;
		j = tmp;
	}

	lookup->distances[i][j] = value;
}


pareto_set *pareto_set_read(FILE *in)
{
	int num;
	int dim;
	fscanf(in, "%d %d\n", &num, &dim);

	pareto_set *ret = pareto_set_new((unsigned int) num, (unsigned int) dim);

	double *point = (double *) allocate (sizeof (*point) * dim);

	int i;
	for (i=0;i<num;i++)
	{
		int j;
		for (j=0;j<dim;j++)
		{
			fscanf(in, "%lf", &point[j]);
		}
		pareto_set_add(ret, point);
	}

	destroy(point);

	return ret;
}
