/*
 * Delta.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef DELTA_H_
#define DELTA_H_

#include "RepresentationMetric.h"

class Delta: public RepresentationMetric
{
private:
	int num_closest; // number of closest to average over...
	double *dists;
	int *indices;
public:
	Delta(InitialSet *set_, int num_closest_);
	virtual ~Delta();

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* DELTA_H_ */
