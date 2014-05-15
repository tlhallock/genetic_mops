/*
 * GeneticSolver.cpp
 *
 *  
 *      Author: thallock
 */

#include "GeneticSolver.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

GeneticSolver::GeneticSolver(int _xdim, int _ydim, int _breed_size) :
	breed_size(_breed_size),
	xdim(_xdim),
	ydim(_ydim),
	current_fit((double **) malloc (sizeof(*current_fit) * breed_size)),
	offspring((double *) malloc (sizeof (*offspring) * _xdim)),
	pop((xy_pair *) malloc (sizeof (*pop) * 2 * TARGET_PNTS))
{
	for (int i = 0; i < _breed_size; i++)
	{
		current_fit[i] = NULL;
	}
}

GeneticSolver::~GeneticSolver()
{
	free(pop);
	free(current_fit);
	free(offspring);
}

double GeneticSolver::get_fitness(BoundedMopStats *board, double *x, double *y)
{
    return 0;
}

void GeneticSolver::bread()
{
	for (int i = 0; i < xdim; i++)
	{
		int parent = rand() % breed_size;
		offspring[i] = current_fit[parent][i];
	}
}

void GeneticSolver::find_fittest()
{
	if (exploring)
	{

	}
	else // improving representation
	{

	}
}

void GeneticSolver::mutate()
{
	FILE *debug_file = fopen("ga_debug.txt", "a");
	fprintf(debug_file, "mutating: ");
//	print_point(debug_file, offspring, xdim, false);

	for (int i = 0; i < xdim; i++)
	{
//		offspring[i] += offspring[i] * mutation_factor * (2 * (rand() / (double) INT_MAX) - 1);
	}

	fprintf(debug_file, " to ");
//	print_point(debug_file, offspring, xdim, true);
	fclose(debug_file);
}

void GeneticSolver::solve(BoundedMopStats *mop, int num_to_find, long timeout)
{
	time_t start_time = time(0);

	int xdim = mop->get_x_dimension();
	int ydim = mop->get_y_dimension();

	DistCache dcache(xdim, ydim);

	double *x = (double *) malloc(sizeof(*x) * xdim);
	double *y = (double *) malloc(sizeof(*y) * ydim);

	for (int i = 0; i < 20; i++)
	{
		mop->sample_feasible(x);
		mop->make_guess(x, y);

		dcache.add(x, y);
	}

	dcache.clear_non_pareto();

	while ((time(0) - start_time) < timeout
			&& mop->get_num_points() < num_to_find)
	{
		find_fittest();
		bread();
		mutate();

		mop->make_guess(offspring, y);
		dcache.add(offspring, y);
		select();
	}

	free(x);
	free(y);
}


double GeneticSolver::find_isolated(int index, double *minsL, double *minsU, DistCache *cache)
{
	int ydim = cache->get_dim();
	int size = cache->size();

//	double *minsL = (double *) alloca(sizeof(*minsL) * ydim);
//	double *minsU = (double *) alloca(sizeof(*minsU) * ydim);
	for (int i=0; i<ydim;i++)
	{
		minsL[i] = DBL_MAX;
		minsU[i] = DBL_MAX;
	}

	double *pnt = cache->getY(index);

	double max = -DBL_MAX;
	for (int j = 0; j < size; j++)
	{
		double *other = cache->getY(j);

		for (int i = 0; i < ydim; i++)
		{
			if (other[i] < pnt[i])
			{
				double dist = pnt[i] - other[i];
				if (dist < minsL[i])
				{
					minsL[i] = dist;
				}
			}
			else
			{
				double dist = other[i] - pnt[i];
				if (dist < minsU[i])
				{
					minsU[i] = dist;
				}
			}
		}
	}
}
