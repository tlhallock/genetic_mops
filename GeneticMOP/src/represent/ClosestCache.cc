/*
 * ClosestCache.cpp
 *
 *  Created on: May 15, 2014
 *      Author: thallock
 */

#include "Representation.h"

#include "../common.h"

void ClosestCache::init()
{
	for (int i = 0; i < dcache->size(); i++)
	{
		if (points[i] != NULL)
		{
			if (points[i]->pair == dcache->get(i))
			{
				continue;
			}
			else
			{
				delete points[i];
			}
		}
		points[i] = new ClosestCacheEntry(dcache->get(i));
	}
	accurate_masks = false;
	accurate_dist = true;

	representation.clear();
}

ClosestCache::ClosestCache(DistCache *dcache_) :
		points((ClosestCacheEntry **) malloc(sizeof(*points) * dcache_->capacity())),
		accurate_masks(false),
		accurate_dist(false),
		dcache(dcache_),
		representation()
{
	for (int i = 0; i < dcache_->capacity(); i++)
	{
		points[i] = NULL;
	}
}

ClosestCache::~ClosestCache()
{
	for (int i = 0; i < dcache->capacity(); i++)
	{
		if (points[i] != NULL)
		{
			delete points[i];
		}
	}
	free(points);
}

void ClosestCache::ensure_masks()
{
	if (!accurate_dist)
	{
		init();
	}

	if (accurate_masks)
	{
		return;
	}

	int size = representation.size();
	for (int i = 0; i < size; i++)
	{
		calc_n_nearest(i, &points[i]->indices[0], &points[i]->dists[0]);
	}

	accurate_masks = true;
}

void ClosestCache::point_added()
{
	accurate_dist = false;
}

void ClosestCache::point_removed()
{
	accurate_dist = false;
}

void ClosestCache::get_n_nearest(int uindex, int n, int *nearest, double *dists)
{
	if (n > NUM_CLOSE_CACHE)
	{
		puts("Not cached. 105715638761938756913");
		break_die();
	}

	ensure_masks();

	ClosestCacheEntry *entry = points[uindex];
	for (int i = 0; i < n; i++)
	{
		dists[i] = entry->dists[i];
		nearest[i] = entry->indices[i];
	}
}

void ClosestCache::calc_n_nearest(int uindex, int *nearest, double *dists)
{
	unsigned int index = (unsigned int) uindex;
	for (int j = 0; j < NUM_CLOSE_CACHE; j++)
	{
		dists[j] = DBL_MAX;
		nearest[j] = -13000;
	}

	for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
	{
		unsigned int i = *it;

		if (i == index)
		{
			continue;
		}

		double dist = dcache->get_distance(index, i);
		if (dist >= dists[0])
		{
			continue;
		}
		nearest[NUM_CLOSE_CACHE-1] = i;
		dists[NUM_CLOSE_CACHE-1] = dist;

		for (int j = NUM_CLOSE_CACHE-1; j > 0; j--)
		{
			if (dists[j-1] <= dists[j])
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

bool ClosestCache::uses(int index)
{
	return representation.find(index) != representation.end();
}

int ClosestCache::get_nearest_index(int index)
{
	return points[index]->indices[0];
}

void ClosestCache::assign(std::set<int> *indices_)
{
	for (std::set<int>::iterator it = indices_->begin(); it != indices_->end(); it++)
	{
		printf("%d\t", *it);
	}
	putchar('\n');


	representation = *indices_;
	accurate_masks = false;
}

void ClosestCache::zero_improvements()
{
	int size = representation.size();
	for (int i = 0; i < size; i++)
	{
		points[i]->improvement = 0;
	}
}


void ClosestCache::mark(int index, bool used)
{
	std::set<int>::iterator it = representation.find(index);
	if (used)
	{
		if (it == representation.end())
		{
			representation.insert(index);
			points[index]->improvement = 0;
		}
	}
	else
	{
		if (it != representation.end())
		{
			representation.erase(it);
		}
	}

	accurate_masks = false;
}

















































//void ClosestCache::copy_out(std::vector<int> indices_, char *mask)
//{
//	int rsize = points.size();
//	for (int i = 0; i < rsize; i++)
//	{
//		mask[i] = 0;
//	}
//
//	for (std::set<int>::iterator it = indices.begin(); it != indices.end(); it++)
//	{
//		indices_.push_back(*it);
//		mask[*it] = 1;
//	}
//}


