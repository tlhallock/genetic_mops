/*
 * Epsilon.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef EPSILON_H_
#define EPSILON_H_

#include "RepresentationMetric.h"

class Epsilon : public RepresentationMetric
{
	int num_closest;
	double *dists;
	int *indices;
public:
	Epsilon(InitialSet *set, int n_);
	virtual ~Epsilon();

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* EPSILON_H_ */
