/*
 * RepresentationBreeder.h
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#ifndef REPRESENTATIONBREEDER_H_
#define REPRESENTATIONBREEDER_H_

#include "Representation.h"

class RepresentationBreeder
{
	int cap;
	int pop_size;
	std::set<int> **indices;
	double *fitness;

	int current_size;

	void ensure_uses(int index, unsigned int num_to_use);
	void cross_over(int parent1, int parent2, int num_to_use);
	void mutate(int num_to_flip);
	void select(int generation);

	void print(int index);

public:
	RepresentationBreeder(int cap, int _pop_size);
	~RepresentationBreeder();

	void represent(int num_points, DistCache *dcache, std::set<int> *rep_out);
};







#endif /* REPRESENTATIONBREEDER_H_ */
