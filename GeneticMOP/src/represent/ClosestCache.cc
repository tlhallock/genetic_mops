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
	if (!assigned)
	{
		puts("foo");
		break_die();
	}

	int dsize = dcache->size();
	for (int i = 0; i < dsize; i++)
	{
		points[i]->set(dcache->get(i));
	}

	accurate_selected_closest = false;
	accurate_all_closest = true;
	accurate_points = false;
}

ClosestCache::ClosestCache(DistCache *dcache_) :
		points((ClosestCacheEntry **) malloc(sizeof(*points) * dcache_->capacity())),
		accurate_selected_closest(false),
		accurate_all_closest(false),
		accurate_points(false),
		dcache(dcache_),
		representation()
{
	for (int i = 0; i < dcache_->capacity(); i++)
	{
		points[i] = new ClosestCacheEntry(NULL);
	}
}

ClosestCache::~ClosestCache()
{
	int dcap = dcache->capacity();
	for (int i = 0; i < dcap; i++)
	{
		if (points[i] != NULL)
		{
			delete points[i];
		}
	}
	free(points);
}

void ClosestCache::ensure_all_closest()
{

}

void ClosestCache::ensure_masks()
{
	if (!accurate_points)
	{
		init();
	}

	if (!accurate_all_closest)
	{
		ensure_all_closest();
	}

	if (accurate_selected_closest)
	{
		return;
	}

	int size = dcache->size();
	for (int i = 0; i < size; i++)
	{
		calc_n_nearest(i);
	}

	accurate_selected_closest = true;
}

void ClosestCache::point_added()
{
	accurate_points = false;
}

void ClosestCache::point_removed()
{
	accurate_points = false;
	representation.clear();
	assigned = false;
}

void ClosestCache::get_n_nearest(int index, int n, int *nearest, double *dists)
{
	if (n > NUM_CLOSE_CACHE)
	{
		puts("Not cached. 105715638761938756913");
		break_die();
	}

	ensure_masks();

	ClosestCacheEntry *entry = points[index];
	for (int i = 0; i < n; i++)
	{
		dists[i] = entry->s_dists[i];
		nearest[i] = entry->s_indices[i];
	}
}


void ClosestCache::cacl_n_nearest(int index, int other)
{
#define least_close_index (NUM_CLOSE_CACHE - 1)
	double dist = dcache->get_distance(index, other);
	if (dist >= points[index]->a_dists[least_close_index])
	{
		return;
	}

	points[index]->a_indices[least_close_index] = other;
	points[index]->a_dists[least_close_index] = dist;

	for (int j = least_close_index; j > 0; j--)
	{
		if (points[index]->a_dists[j - 1] <= points[index]->a_dists[j])
		{
			break;
		}

		int tmp = points[index]->a_indices[j];
		points[index]->a_indices[j] = points[index]->a_indices[j - 1];
		points[index]->a_indices[j - 1] = tmp;

		double td = points[index]->a_dists[j];
		points[index]->a_dists[j] = points[index]->s_dists[j - 1];
		points[index]->a_dists[j - 1] = td;
	}

	if (representation.find(other) == representation.end())
	{
		return;
	}

	points[index]->s_indices[least_close_index] = other;
	points[index]->s_dists[least_close_index] = dist;

	for (int j = least_close_index; j > 0; j--)
	{
		if (points[index]->s_dists[j - 1] <= points[index]->s_dists[j])
		{
			break;
		}

		int tmp = points[index]->s_indices[j];
		points[index]->s_indices[j] = points[index]->s_indices[j - 1];
		points[index]->s_indices[j - 1] = tmp;

		double td = points[index]->s_dists[j];
		points[index]->s_dists[j] = points[index]->s_dists[j - 1];
		points[index]->s_dists[j - 1] = td;
	}
}

void ClosestCache::calc_n_nearest(const int index)
{
	for (int j = 0; j < NUM_CLOSE_CACHE; j++)
	{
		points[index]->s_dists[j] = DBL_MAX;
		points[index]->s_indices[j] = -13000;
	}

	int end = use == NULL ? dcache->size() : use->size();
	for (std::set<int>::iterator it = representation.begin(); it != representation.end(); it++)
	{
		unsigned int i = *it;

		if (i == index)
		{
			continue;
		}

	}
}

bool ClosestCache::uses(int index)
{
	return representation.find(index) != representation.end();
}

int ClosestCache::get_nearest_index(int index)
{
	return points[index]->s_indices[0];
}

void ClosestCache::assign(std::set<int> *indices_)
{
	assigned = true;
	for (std::set<int>::iterator it = indices_->begin(); it != indices_->end(); it++)
	{
		printf("%d\t", *it);
	}
	putchar('\n');

	representation.clear();
	for (std::set<int>::iterator it = indices_->begin(); it != indices_->end(); it++)
	{
		representation.insert(*it);
	}
	accurate_masks = false;
}

void ClosestCache::zero_improvements()
{
	int size = dcache->size();
	for (int i = 0; i < size; i++)
	{
		points[i]->improvement = 0;
	}
}

void ClosestCache::mark(int index, bool used)
{
	std::set<int>::iterator it = representation.find(index);
	bool already_marked = it != representation.end();
	if (used == already_marked)
	{
		return;
	}

	if (already_marked)
	{
		representation.erase(it);
	}
	else
	{
		representation.insert(index);
		points[index]->improvement = 0;
	}

	accurate_masks = false;
}










void ClosestCacheEntry::set(XYPair *pair_)
{
	pair = pair_;
	improvement = 0;
	for (int j = 0; j < NUM_CLOSE_CACHE; j++)
	{
		s_dists[j] = DBL_MAX;
		s_indices[j] = -13000;
	}
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


