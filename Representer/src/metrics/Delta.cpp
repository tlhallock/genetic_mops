/*
 * Delta.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Delta.h"

Delta::Delta(InitialSet *set_, int num_closest_) :
		RepresentationMetric(set_), num_closest(num_closest_)
{
}

double Delta::get_fitness(char *mask, char *to_represent, double *costs)
{
	return 0;
}
