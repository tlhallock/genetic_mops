/*
 * RepresentationBreeder.cpp
 *
 *  Created on: May 14, 2014
 *      Author: thallock
 */

#include "RepresentationBreeder.h"

#include "../common.h"

#define GA_VERBOSE 1


void RepresentationBreeder::ensure_uses(int index, unsigned int num_to_use)
{
	std::set<int> *s = indices[index];

	for (std::set<int>::iterator it = s->begin(); it != s->end(); it++)
	{
		if (*it >= current_size)
		{
			s->clear();
			break;
		}
	}

	while (s->size() > num_to_use)
	{
		int to_del = rand() % s->size();
		std::set<int>::iterator it = s->begin();
		std::advance(it, to_del);
		s->erase(it);
	}

	while (s->size() < num_to_use)
	{
		int to_add = rand() % current_size;
		if (s->find(to_add) != s->end())
		{
			continue;
		}
		s->insert(to_add);
	}
}

void RepresentationBreeder::cross_over(int parent1, int parent2, int num_to_use)
{
	int idx1 = rand() % current_size;
	int idx2;
	do
	{
		idx2 = rand() % current_size;
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

	std::set<int>::iterator p1 = indices[parent1]->begin();
	std::set<int>::iterator p2 = indices[parent2]->begin();
	std::set<int>::iterator end = indices[parent1]->end();
	for (int i = 0; p1 != end; i++, p1++, p2++)
	{
		int index;
		if (i < idx1 || i >= idx2)
		{
			index = *p1;
		}
		else
		{
			index = *p2;
		}

		indices[0]->insert(index);
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

void RepresentationBreeder::mutate(int num_to_flip)
{
	for (int i = 0; i < num_to_flip; i++)
	{
		std::set<int>::iterator it = indices[0]->begin();
		advance(it, rand() % indices[0]->size());
		int turn_off = *it;
		indices[0]->erase(it);

		int turn_on;
		do
		{
			turn_on = rand() % current_size;
		} while (turn_on == turn_off ||
				indices[0]->find(turn_on) != indices[0]->end());

		if (GA_VERBOSE)
		{
			printf("flipping %d to %d\n", turn_off, turn_on);
		}

		indices[0]->insert(turn_on);
	}
}

RepresentationBreeder::RepresentationBreeder(int _cap, int _pop_size) :
		cap(_cap),
		pop_size(_pop_size),
		indices((std::set<int> **) malloc (sizeof(*indices) * (_pop_size + 1))),
		fitness((double *) malloc (sizeof(*fitness) * (_pop_size + 1))),
		current_size(0)
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		indices[i] = new std::set<int>;
	}
}

RepresentationBreeder::~RepresentationBreeder()
{
	for (int i = 0; i < pop_size + 1; i++)
	{
		delete indices[i];
	}
	free(indices);
	free(fitness);
}

void RepresentationBreeder::represent(int num_points, DistCache *dcache, std::set<int> *mask_out)
{
	current_size = dcache->size();
	if (current_size > cap)
	{
		puts("oops! 1234567876543678");
		break_die();
	}

	if (current_size <= num_points)
	{
		mask_out->clear();
		for (int i = 0; i < current_size; i++)
		{
			mask_out->insert(i);
		}
		return;
	}

	ClosestCache cc(dcache);

	// select initial population
	for (int i = 1; i <= pop_size; i++)
	{
		ensure_uses(i, num_points);
		cc.assign(indices[i]);
		fitness[i] = cc.get_diversity_fitness(-13);
	}
	fitness[0] = -1;

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
		mutate(2);

		cc.assign(indices[0]);
		fitness[0] = cc.get_diversity_fitness(-13);

		select(i);
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


	std::set<int> *rset = indices[most_fit_index];
	mask_out->clear();
	for (std::set<int>::iterator it=rset->begin(); it != rset->end(); it++)
	{
		mask_out->insert(*it);
	}
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

	if (least_fit_index == INT_MIN)
	{
		puts("all elements have fitness of DBL_MAX!");
		return;
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

	{
		std::set<int> *tmp = indices[0];
		indices[0] = indices[least_fit_index];
		indices[least_fit_index] = tmp;
	}
	{
		float tmp = fitness[0];
		fitness[0] = fitness[least_fit_index];
		fitness[least_fit_index] = tmp;
	}
}

void RepresentationBreeder::print(int index)
{
	for (int i = 0; i <= pop_size; i++)
	{
		printf("%d f=%lf: ", i, fitness[i]);
		for (int j = 0; j < current_size; j++)
		{
			if (indices[i]->find(j) != indices[i]->end())
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

