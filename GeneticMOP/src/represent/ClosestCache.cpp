/*
 * ClosestCache.cpp
 *
 *  Created on: May 15, 2014
 *      Author: thallock
 */

#include "Representation.h"

#include "../common.h"

ClosestCache::ClosestCache(DistCache *dcache_) :
		size(dcache_->size()),
		points((ClosestCacheEntry **) malloc(sizeof(*points) * size)),
		accurate_masks(false),
		dcache(dcache_),
		representation()
{
	for (int i = 0; i < size; i++)
	{
		points[i] = new ClosestCacheEntry(dcache_->get(i));
	}
}


ClosestCache::~ClosestCache()
{

}

void ClosestCache::ensure_masks()
{
	if (accurate_masks)
	{
		return;
	}

	for (int i = 0; i < size; i++)
	{
		calc_n_nearest(i, &points[i]->indices[0], &points[i]->dists[0]);
	}

	accurate_masks = true;
}


void ClosestCache::get_n_nearest(int uindex, int n, int *nearest, double *dists)
{
	if (n > NUM_CLOSE_CACHE)
	{
		puts("Not cached. 105715638761938756913");
		exit(1);
	}

	ensure_masks();

	ClosestCacheEntry *pair = points[uindex];
	for (int i = 0; i < n; i++)
	{
		dists[i] = pair->dists[i];
		nearest[i] = pair->indices[i];
	}
}

void ClosestCache::calc_n_nearest(int uindex, int *nearest, double *dists)
{
	unsigned int index = (unsigned int) uindex;
	for (int j = 0; j < NUM_CLOSE_CACHE; j++)
	{
		dists[j] = DBL_MAX;
		nearest[j] = index;
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
	representation = *indices_;
	accurate_masks = false;
}

void ClosestCache::zero_improvements()
{
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


