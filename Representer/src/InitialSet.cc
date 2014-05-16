/*
 * InitialSet.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "InitialSet.h"

#include <float.h>

void InitialSet::update_dists()
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
		double *p1 = get(i);
		for (unsigned int j = 0; j < points.size(); j++)
		{
			double *p2 = get(j);
			distances[i][j] = norm(p1, p2, dim);
		}
	}
	accurate_dists = true;
}

InitialSet::InitialSet(unsigned int _cap, std::vector<double *> *pnts, int _dim, double (*norm_)(double *, double *, int)) :
		cap(_cap),
		dim(_dim),
		points(),
		distances((double **) malloc(sizeof(*distances) * cap)),
		all_pnts((char *) malloc(sizeof(*all_pnts) * cap)),
		norm(norm_),
		accurate_dists(false)
{
	for (unsigned int i = 0; i < pnts->size(); i++)
	{
		points.push_back(qtree::qtree_point_dup(dim, pnts->at(i)));
	}
	for (unsigned int i = 0; i < cap; i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * cap);
		all_pnts[i] = (char) 1;
	}
}

InitialSet::InitialSet(std::vector<double *> *pnts, int _dim, double (*norm_)(double *, double *, int)) :
	cap(pnts->size()),
	dim(_dim),
	points(),
	distances((double **) malloc(sizeof(*distances) * cap)),
	all_pnts((char *) malloc(sizeof(*all_pnts) * cap)),
	norm(norm_),
	accurate_dists(false)
{
	for (unsigned int i = 0; i < pnts->size(); i++)
	{
		points.push_back(qtree::qtree_point_dup(dim, pnts->at(i)));
	}
	for (unsigned int i = 0; i < cap; i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * cap);
		all_pnts[i] = (char) 1;
	}
}

InitialSet::InitialSet(unsigned int cap_, int dim_, double (*norm_)(double *, double *, int)) :
	cap(cap_),
	dim(dim_),
	points(),
	distances((double **) malloc(sizeof(*distances) * cap)),
	all_pnts((char *) malloc(sizeof(*all_pnts) * cap)),
	norm(norm_),
	accurate_dists(false)
{
	for (unsigned int i = 0; i < cap; i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * cap);
		all_pnts[i] = (char) 1;
	}
}


InitialSet::~InitialSet()
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
		qtree::qtree_point_del(points.at(i));
		free(distances[i]);
	}
	for (unsigned int i = 0; i < cap; i++)
	{
		qtree::qtree_point_del(points.at(i));
		free(distances[i]);
	}
	free(distances);
	free(all_pnts);
}

void InitialSet::get_n_nearest(int uindex, int n, int *nearest, double *dists, char *mask)
{
	unsigned int index = (unsigned int) uindex;
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
	if (!accurate_dists)
	{
		update_dists();
	}
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

void InitialSet::add(double *pnt)
{
	points.push_back(qtree::qtree_point_dup(dim, pnt));
	accurate_dists = false;
}
void InitialSet::remove(int index)
{
	double *pnt = get(index);
	points.erase(points.begin() + index);
	qtree::qtree_point_del(pnt);
	accurate_dists = false;
}

bool InitialSet::is_pareto(int index)
{
	double *pnt = get(index);
	int size = points.size();

	for (int i = 0; i < size; i++)
	{
		if (i == index)
		{
			continue;
		}
		if (qtree::qtree_point_dominates(get(i), pnt, dim))
		{
			return false;
		}
	}
	return true;
}

void InitialSet::clear_non_pareto()
{
	for (int i = points.size() - 1; i >= 0; i++)
	{
		if (!is_pareto(i))
		{
			remove(i);
		}
	}
}
