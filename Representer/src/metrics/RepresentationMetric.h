/*
 * RepresentationMetric.h
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#ifndef REPRESENTATIONMETRIC_H_
#define REPRESENTATIONMETRIC_H_

#include "../InitialSet.h"

class RepresentationMetric
{
protected:
	InitialSet *set;
public:
	RepresentationMetric(InitialSet *set_) :
		set(set_) {}
	virtual ~RepresentationMetric() {}

	virtual double get_fitness(char *mask, char *to_represent, double *costs) = 0;

	InitialSet *get_set() { return set; }
};

#endif /* REPRESENTATIONMETRIC_H_ */
