/*
 * RepresentationBreeder.cpp
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#include "RepresentationBreeder.h"

#include "../common.h"

#define GA_VERBOSE 1

static bool vec_contains(std::vector<int> *vec, int i)
{
	return std::find(vec->begin(), vec->end(), i) != vec->end();
}

void RepresentationBreeder::cross_over(int parent1, int parent2, int num_to_use)
{
	int rsize = representation->size();
	int idx1 = rand() % rsize;
	int idx2;
	do
	{
		idx2 = rand() % rsize;
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

	for (int i = 0; i < rsize; i++)
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

void RepresentationBreeder::mutate(int num_to_flip, int num_to_use, double *costs)
{
	int rsize = representation->size();

	for (int i = 0; i < num_to_flip; i++)
	{
		int idx = rand() % indices[0]->size();
		int turn_off = indices[0]->at(idx);
		int turn_on;
		do
		{
			turn_on = rand() % rsize;
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

RepresentationBreeder::RepresentationBreeder(Representation *_rep, int _pop_size) :
			pop_size(_pop_size),
			pop((char **) malloc (sizeof(*pop) * (_pop_size + 1))),
			indices((std::vector<int> **) malloc (sizeof(*indices) * (_pop_size + 1))),
			fitness((double *) malloc (sizeof(*fitness) * (_pop_size + 1))),
			representation(_rep)
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		indices[i] = new std::vector<int>;
		pop[i] = (char *) malloc(sizeof(*pop[i]) * _pop_size);
	}
}

RepresentationBreeder::~RepresentationBreeder()
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		delete indices[i];
		free(pop[i]);
	}
	free(pop);
	free(indices);
	free(fitness);
}

void RepresentationBreeder::represent(int num_points, char *mask_out)
{
	int rsize = representation->size();

	// select initial population
	for (int i = 1; i <= pop_size; i++)
	{

		ensure_uses(i, num_points);

		fitness[i] = representation->get_diversity_fitness(-13);




		metric->get_fitness(pop[i], iset->get_all_pnts(), costs);
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

		representation->greedy_improve_diversity(3);


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
}


void RepresentationBreeder::select(int generation)
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

void RepresentationBreeder::print(int index)
{
	int rsize = representation->size();
	for (int i = 0; i <= pop_size; i++)
	{
		printf("%d f=%lf: ", i, fitness[i]);
		for (int j = 0; j < rsize; j++)
		{
			if (pop[i][j])
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

