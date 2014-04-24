/*
 * GeneticRepresenter.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#ifndef GENETICREPRESENTER_H_
#define GENETICREPRESENTER_H_

class GeneticRepresenter
{
private:
	int size;
	int capacity;

	int pop_size;
	char **pop;
	std::vector<int> **indices;

	double *weights;

	InitialSet *set;

	void get_weight(int i);
	void cross_over();
	void mutate();

	double evaluate(int i);
public:
	GeneticRepresenter(int cap, int pop_size);
	virtual ~GeneticRepresenter();

	void represent(InitialSet *set, int num_points);
};

#endif /* GENETICREPRESENTER_H_ */
