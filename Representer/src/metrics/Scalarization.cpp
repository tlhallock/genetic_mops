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
	weights(weights_) {

	for (unsigned int i = 0; i < metrics_->size(); i++)
	{
		fprintf(stdout, "metric %d = %p\n", i, metrics.at(i));
	}

}


double Scalarization::get_fitness(char *mask, char *to_represent, double *costs)
{
	double fitness = 0.0;
	int length = metrics.size();

	for (int i = 0; i < get_set()->size(); i++)
	{
		costs[i] = 0.0;
	}

	double *mycosts = (double *) alloca(sizeof(*mycosts) * get_set()->size());

	for (int i = 0; i < length; i++)
	{
		RepresentationMetric *m = metrics.at(i);
		double fitness = m->get_fitness(mask, to_represent, mycosts);
		fitness += weights[i] * fitness;
		for (int j = 0; j < length; j++)
		{
			costs[j] += mycosts[j];
		}
	}
	return fitness;
}
