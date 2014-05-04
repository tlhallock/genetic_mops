/*
 * Scalarization.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef SCALARIZATION_H_
#define SCALARIZATION_H_

#include "RepresentationMetric.h"

class Scalarization : public RepresentationMetric
{
	int length;
	RepresentationMetric **metrics;
	double *weights;
public:
	Scalarization(int num, RepresentationMetric **ms, double *weights);
	virtual ~Scalarization() {}

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* SCALARIZATION_H_ */
