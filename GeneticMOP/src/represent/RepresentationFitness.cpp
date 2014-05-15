/*
 * RepresentationFitness.cc
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#include "Representation.h"

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

bool Representation::dither_diversity_fitness(int index, int num, int num_alt)
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
	points.at(best_index)->improvement = improvement;

	return true;
}


bool Representation::dither_coverage_fitness(int index, int num, int num_alt)
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

	points.at(best_index)->improvement = improvement;

	return true;
}
