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

class Representation;

#define NUM_CLOSE_CACHE 5
class XYPair
{
	friend class Representation;

	double *x;
	double *y;
	int indices[NUM_CLOSE_CACHE];
	double dists[NUM_CLOSE_CACHE];

	bool pareto;
public:
	XYPair(double *x_, double *y_, int dim) :
			x(qtree::qtree_point_dup(dim, x_)),
			y(qtree::qtree_point_dup(dim, y_)),
			pareto(false)
	{
		for (int i = 0; i < NUM_CLOSE_CACHE; i++) {
			indices[i] = 0;
			dists[i] = 0;
		}
	};
	~XYPair()
	{
		if (x != NULL)
		{
			free(x);
		}
		free(y);
	}
};

class Representation
{
private:
	unsigned int cap;
	int dim;
	std::vector<XYPair *> points;
	double **distances;

	char *selected;
	std::set<int> indices;

	double (*norm)(double *, double *, int);

	bool accurate_dists;
	bool accurate_masks;

	void ensure_dists();
	void ensure_masks();
	bool is_pareto(int index);

	void calc_n_nearest(int index, int *nearest, double *dists);
	int get_nearest_index(int index);
public:
	Representation(unsigned int cap, int dim, double (*norm)(double *, double *, int));
	virtual ~Representation();

	void get_n_nearest(int index, int n, int *nearest, double *dists);

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

	void mark(int index, bool used);
	void add(double *x, double *y);
	void remove(int index);

	void clear_non_pareto();

	// In each of the following, num is ignored...
	double get_diversity_fitness(int num);
	double get_coverage_fitness (int num);

	bool dither_diversity_fitness(int index, int num, int num_alt, int *new_index, double *increase);
	bool dither_coverage_fitness (int index, int num, int num_alt, int *new_index, double *increase);
};

#endif /* INITIALSET_H_ */
