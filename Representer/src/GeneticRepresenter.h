/*
 * GeneticRepresenter.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#ifndef GENETICREPRESENTER_H_
#define GENETICREPRESENTER_H_


#include "InitialSet.h"

#include <vector>

class GeneticRepresenter
{
private:
	int capacity;

	int pop_size;
	char **pop;
	std::vector<int> **indices;
	double *fitness;

//	double *weights;

	InitialSet *iset;

	void print(int index);

	double get_weight(int i);
	void cross_over(int parent1, int parent2, int num_to_use);
	void mutate(int num_to_flip, int num_to_use);

	double evaluate(double (*represent_metric)(InitialSet *set, char *mask), int i);
	void select();


	void ensure_uses(int index, unsigned int num_to_use);
public:
	GeneticRepresenter(int cap, int pop_size);
	virtual ~GeneticRepresenter();

	void represent(InitialSet *set, int num_points, double (*represent_metric)(InitialSet *set, char *mask), char *mask_out);
};

#endif /* GENETICREPRESENTER_H_ */
