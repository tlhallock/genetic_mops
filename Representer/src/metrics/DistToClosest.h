/*
 * DistToClosest.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef DISTTOCLOSEST_H_
#define DISTTOCLOSEST_H_

#include "RepresentationMetric.h"

class DistToClosest : public RepresentationMetric
{
	int num_closest;
public:
	DistToClosest(InitialSet *set, int n_);
	virtual ~DistToClosest() {}

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* DISTTOCLOSEST_H_ */
