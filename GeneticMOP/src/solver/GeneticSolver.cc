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
	dcache(cap, xdim, ydim, &l_2),
	ccache(&dcache),
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

void GeneticSolver::breed()
{
	for (int i = 0; i < xdim; i++)
	{
		int parent;
		do
		{
			parent = rand() % breed_size;
		} while(current_fit[parent] == NULL);

		offspring[i] = current_fit[parent][i];
	}

	printf("offspring = ");
	print_point(stdout, offspring, xdim, true);
}

void GeneticSolver::find_fittest()
{
	int rsize = dcache.size();

	int parent = -1555;
	if (rand() % 2 || true)
	{
		double max = -DBL_MAX;
		double *minsL = (double *) alloca(sizeof(*minsL) * ydim);
		double *minsU = (double *) alloca(sizeof(*minsU) * ydim);

		int sample_size;
		if (rsize < 3)
		{
			sample_size = 1;
		} else
		{
			sample_size = rsize / 3;
		}

		for (int i = 0; i < sample_size; i++)
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

	current_fit[0] = dcache.getY(parent);

	printf("Found fittest %d ", parent);
	print_point(stdout, current_fit[0], xdim, true);


#define NUM_INCEST 3
	int nearest[NUM_INCEST];
	double dists[NUM_INCEST];
	ccache.get_n_nearest(parent, NUM_INCEST, nearest, dists);
	int p2 = rand() % NUM_INCEST;
	if (nearest[p2] >= 0)
	{
		current_fit[1] = dcache.getY(nearest[p2]);
		printf("breeding with %d ", nearest[p2]);
		print_point(stdout, current_fit[1], xdim, true);
	}
	else
	{
		printf("There isn't a %d closest.\n", p2);
		current_fit[1] = NULL;
	}
}

void GeneticSolver::mutate()
{
	for (int i = 0; i < xdim; i++)
	{
		double randomNumber = 2 * rand() / (double) RAND_MAX - 1;
		offspring[i] *= 1 + randomNumber / 100.0;
	}
	printf("mutated to = ");
	print_point(stdout, offspring, xdim, true);
}

void GeneticSolver::select()
{
	bool cleared = false;
	if (dcache.size() >= .90 * cap)
	{
		cleared = true;

		dcache.clear_non_pareto();
		ccache.point_removed();

		while (dcache.size() >= .90 * cap)
		{
			// remove the closest...
			puts("implement this... 15709138560113508173507");
			break_die();
		}
	}

	if (dcache.size() <= npoints)
	{
		for (int i = 0; i < dcache.size(); i++)
		{
			current_rep.insert(i);
		}
	}
	else if (cleared || (0 == rand() % 50 && dcache.size() > 2 * npoints))
	{
		gene_selector.represent(npoints, &dcache, &current_rep);
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

		dcache.add(x, y);
		ccache.point_added();
	}

	dcache.clear_non_pareto();
	ccache.point_removed();

	gene_selector.represent(npoints, &dcache, &current_rep);
	ccache.assign(&current_rep);

	while ((time(0) - start_time) < timeout
			&& mop->get_num_points() < num_to_find)
	{
		printf("currently have %d points to choose from.\n", dcache.size());

		find_fittest();
		breed();
		mutate();

		mop->make_guess(offspring, y);
		dcache.add(offspring, y);
		ccache.point_added();

		select();
		putchar('\n');
	}

	free(x);
	free(y);
}


double GeneticSolver::get_isolation(int index, double *minsL, double *minsU)
{
	int ydim = dcache.get_ydim();
	int size = dcache.size();

	for (int i = 0; i < ydim; i++)
	{
		minsL[i] = DBL_MAX;
		minsU[i] = DBL_MAX;
	}

	double *pnt = dcache.getY(index);

	for (int j = 0; j < size; j++)
	{
		double *other = dcache.getY(j);

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
