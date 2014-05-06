/*
 * GeneticRepresenter.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#ifndef GENETICREPRESENTER_H_
#define GENETICREPRESENTER_H_


#include "InitialSet.h"

#include "metrics/RepresentationMetric.h"

#include <vector>

void plot(const char *filename, InitialSet *set, char *mask, double fitness);

class GeneticRepresenter
{
private:
	int pop_size;
	char **pop;
	std::vector<int> **indices;
	double *fitness;

	char *msk_all;

	InitialSet *iset;
	void print(int index);

	double get_weight(int i);
	void cross_over(int parent1, int parent2, int num_to_use);
	void mutate(int num_to_flip, int num_to_use, double *costs);

	void select(int generation);
	void ensure_uses(int index, unsigned int num_to_use);
public:
	GeneticRepresenter(int cap, int pop_size);
	virtual ~GeneticRepresenter();

	void represent(int num_points, RepresentationMetric *metric, char *mask_out);
};

#endif /* GENETICREPRESENTER_H_ */
