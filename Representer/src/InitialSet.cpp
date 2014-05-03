/*
 * InitialSet.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "InitialSet.h"

#include <float.h>

void InitialSet::get_n_nearest(int index, int n, int *nearest, double *dists, char *mask)
{
	for (int j = 0; j < n; j++)
	{
		dists[j] = DBL_MAX;
		nearest[j] = index;
	}

	for (int i = 0; i < points.size(); i++)
	{
		if (!mask[i])
		{
			continue;
		}

		if (i == index)
		{
			continue;
		}

		double dist = get_distance(index, i);
		if (dist >= dists[0])
		{
			continue;
		}
		nearest[0] = i;
		dists[0] = dist;

		for (int j = 1; j < n; j++)
		{
			if (dists[j-1] >= dists[j])
			{
				break;
			}

			int tmp = nearest[j];
			nearest[j] = nearest[j-1];
			nearest[j-1] = tmp;

			double td = dists[j];
			dists[j] = dists[j-1];
			dists[j-1] = td;
		}
	}
}

double InitialSet::get_distance(int i, int j)
{
	return distances[i][j];
}

int InitialSet::index_of(double *point)
{
	for (int i = 0; i < size(); i++)
	{
		if (qtree::qtree_point_equals(point, get(i), get_dim()))
		{
			continue;
		}
		return i;
	}
	return -194619;
}
