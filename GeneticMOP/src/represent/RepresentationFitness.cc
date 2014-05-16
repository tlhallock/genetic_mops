/*
 * RepresentationFitness.cc
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#include "Representation.h"

#include "../common.h"

void ClosestCache::greedy_improve_coverage(int num_loops)
{
	bool improvement;
	for (int i = 0; i < num_loops; i++)
	{
		improvement = false;
		for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
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

void ClosestCache::greedy_improve_diversity(int num_loops)
{
	bool improvement;
	for (int i = 0; i < num_loops; i++)
	{
		improvement = false;
		for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
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



double ClosestCache::get_diversity_fitness(int num)
{
	double sum = 0;

	double dist;
	int nearest;

	for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
	{
		get_n_nearest(*it, 1, &nearest, &dist);
		sum += dist;
	}

	return sum;
}

double ClosestCache::get_coverage_fitness(int num)
{
	ensure_masks();

	double sum = 0;

	double dist;
	int nearest;

	int size = representation.size();
	for (int i = 0; i < size; i++)
	{
		if (uses(i))
		{
			continue;
		}

		get_n_nearest(i, 1, &nearest, &dist);
		sum += dist;
	}

	return sum;
}

bool ClosestCache::dither_diversity_fitness(int index, int num, int num_alt)
{
	ensure_masks();

	if (num_alt > NUM_CLOSE_CACHE - 1)
	{
		puts("ERROR NOT CACHED 1324578173246591722295");
		break_die();
	}

	int best_index = index;
	double improvement = 0;

	for (int i = 0; i < num_alt; i++)
	{
		int alternate = points[index]->indices[i];
		if (uses(alternate))
		{
			continue;
		}

		double sum = 0;
		double increase = 0.0;
		for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
		{
			int nearest = get_nearest_index(*it);
			if (nearest == index)
			{
				double new_nearest = dcache->get_distance(*it, alternate);
				if (points[*it]->dists[1] < new_nearest)
				{
					new_nearest = points[*it]->dists[1];
				}
				// if the nearest is the old one
				increase += new_nearest - dcache->get_distance(*it, nearest);
			}
			else if (dcache->get_distance(*it, nearest) > dcache->get_distance(*it, alternate))
			{
				// if the new one is closer than this one's closest
				sum += dcache->get_distance(*it, alternate) - dcache->get_distance(*it, nearest);
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
	points[best_index]->improvement = improvement;

	return true;
}


bool ClosestCache::dither_coverage_fitness(int index, int num, int num_alt)
{
	ensure_masks();

	if (num_alt > NUM_CLOSE_CACHE - 1)
	{
		puts("ERROR NOT CACHED 66421641641");
		break_die();
	}

	int size = representation.size();

	int best_index = index;
	double improvement = 0;

	for (int i = 0; i < num_alt; i++)
	{
		int alternate = points[index]->indices[i];
		if (uses(alternate))
		{
			continue;
		}

		double sum = 0;
		double increase = 0.0;
		for (int j = 0; j < size; j++)
		{
			if (uses(j))
			{
				continue;
			}

			int nearest = get_nearest_index(j);
			if (nearest == index)
			{
				double new_nearest = dcache->get_distance(j, alternate);
				if (points[j]->dists[1] < new_nearest)
				{
					new_nearest = points[j]->dists[1];
				}
				// if the nearest is the old one
				increase += new_nearest - dcache->get_distance(j, nearest);
			}
			else if (dcache->get_distance(j, nearest) > dcache->get_distance(j, alternate))
			{
				// if the new one is closer than this one's closest
				sum += dcache->get_distance(j, alternate) - dcache->get_distance(j, nearest);
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

	points[best_index]->improvement = improvement;

	return true;
}
