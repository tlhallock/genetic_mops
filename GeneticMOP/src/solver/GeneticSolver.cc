/*
 * GeneticSolver.cpp
 *
 *  
 *      Author: thallock
 */

#include "GeneticSolver.h"

#include "../common.h"

GeneticSolver::GeneticSolver(int _xdim, int _ydim, int num_points) :
	cap(500),
	breed_size(2),
	npoints(num_points),
	xdim(_xdim),
	ydim(_ydim),
	current_fit((double **) malloc (sizeof(*current_fit) * breed_size)),
	offspring((double *) malloc (sizeof (*offspring) * _xdim)),
	current_rep(),
	cache(cap, xdim, ydim, &l_2),
	ccache(&cache),
	gene_selector(cap, 10)
{
	for (int i = 0; i < breed_size; i++)
	{
		current_fit[i] = NULL;
	}
}

GeneticSolver::~GeneticSolver()
{
	free(current_fit);
	free(offspring);
}

double GeneticSolver::get_fitness(BoundedMopStats *board, double *x, double *y)
{
    return 0;
}

void GeneticSolver::breed()
{
	for (int i = 0; i < xdim; i++)
	{
		int parent = rand() % breed_size;
		offspring[i] = current_fit[parent][i];
	}
}

void GeneticSolver::find_fittest()
{
	int rsize = cache.size();

	int parent = 0;
	if (rand() % 2)
	{
		double max = -DBL_MAX;
		double *minsL = (double *) alloca(sizeof(*minsL) * ydim);
		double *minsU = (double *) alloca(sizeof(*minsU) * ydim);

		for (int i = 0; i < rsize / 3; i++)
		{
			int index = rand() % rsize;

			if (get_isolation(index, minsL, minsU) > max)
			{
				parent = index;
			}
		}
	}
	else // improving representation
	{

	}

	double *p = cache.getY(parent);
	for (int i = 0; i < ydim; i++)
	{
		current_fit[0][i] = p[i];
	}

#define NUM_INCEST 3
	int nearest[NUM_INCEST];
	double dists[NUM_INCEST];
	ccache.get_n_nearest(parent, NUM_INCEST, nearest, dists);
	p = cache.getY(nearest[rand() % NUM_INCEST]);
	for (int i = 0; i < ydim; i++)
	{
		current_fit[1][i] = p[i];
	}
}

void GeneticSolver::mutate()
{
	for (int i = 0; i < xdim; i++)
	{
		double randomNumber = 2 * rand() / (double) RAND_MAX - 1;
		offspring[i] *= 1 + randomNumber / 100.0;
	}
}

void GeneticSolver::select()
{
	if (cache.size() >= .90 * cap)
	{
		cache.clear_non_pareto();

		while (cache.size() >= .90 * cap)
		{
			// remove the closest...
			puts("implement this... 15709138560113508173507");
			exit(1);
		}
	}

	if (rand() % 100)
	{
		gene_selector.represent(npoints, &cache, &current_rep);
		ccache.assign(&current_rep);
	}
}

void GeneticSolver::solve(BoundedMopStats *mop, int num_to_find, long timeout)
{
	time_t start_time = time(0);

	double *x = (double *) malloc(sizeof(*x) * xdim);
	double *y = (double *) malloc(sizeof(*y) * ydim);

	for (int i = 0; i < 20; i++)
	{
		mop->sample_feasible(x);
		mop->make_guess(x, y);

		cache.add(x, y);
	}

	cache.clear_non_pareto();
	gene_selector.represent(npoints, &cache, &current_rep);
	ccache.assign(&current_rep);

	while ((time(0) - start_time) < timeout
			&& mop->get_num_points() < num_to_find)
	{
		find_fittest();
		breed();
		mutate();

		mop->make_guess(offspring, y);
		cache.add(offspring, y);

		select();
	}

	free(x);
	free(y);
}


double GeneticSolver::get_isolation(int index, double *minsL, double *minsU)
{
	int ydim = cache.get_ydim();
	int size = cache.size();

	for (int i=0; i<ydim;i++)
	{
		minsL[i] = DBL_MAX;
		minsU[i] = DBL_MAX;
	}

	double *pnt = cache.getY(index);

	for (int j = 0; j < size; j++)
	{
		double *other = cache.getY(j);

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

	// should try average...
	double max = -DBL_MAX;
	for (int i = 0; i < ydim; i++)
	{
		if (minsL[i] < DBL_MAX && minsL[i] > max)
		{
			max = minsL[i];
		}
		if (minsU[i] < DBL_MAX && minsU[i] > max)
		{
			max = minsU[i];
		}
	}
	return max;
}
