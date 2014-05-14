/*
 * InitialSet.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "Representation.h"

#include "../common.h"

void Representation::ensure_dists()
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
			points[i]->pareto = true;
			double *p2 = getY(j);
			distances[i][j] = norm(p1, p2, dim);

			if (points[i]->pareto &&
					qtree::qtree_point_dominates(getY(j), getY(i), dim))
			{
				points[i]->pareto = false;
			}
		}
	}

	accurate_dists = true;
}

void Representation::ensure_masks()
{
	ensure_dists();

	if (accurate_masks)
	{
		return;
	}

	unsigned int size = points.size();

	for (unsigned int i = 0; i < size; i++)
	{
		selected[i] = false;
	}

	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		selected[*it] = true;
	}

	for (unsigned int i = 0; i < size; i++)
	{
		calc_n_nearest(i, &points.at(i)->indices[0], &points.at(i)->dists[0]);
	}

	accurate_masks = true;
}

Representation::Representation(unsigned int cap_, int dim_, double (*norm_)(double *, double *, int)) :
	cap(cap_),
	dim(dim_),
	points(),
	distances((double **) malloc(sizeof(*distances) * cap)),
	selected((char *) malloc(sizeof(*selected) * cap)),
	indices(),
	norm(norm_),
	accurate_dists(false),
	accurate_masks(false)
{
	for (unsigned int i = 0; i < cap; i++)
	{
		distances[i] = (double *) malloc(sizeof(*distances[i]) * i);
	}
}


Representation::~Representation()
{
	for (unsigned int i = 0; i < cap; i++)
	{
		delete points.at(i);
		free(distances[i]);
	}
	free(distances);
	free(selected);
}

void Representation::get_n_nearest(int uindex, int n, int *nearest, double *dists)
{
	if (n > NUM_CLOSE_CACHE)
	{
		puts("Not cached. 105715638761938756913");
		exit(1);
	}

	ensure_masks();

	XYPair *pair = points.at(uindex);
	for (int i = 0; i < n; i++)
	{
		dists[i] = pair->dists[i];
		nearest[i] = pair->indices[i];
	}
}

void Representation::calc_n_nearest(int uindex, int *nearest, double *dists)
{
	unsigned int index = (unsigned int) uindex;
	for (int j = 0; j < NUM_CLOSE_CACHE; j++)
	{
		dists[j] = DBL_MAX;
		nearest[j] = index;
	}

	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		unsigned int i = *it;

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

		for (int j = 1; j < NUM_CLOSE_CACHE; j++)
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

double Representation::get_distance(int i, int j)
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

int Representation::index_of(double *yval)
{
	for (int i = 0; i < size(); i++)
	{
		if (qtree::qtree_point_equals(yval, getY(i), get_dim()))
		{
			continue;
		}
		return i;
	}
	return -194619;
}

void Representation::mark(int index, bool used)
{
	std::set<int>::iterator it = indices.find(index);
	if (used)
	{
		if (it == indices.end())
		{
			indices.insert(index);
			points.at(index)->improvement = 0;
		}
	}
	else
	{
		if (it != indices.end())
		{
			indices.erase(it);
		}
	}

	accurate_dists = false;
}

void Representation::copy_out(std::vector<int> indices_, char *mask)
{
	int rsize = points.size();
	for (int i = 0; i < rsize; i++)
	{
		mask[i] = 0;
	}

	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		indices_.push_back(*it);
		mask[*it] = 1;
	}
}

void Representation::fill_from(std::vector<int> *indices_)
{
	indices.clear();
	for (std::vector<int>::iterator it = indices_->begin(); it != indices_->end(); it++)
	{
		indices.insert(*it);
	}
	accurate_masks = false;
}

void Representation::add(double *x, double *y)
{
	points.push_back(new XYPair(x, y, dim));
	selected[points.size() - 1] = false;
	accurate_dists = false;
}
void Representation::remove(int index)
{
	delete points.at(index);
	points.erase(points.begin() + index);
	accurate_dists = false;

	std::set<int>::iterator it = indices.find(index);
	if (it != indices.end())
	{
		accurate_masks = false;
		indices.erase(it);
	}
}

bool Representation::is_pareto(int index)
{
	return points.at(index)->pareto;
}

void Representation::clear_non_pareto()
{
	for (int i = points.size() - 1; i >= 0; i++)
	{
		if (!is_pareto(i))
		{
			remove(i);
		}
	}
}

int Representation::get_nearest_index(int index)
{
	return points.at(index)->indices[0];
}

void Representation::zero_improvements()
{
	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		points.at(*it)->improvement = 0;
	}
}
void Representation::greedy_improve_coverage(int num_loops)
{
	bool improvement;
	for (int i = 0; i < num_loops; i++)
	{
		improvement = false;
		for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
		{
			if (dither_coverage_fitness (*it, -1300, 3))
			{
				improvement = true;
			}
		}
		if (!improvement)
		{
			return;
		}
	}
}

void Representation::greedy_improve_diversity(int num_loops)
{
	bool improvement;
	for (int i = 0; i < num_loops; i++)
	{
		improvement = false;
		for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
		{
			if (dither_diversity_fitness (*it, -1300, 3))
			{
				improvement = true;
			}
		}
		if (!improvement)
		{
			return;
		}
	}
}


void Representation::ensure_uses(unsigned int num_to_use)
{
	while (indices.size() > num_to_use)
	{
		int to_del = rand() % indices.size();
		indices.erase(indices.begin() + to_del);
	}

	while (indices.size() < num_to_use)
	{
		int to_add = rand() % points.size();
		if (indices.find(to_add) != indices.end())
		{
			continue;
		}
		indices.insert(to_add);
	}
}

void Representation::clear()
{
	indices.clear();
	accurate_masks = false;
}

