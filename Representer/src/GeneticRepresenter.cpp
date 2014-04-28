/*
 * GeneticRepresenter.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "GeneticRepresenter.h"

#include <stdlib.h>

#include <algorithm>
#include <float.h>


static bool vec_contains(std::vector<int> *vec, int i)
{
	return std::find(vec->begin(), vec->end(), i) != vec->end();
}

double GeneticRepresenter::evaluate(double (*represent_metric)(InitialSet *set, char *mask), int i)
{
	return represent_metric(iset, pop[i]);
}

void GeneticRepresenter::ensure_uses(int index, unsigned int num_to_use)
{
	while (indices[index]->size() > num_to_use)
	{
		int to_del = rand() % indices[index]->size();
		pop[index][indices[index]->at(to_del)] = 0;
		indices[index]->erase(std::find(indices[index]->begin(), indices[index]->end(), to_del));
	}

	while (indices[index]->size() < num_to_use)
	{
		int to_add = rand() % iset->size();
		if (vec_contains(indices[index], to_add))
		{
			continue;
		}
		indices[index]->push_back(to_add);
	}
}

void GeneticRepresenter::cross_over(int parent1, int parent2, int num_to_use)
{
	int idx1 = rand() % iset->size();
	int idx2;
	do
	{
		idx2 = rand() % iset->size();
	} while (idx1 == idx2);

	if (idx2 < idx1)
	{
		int tmp = idx1;
		idx1 = idx2;
		idx2 = tmp;
	}

	indices[0]->clear();

	for (int i = 0; i < iset->size(); i++)
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

	ensure_uses(0, num_to_use);
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
			turn_on = rand() % iset->size();
		} while (vec_contains(indices[0], turn_off));

		indices[0]->at(turn_off) = turn_on;
		pop[0][turn_off] = 0;
		pop[0][turn_on] = 1;
	}
}

double GeneticRepresenter::get_weight(int i)
{
/*	int num_to_average = 3;

	double *dists = (double *) malloc (sizeof (*dists) * num_to_average);
	int *indices = (int *) malloc (sizeof (*indices) * num_to_average);

	set->get_n_nearest(i, num_to_average, indices, dists);

	double sum = 0.0;
	for (int i = 0; i < num_to_average; i++)
	{
		sum += dists[i];
	}
	return sum / num_to_average; */
	return 1;
}

GeneticRepresenter::GeneticRepresenter(int cap, int _pop_size) :
			capacity(cap),
			pop_size(_pop_size),
			pop((char **) malloc (sizeof(*pop) * (_pop_size + 1))),
			indices((std::vector<int> **) malloc (sizeof(*indices) * (_pop_size + 1))),
			fitness((double *) malloc (sizeof(*fitness) * (_pop_size + 1))),
//			weights((double *) malloc (sizeof(*weights) * cap)),
			iset(NULL)
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		indices[i] = new std::vector<int>;
		pop[i] = (char *) malloc (sizeof (*pop[i]) * cap);
	}

//	for (int i = 0; i < pop_size; i++)
//	{
//		weights[i] = get_weight(i);
//	}
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
//	free(weights);
	free(fitness);
}

void GeneticRepresenter::represent(InitialSet* set, int num_points, double (*represent_metric)(InitialSet *set, char *mask), char *mask_out)
{
	this->iset = set;

	for (int i = 0; i < 50; i++)
	{
		int p1 = rand() % pop_size;
		int p2;
		do
		{
			p2 = rand() % pop_size;
		} while (p1 == p2);

		printf("selecting %d and %d\n", p1, p2);

		print(0);

		cross_over(p1, p2, num_points);
		mutate(2, num_points);
		fitness[0] = evaluate(represent_metric, 0);
		select();
	}

	this->iset = NULL;
}


void GeneticRepresenter::select()
{
	int least_fit_index = -194714;
	double least_fitness = -DBL_MAX;

	for (int i = 0; i < pop_size + 1; i++)
	{
		if (fitness[i] < least_fitness)
		{
			least_fit_index = i;
			least_fitness = fitness[i];
		}
	}

	if (least_fit_index == 0)
	{
		return;
	}

	{
		std::vector<int> *tmp = indices[0];
		indices[0] = indices[least_fit_index];
		indices[least_fit_index] = tmp;
	}
	{
		char *tmp = pop[0];
		pop[0] = pop[least_fit_index];
		pop[least_fit_index] = tmp;
	}
}


void GeneticRepresenter::print(int index)
{
	for (int i = 0; i < pop_size; i++)
	{
		printf("%d: ", i);
		for (int j = 0; j < iset->size(); j++)
		{
			if(pop[i][j])
			{
				fputc('1', stdout);
			}
			else
			{
				fputc('0', stdout);
			}
		}
		fputc('\n', stdout);
	}

	fflush(stdout);
}
