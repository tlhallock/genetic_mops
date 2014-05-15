/*
 * RepresentationBreeder.h
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#ifndef REPRESENTATIONBREEDER_H_
#define REPRESENTATIONBREEDER_H_

class RepresentationBreeder
{
	int pop_size;
	char **pop;
	std::vector<int> **indices;
	double *fitness;
	Representation *representation;

	void ensure_uses(int index, unsigned int num_to_use);
	void cross_over(int parent1, int parent2, int num_to_use);
	void mutate(int num_to_flip, int num_to_use, double *costs);
	void select(int generation);

	void print(int index);

public:
	RepresentationBreeder(Representation *rep, int _pop_size);
	~RepresentationBreeder();

	void represent(int num_points, char *mask_out);
};







#endif /* REPRESENTATIONBREEDER_H_ */
