/*
 * DistToClosest.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "DistToClosest.h"

DistToClosest::DistToClosest(InitialSet *set, int n_) :
	RepresentationMetric(set),
	num_closest(n_)
{

}

double DistToClosest::get_fitness(char *mask, char *to_represent, double *costs)
{

}
