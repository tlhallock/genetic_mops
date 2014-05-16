/*
 * DistCache.cpp
 *
 *  Created on: May 15, 2014
 *      Author: thallock
 */

#include "Representation.h"

#include "../common.h"

DistCache::DistCache(unsigned int cap_, int xdim_, int ydim_, double (*norm_)(double *, double *, int)) :
		xdim(ydim_),
		ydim(xdim_),
		cap(cap_),
		points(),
		distances((double **) malloc (sizeof(*distances) * cap_)),
		norm(norm_),
		accurate_dists(false),
		accurate_paretos(false)
{
	for (unsigned int i = 0; i < cap; i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * i);
	}
}

DistCache::~DistCache()
{
	for (std::vector<XYPair *>::iterator it = points.begin(); it != points.end(); it++)
	{
		delete *it;
	}
	for (unsigned int i = 0; i < cap; i++)
	{
		free(distances[i]);
	}
	free(distances);
}

void DistCache::ensure_dists()
{
	if (accurate_dists)
	{
		return;
	}
	unsigned int size = points.size();

	for (unsigned int i = 0; i < size; i++)
	{
		double *p1 = getY(i);
		for (unsigned int j = 0; j < i; j++)
		{
			distances[i][j] = norm(p1, getY(j), ydim);
		}
	}

	accurate_dists = true;
}

void DistCache::ensure_paretos()
{

	if (accurate_paretos)
	{
		return;
	}

	for (std::vector<XYPair *>::iterator i1 = points.begin(); i1 != points.end(); i1++)
	{
		(*i1)->pareto = true;

		for (std::vector<XYPair *>::iterator i2 = points.begin(); i2 != points.end(); i2++)
		{
			if (i1 == i2)
			{
				continue;
			}

			if (qtree::qtree_point_dominates((*i2)->y, (*i1)->y, ydim))
			{
				(*i1)->pareto = false;
				break;
			}
		}
	}

	accurate_paretos = true;
}


double DistCache::get_distance(int i, int j)
{
	if (i == j)
	{
		return 0.0;
	}

	ensure_dists();

	if (j > i)
	{
		int tmp = j;
		j = i;
		i = tmp;
	}

	return distances[i][j];
}

void DistCache::set_distance(int i, int j, double d)
{
	if (i == j)
	{
		puts("What!?! 250629877650276528765082754");
		break_die();
	}

	ensure_dists();

	if (j > i)
	{
		int tmp = j;
		j = i;
		i = tmp;
	}

	distances[i][j] = d;
}

int DistCache::index_of(double *yval)
{
	for (int i = 0; i < size(); i++)
	{
		if (qtree::qtree_point_equals(yval, getY(i), ydim))
		{
			continue;
		}
		return i;
	}
	return -194619;
}

void DistCache::add(double *x, double *y)
{
	int size = points.size();
	points.push_back(new XYPair(x, xdim, y, ydim));
	for (int i = 0; i < size; i++)
	{
		set_distance(i, size, norm(points.at(i)->y, points.at(size)->y, ydim));
	}
	accurate_paretos = false;
}

void DistCache::remove(int index)
{
	delete points.at(index);
	points.erase(points.begin() + index);
	accurate_dists = false;
	accurate_paretos = false;
}

void DistCache::clear_non_pareto()
{
	ensure_paretos();
	for (int i = points.size() - 1; i >= 0; i--)
	{
		if (!points.at(i)->pareto)
		{
			remove(i);
		}
	}
}
