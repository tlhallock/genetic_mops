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

double Representation::get_diversity_fitness(int num)
{
	ensure_masks();

	double sum = 0;

	double dist;
	int nearest;

	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		get_n_nearest(*it, 1, &nearest, &dist);
		sum += dist;
	}

	return sum;
}

double Representation::get_coverage_fitness(int num)
{
	ensure_masks();

	double sum = 0;

	double dist;
	int nearest;

	unsigned int size = points.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (selected[i])
		{
			continue;
		}

		get_n_nearest(i, 1, &nearest, &dist);
		sum += dist;
	}

	return sum;
}

bool Representation::dither_diversity_fitness(int index, int num, int num_alt, int *new_index, double *increase)
{
	ensure_masks();

	if (num_alt > NUM_CLOSE_CACHE - 1)
	{
		puts("ERROR NOT CACHED 1324578173246591722295");
		exit(1);
	}

	int best_index = index;
	double improvement = 0;

	for (int i = 0; i < num_alt; i++)
	{
		int alternate = points.at(index)->indices[i];
		if (selected[alternate])
		{
			continue;
		}

		double sum = 0;
		double increase = 0.0;
		for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
		{
			int nearest = get_nearest_index(*it);
			if (nearest == index)
			{
				double new_nearest = get_distance(*it, alternate);
				if (points.at(*it)->dists[1] < new_nearest)
				{
					new_nearest = points.at(*it)->dists[1];
				}
				// if the nearest is the old one
				increase += new_nearest - get_distance(*it, nearest);
			}
			else if (get_distance(*it, nearest) > get_distance(*it, alternate))
			{
				// if the new one is closer than this one's closest
				sum += get_distance(*it, alternate) - get_distance(*it, nearest);
			}
		}

		if (sum > improvement)
		{
			best_index = alternate;
			improvement = sum;
		}
	}

	if (best_index != index)
	{
		return false;
	}

	mark(index, false);
	mark(best_index, true);

	return true;
}

bool Representation::dither_coverage_fitness(int index, int num, int num_alt, int *new_index, double *increase)
{
	ensure_masks();

	if (num_alt > NUM_CLOSE_CACHE - 1)
	{
		puts("ERROR NOT CACHED 66421641641");
		exit(1);
	}

	int best_index = index;
	double improvement = 0;

	for (int i = 0; i < num_alt; i++)
	{
		int alternate = points.at(index)->indices[i];
		if (selected[alternate])
		{
			continue;
		}

		double sum = 0;
		double increase = 0.0;
		for (unsigned int j = 0; j < points.size(); j++)
		{
			if (selected[j])
			{
				continue;
			}

			int nearest = get_nearest_index(j);
			if (nearest == index)
			{
				double new_nearest = get_distance(j, alternate);
				if (points.at(j)->dists[1] < new_nearest)
				{
					new_nearest = points.at(j)->dists[1];
				}
				// if the nearest is the old one
				increase += new_nearest - get_distance(j, nearest);
			}
			else if (get_distance(j, nearest) > get_distance(j, alternate))
			{
				// if the new one is closer than this one's closest
				sum += get_distance(j, alternate) - get_distance(j, nearest);
			}
		}

		if (sum < improvement)
		{
			best_index = alternate;
			improvement = sum;
		}
	}

	if (best_index != index)
	{
		return false;
	}

	mark(index, false);
	mark(best_index, true);

	return true;
}
