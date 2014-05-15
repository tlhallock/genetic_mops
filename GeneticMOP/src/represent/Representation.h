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

class XYPair
{
	friend class Representation;

	double *x;
	double *y;

	bool pareto;
public:
	XYPair(double *x_, double *y_, int dim) :
			x(qtree::qtree_point_dup(dim, x_)),
			y(qtree::qtree_point_dup(dim, y_)),
			pareto(false) {};
	~XYPair()
	{
		free(x);
		free(y);
	}
};

class DistCache
{
private:
	int dim;
	unsigned int cap;
	std::vector<XYPair *> points;
	double **distances;

	double (*norm)(double *, double *, int);

	bool accurate_dists;

	void ensure_masks();
	bool is_pareto(int index);
	void set_distance(int i, int j, double d);
public:
	DistCache(unsigned int cap, int dim, double (*norm)(double *, double *, int));
	virtual ~DistCache();

	double *getY(int index)
	{
		return points.at(index)->y;
	}
	double *getX(int index)
	{
		return points.at(index)->x;
	}

	double get_distance(int i, int j);
	int size() { return points.size(); }
	int get_dim() { return dim; }
	int index_of(double *point);

	void add(double *x, double *y);
	void remove(int index);
	void clear_non_pareto();
};

#define NUM_CLOSE_CACHE 4
class ClosestCacheEntry
{
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
private:
	std::vector<ClosestCacheEntry *> points;
	bool accurate_masks;

	DistCache *dcache;
	std::set<int> representation;

	void ensure_masks();

	int get_nearest_index(int index);
	void calc_n_nearest(int index, int *nearest, double *dists);
public:
	ClosestCache(DistCache *dcache);
	~ClosestCache();

	void mark(int index, bool used);
	void get_n_nearest(int index, int n, int *nearest, double *dists);

	void change_to(int on, int off);
};


class Representation
{
public:
	Representation(ClosestCache *cache);
	virtual ~Representation();

	// In each of the following, num is ignored...
	double get_diversity_fitness(int num);
	double get_coverage_fitness (int num);

	bool dither_diversity_fitness(int index, int num, int num_alt);
	bool dither_coverage_fitness (int index, int num, int num_alt);

	void zero_improvements();
	void greedy_improve_coverage(int num_loops);
	void greedy_improve_diversity(int num_loops);
};

#endif /* INITIALSET_H_ */
