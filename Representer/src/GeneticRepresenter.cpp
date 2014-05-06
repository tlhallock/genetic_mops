/*
 * GeneticRepresenter.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rever
 */

#include "GeneticRepresenter.h"
#include "common.h"

#define GA_VERBOSE 0

static bool vec_contains(std::vector<int> *vec, int i)
{
	return std::find(vec->begin(), vec->end(), i) != vec->end();
}

void GeneticRepresenter::ensure_uses(int index, unsigned int num_to_use)
{
	while (indices[index]->size() > num_to_use)
	{
		int to_del = rand() % indices[index]->size();
		pop[index][indices[index]->at(to_del)] = 0;
		// this line should be easier...
//		indices[index]->erase(std::find(indices[index]->begin(), indices[index]->end(), indices[index]->at(to_del)));
		indices[index]->erase(indices[index]->begin() + to_del);
	}

	while (indices[index]->size() < num_to_use)
	{
		int to_add = rand() % iset->size();
		if (vec_contains(indices[index], to_add))
		{
			continue;
		}
		indices[index]->push_back(to_add);
		pop[index][to_add] = 1;
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

	if (GA_VERBOSE)
	{
		printf("mutating from %d to %d\n", idx2, idx1);
	}

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


//void GeneticRepresenter::cross_over(int parent1, int parent2, int num_to_use)
//{
//	indices[0]->clear();
//	for (int i = 0; i < iset->size(); i++)
//	{
//		pop[0][i] = 0;
//	}
//
//	for (int i = 0; i < num_to_use; i++)
//	{
//		int index;
//		bool first = rand() % 2;
//		if (first)
//		{
//			index = indices[parent1]->at(i);
//		}
//		else
//		{
//			index = indices[parent2]->at(i);
//		}
//
//		if (vec_contains(indices[0], index))
//		{
//			continue;
//		}
//
//		indices[0]->push_back(index);
//		pop[0][index] = 1;
//	}
//
//	ensure_uses(0, num_to_use);
//}

//
//void GeneticRepresenter::mutate(int num_to_flip, int num_to_use, double *costs)
//{
//	for (int i = 0; i < num_to_flip; i++)
//	{
//		int idx = rand() % indices[0]->size();
//		int turn_off = indices[0]->at(idx);
//		int turn_on;
//		do
//		{
//			turn_on = rand() % iset->size();
//		} while (vec_contains(indices[0], turn_on));
//
//		printf("flipping %d to %d\n", turn_off, turn_on);
//
//		indices[0]->at(idx) = turn_on;
//		pop[0][turn_off] = 0;
//		pop[0][turn_on] = 1;
//	}
//}


void GeneticRepresenter::mutate(int num_to_flip, int num_to_use, double *costs)
{
	for (int i = 0; i < num_to_flip; i++)
	{
		int idx = rand() % indices[0]->size();
		int turn_off = indices[0]->at(idx);
		int turn_on;
		do
		{
			turn_on = rand() % iset->size();
		} while (vec_contains(indices[0], turn_on));

		if (GA_VERBOSE)
		{
			printf("flipping %d to %d\n", turn_off, turn_on);
		}

		indices[0]->at(idx) = turn_on;
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
			pop_size(_pop_size),
			pop((char **) malloc (sizeof(*pop) * (_pop_size + 1))),
			indices((std::vector<int> **) malloc (sizeof(*indices) * (_pop_size + 1))),
			fitness((double *) malloc (sizeof(*fitness) * (_pop_size + 1))),
			msk_all((char *) malloc (sizeof(*msk_all) * cap)),
			iset(NULL)
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		indices[i] = new std::vector<int>;
		pop[i] = (char *) malloc(sizeof(*pop[i]) * cap);
	}

	for (int i = 0; i < cap; i++)
	{
		msk_all[i] = 1;
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
	free(msk_all);
	free(fitness);
}

void GeneticRepresenter::represent(int num_points, RepresentationMetric *metric, char *mask_out)
{
	this->iset = metric->get_set();

	double *costs = (double *) malloc (sizeof (*costs) * num_points);

	// select initial population
	for (int i = 1; i <= pop_size; i++)
	{
		for (int j = 0; j < iset->size(); j++)
		{
			// no uninitialized variables....
			pop[i][j] = 0.0;
		}
		ensure_uses(i, num_points);
		greedy_improve(metric, indices[i], pop[i]);

		fitness[i] = metric->get_fitness(pop[i], iset->get_all_pnts(), costs);
	}

	int nloops = 100;

	for (int i = 0; i < nloops; i++)
	{
		if (GA_VERBOSE)
		{
			printf("generation %d\n", i);
		}
		else if ((i % 100) == 0)
		{
			printf("%d/%d\n", i, nloops);
		}

		int p1 = 1 + rand() % pop_size;
		int p2;
		do
		{
			p2 = 1 + rand() % pop_size;
		} while (p1 == p2);

		if (GA_VERBOSE)
		{
			printf("selecting %d and %d\n", p1, p2);
			print(0);
		}

		cross_over(p1, p2, num_points);
		mutate(2, num_points, costs);

		greedy_improve(metric, indices[0], pop[0]);

		fitness[0] = metric->get_fitness(pop[0], iset->get_all_pnts(), costs);

		select(i);

		if (GA_VERBOSE)
		{
//			sleep(1);
		}
	}

	int most_fit_index = INT_MIN;
	double most_fit = -DBL_MAX;

	for (int i = 0; i <= pop_size; i++)
	{
		if (fitness[i] > most_fit)
		{
			most_fit_index = i;
			most_fit = fitness[i];
		}
	}

	greedy_improve(metric, indices[most_fit_index], pop[most_fit_index]);

	char * most_fit_mask = pop[most_fit_index];
	for (int i = 0; i < iset->size(); i++)
	{
		mask_out[i] = most_fit_mask[i];
	}

	free(costs);
	iset = NULL;
}


void GeneticRepresenter::select(int generation)
{
	int least_fit_index = INT_MIN;
	double least_fitness = DBL_MAX;

	for (int i = 0; i < pop_size + 1; i++)
	{
		if (fitness[i] < least_fitness)
		{
			least_fit_index = i;
			least_fitness = fitness[i];
		}
	}

	if (GA_VERBOSE)
	{
		printf("Least fit = %d, f=%lf\n", least_fit_index, fitness[least_fit_index]);
	}

	if (least_fit_index == 0)
	{
		return;
	}

	if (GA_VERBOSE)
	{
		printf("\tNew least fit: gen=\t%d\t%lf\n", generation, fitness[least_fit_index]);
	}

//	plot("test.m", iset, pop[0], fitness[0]);

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
	{
		float tmp = fitness[0];
		fitness[0] = fitness[least_fit_index];
		fitness[least_fit_index] = tmp;
	}
}

void GeneticRepresenter::print(int index)
{
	for (int i = 0; i <= pop_size; i++)
	{
		printf("%d f=%lf: ", i, fitness[i]);
		for (int j = 0; j < iset->size(); j++)
		{
			if (pop[i][j])
			{
				fputc('1', stdout);
			} else
			{
				fputc('0', stdout);
			}
		}
		fputc('\n', stdout);
	}

	fflush(stdout);
}

