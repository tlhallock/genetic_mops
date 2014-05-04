/*
 * InitialSet.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#ifndef INITIALSET_H_
#define INITIALSET_H_

#include <vector>
#include <stdlib.h>

#include "../../Rtree/src/qtree_point.h"

class InitialSet
{
private:
	int dim;
	std::vector<double *> points;
	double **distances;
	char *all_pnts;

	void init(double (*norm)(double *, double *, int));
public:
	InitialSet(std::vector<double *> *pnts, int dim, double (*norm)(double *, double *, int));
	InitialSet(int size, int dim, double (*norm)(double *, double *, int));

	virtual ~InitialSet();

	void get_n_nearest(int index, int n, int *nearest, double *dists, char *mask);

	double *get(int index)
	{
		return points.at(index);
	}

	double get_distance(int i, int j);
	int size() { return points.size(); }
	int get_dim() { return dim; }
	int index_of(double *point);
	char *get_all_pnts() { return all_pnts; }
};

#endif /* INITIALSET_H_ */
