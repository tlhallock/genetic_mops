/*
 * Scalarization.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Scalarization.h"


#include <alloca.h>

Scalarization::Scalarization(std::vector<RepresentationMetric *> *metrics_, double *weights_) :
	RepresentationMetric(metrics_->at(0)->get_set()),
	metrics(*metrics_),
	weights(weights_) {}


double Scalarization::get_fitness(char *mask, char *to_represent, double *costs)
{
	double fitness = 0.0;
	int length = metrics.size();

	double *mycosts = (double *) alloca(sizeof(*mycosts) * length);

	for (int i = 0; i < length; i++)
	{
		fitness += weights[i] * metrics[i]->get_fitness(mask, to_represent, costs);
		for (int j = 0; j < length; j++)
		{
			mycosts[j] += costs[j];
		}
	}
	return fitness;
}
