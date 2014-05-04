/*
 * Scalarization.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Scalarization.h"


#include <alloca.h>

Scalarization::Scalarization(int num, RepresentationMetric **ms, double *weights_) :
	RepresentationMetric(ms[0]->get_set()),
	length(num),
	metrics(ms),
	weights(weights_) {}


double Scalarization::get_fitness(char *mask, char *to_represent, double *costs)
{
	double fitness = 0.0;

	double *mycosts = (double *) alloca(sizeof(*mycosts) * length);

	for (int i = 0; i < this->length; i++)
	{
		fitness += weights[i] * metrics[i]->get_fitness(mask, to_represent, costs);
		for (int j = 0; j < length; j++)
		{
			mycosts[j] += costs[j];
		}
	}
	return fitness;
}
