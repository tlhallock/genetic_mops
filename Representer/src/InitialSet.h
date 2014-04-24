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
	int dim;
	std::vector<double *> points;
	double **distances;
public:
	InitialSet(int _size, int _dim, double (*norm)(double *, double *, int)) :
			dim(_dim), points(), distances((double **) malloc(sizeof(*distances) * _size))
	{
		for (int i = 0; i < _size; i++)
		{
			points.push_back(qtree::qtree_point_new_rand(dim));
			distances[i] = (double *) malloc(sizeof(*distances[i]) * _size);
		}
		for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _size; j++)
			{
				distances[i][j] = norm(get(i), get(j));
			}
		}
	}
	virtual ~InitialSet()
	{
		for (int i = 0; i < points.size(); i++)
		{
			qtree::qtree_point_del(points.at(i));
			free(distances[i]);
		}
		free(distances);
	}

	double get_n_nearest(int index, int n, int *nearest, double *dists);
	double *get(int index)
	{
		return points.at(index);
	}

	double get_distance(int i, int j);
	int size() { return points.size(); }
};

#endif /* INITIALSET_H_ */
