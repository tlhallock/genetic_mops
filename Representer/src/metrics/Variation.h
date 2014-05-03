/*
 * Variation.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef VARIATION_H_
#define VARIATION_H_

#include "RepresentationMetric.h"

class Variation : public RepresentationMetric
{
public:
	Variation() {}
	virtual ~Variation() {}

	double get_fitness(char *mask, char *to_represent, double *costs);
};

#endif /* VARIATION_H_ */
