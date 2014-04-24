/*
 * GeneticRepresenter.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "GeneticRepresenter.h"

#include <stdlib.h>

#include <algorithm>


static bool vec_contains(std::vector<int> *vec, int i)
{
	return std::find(vec->begin(), vec->end(), i) != vec->end();
}

double GeneticRepresenter::evaluate(int i)
{

	return 0;
}

void GeneticRepresenter::cross_over(int num_to_bread, int parent1, int parent2, int num_to_use)
{
	int idx1 = rand() % set->size();
	int idx2;
	do
	{
		idx2 = rand() % set->size();
	} while (idx1 == idx2);

	if (idx2 < idx1)
	{
		int tmp = idx1;
		idx1 = idx2;
		idx2 = tmp;
	}

	indices[0]->clear();

	for (int i = 0; i < set->size(); i++)
	{
		char on;
		if (i < idx1 || i >= idx2)
		{
			on = pop[parent1][i];
		}
		else
		{
			on = pop[parent2][i];
		}

		if (!on)
		{
			pop[0][i] = 0;
			continue;
		}

		indices[0]->push_back(i);
		pop[0][i] = 1;
	}

	while (indices[0]->size() > num_to_use)
	{
		int to_del = rand() % indices[0]->size();
		// delete element at position to_del
	}

	while (indices[0]->size < num_to_use)
	{
		int to_add = rand() % set->size();
		if (vec_contains(indices[0], to_add))
		{
			continue;
		}
		indices[0]->push_back(to_add);
	}
}

void GeneticRepresenter::mutate(int num_to_flip, int num_to_use)
{
	for (int i = 0; i < num_to_flip; i++)
	{
		int idx = rand() % indices[0]->size();
		int turn_off = indices[0]->at(idx);
		int turn_on;
		do
		{
			turn_on = rand() % set->size();
		} while (vec_contains(indices[0], turn_off));

		indices[0][turn_off] = turn_on;
		pop[0][turn_off] = 0;
		pop[0][turn_on] = 1;
	}
}

double GeneticRepresenter::get_weight(int i)
{
	int num_to_average = 3;

	double *dists = (double *) malloc (sizeof (*dists) * num_to_average);
	int *indices = (int *) malloc (sizeof (*indices) * num_to_average);

	set->get_n_nearest(i, num_to_average, indices, dists);

	double sum = 0.0;
	for (int i = 0; i < num_to_average; i++)
	{
		sum += dists[i];
	}
	return sum / num_to_average;
}

GeneticRepresenter::GeneticRepresenter(int cap, int _pop_size) :
			size(0),
			capacity(cap),
			pop_size(_pop_size),
			pop((int **) malloc (sizeof(*pop) * (_pop_size + 1))),
			indices((std::vector<int> **) malloc (sizeof(*indices) * (_pop_size + 1))),
			weights((double *) malloc (sizeof(*weights) * cap)),
			set(NULL)
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		indices[i] = new std::vector<int>;
		pop[i] = (int *) malloc (sizeof (*pop[i]) * cap);
	}
}

GeneticRepresenter::~GeneticRepresenter()
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		delete indices[i];
		free(pop[i]);
	}
	free(pop);
	free(indices);
	free(weights);
}

void GeneticRepresenter::represent(InitialSet* set, int num_points)
{
}
