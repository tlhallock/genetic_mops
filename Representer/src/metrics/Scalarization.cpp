/*
 * Scalarization.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Scalarization.h"

Scalarization::Scalarization(int num, RepresentationMetric *ms, double *weights_) :
	length(num),
	metrics(ms),
	weights(weights_) {}


double Scalarization::get_fitness(char *mask, char *to_represent, double *costs)
{

}
