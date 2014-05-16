/*
 * InitialSet.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */
/*
 * InitialSet.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

#include <vector>
#include <set>
#include <stdlib.h>

#include "../../../Rtree/src/qtree_point.h"


class XYPair;
class DistCache;
class ClosestCacheEntry;
class ClosestCache;
class Representation;

class XYPair
{
	friend class DistCache;
	friend class ClosestCacheEntry;
	friend class ClosestCache;
	friend class Representation;

	double *x;
	double *y;

	bool pareto;
public:
	XYPair(double *x_, int xdim, double *y_, int ydim) :
			x(qtree::qtree_point_dup(xdim, x_)),
			y(qtree::qtree_point_dup(ydim, y_)),
			pareto(false) {};
	~XYPair()
	{
		free(x);
		free(y);
	}
};

class DistCache
{
	friend class XYPair;
	friend class ClosestCacheEntry;
	friend class ClosestCache;
	friend class Representation;
private:
	int xdim;
	int ydim;
	unsigned int cap;
	std::vector<XYPair *> points;
	double **distances;

	double (*norm)(double *, double *, int);

	bool accurate_dists;
	bool accurate_paretos;

	void ensure_paretos();
	void ensure_dists();
	void set_distance(int i, int j, double d);
public:
	DistCache(unsigned int cap, int xdim, int ydim, double (*norm)(double *, double *, int));
	virtual ~DistCache();

	double *getY(int index)
	{
		return points.at(index)->y;
	}
	double *getX(int index)
	{
		return points.at(index)->x;
	}
	XYPair *get(int index)
	{
		return points.at(index);
	}

	int capacity() { return cap; }

	double get_distance(int i, int j);
	int size() { return points.size(); }
	int get_xdim() { return xdim; }
	int get_ydim() { return ydim; }
	int index_of(double *point);

	void add(double *x, double *y);
	void remove(int index);
	void clear_non_pareto();
};

#define NUM_CLOSE_CACHE 4
class ClosestCacheEntry
{
	friend class XYPair;
	friend class DistCache;
	friend class ClosestCache;
	friend class Representation;
private:
	XYPair *pair;
	int indices[NUM_CLOSE_CACHE];
	double dists[NUM_CLOSE_CACHE];

	double improvement;
public:
	ClosestCacheEntry(XYPair *pair_) :
			pair(pair_),
			improvement(0)
	{
		for (int i = 0; i < NUM_CLOSE_CACHE; i++)
		{
			indices[i] = 0;
			dists[i] = 0;
		}
	};
	~ClosestCacheEntry() {}
};

class ClosestCache
{
	friend class XYPair;
	friend class DistCache;
	friend class ClosestCacheEntry;
	friend class Representation;
private:
	ClosestCacheEntry ** points;
	bool accurate_masks;
	bool accurate_dist;

	DistCache *dcache;
	std::set<int> representation;

	void ensure_masks();

	void init();
	bool uses(int index);
	int get_nearest_index(int index);
	void calc_n_nearest(int index, int *nearest, double *dists);
public:
	ClosestCache(DistCache *dcache);
	~ClosestCache();

	void mark(int index, bool used);
	void get_n_nearest(int index, int n, int *nearest, double *dists);

	void change_to(int on, int off);
	void assign(std::set<int> *mask);

	void point_added();
	void point_removed();

	void zero_improvements();
	void clear();


	// In each of the following, num is ignored...
	double get_diversity_fitness(int num);
	double get_coverage_fitness (int num);

	bool dither_diversity_fitness(int index, int num, int num_alt);
	bool dither_coverage_fitness (int index, int num, int num_alt);

	void greedy_improve_coverage(int num_loops);
	void greedy_improve_diversity(int num_loops);
};


class Representation
{
	friend class XYPair;
	friend class DistCache;
	friend class ClosestCacheEntry;
	friend class ClosestCache;
private:
	DistCache *dcache;
	ClosestCache *ccache;
public:
	Representation(DistCache *dcache, ClosestCache *cache);
	virtual ~Representation();


	DistCache *get_dist_cache();
	ClosestCache *get_closest_cache();
};

#endif /* INITIALSET_H_ */
