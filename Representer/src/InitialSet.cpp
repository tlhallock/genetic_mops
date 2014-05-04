/*
 * InitialSet.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "InitialSet.h"

#include <float.h>

void InitialSet::init(double (*norm)(double *, double *, int))
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * points.size());
		all_pnts[i] = (char) 1;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
		double *p1 = get(i);
		for (unsigned int j = 0; j < points.size(); j++)
		{
			double *p2 = get(j);
			distances[i][j] = norm(p1, p2, dim);
		}
	}
}

InitialSet::InitialSet(int _size, int _dim, double (*norm)(double *, double *, int)) :
		dim(_dim),
		points(),
		distances((double **) malloc(sizeof(*distances) * _size)),
		all_pnts((char *) malloc(sizeof(*all_pnts) * _size))
{
	for (int i = 0; i < _size; i++)
	{
		points.push_back(qtree::qtree_point_new_rand(dim));
	}
	init(norm);
}


InitialSet::InitialSet(std::vector<double *> *pnts, int _dim, double (*norm)(double *, double *, int)) :
		dim(_dim),
		points(*pnts),
		distances((double **) malloc(sizeof(*distances) * pnts->size())),
		all_pnts((char *) malloc(sizeof(*all_pnts) * pnts->size()))
{
	init(norm);
}


InitialSet::~InitialSet()
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
		qtree::qtree_point_del(points.at(i));
		free(distances[i]);
	}
	free(distances);
	free(all_pnts);
}

void InitialSet::get_n_nearest(int index, int n, int *nearest, double *dists, char *mask)
{
	for (int j = 0; j < n; j++)
	{
		dists[j] = DBL_MAX;
		nearest[j] = index;
	}

	for (unsigned int i = 0; i < points.size(); i++)
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
