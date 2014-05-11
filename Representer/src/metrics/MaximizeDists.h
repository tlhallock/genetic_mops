/*
 * Maximize.h
 *
 *  Created on: May 6, 2014
 *      Author: rever
 */

#ifndef MAXIMIZE_H_
#define MAXIMIZE_H_

#include "RepresentationMetric.h"

class MaximizeDists : public RepresentationMetric
{
	int num_nearest;
	int *nearest;
	double *dists;
public:
	MaximizeDists(InitialSet *set, int num_closest);
	virtual ~MaximizeDists();

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* MAXIMIZE_H_ */
