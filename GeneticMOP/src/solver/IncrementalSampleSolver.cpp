/*
 * GaussianSampleSolver.cpp
 *
 *  
 *      Author: thallock
 */

#include "IncrementalSampleSolver.h"

static void set_pareto_optimals_with_x(std::vector<double *> *initial_x, std::vector<double *> *next_x, std::vector<double *> *initial_y, std::vector<double *> *next_y, int dim, bool trim)
{

	for (unsigned int i = 0; i < initial_y->size(); i++)
	{
		bool ignore = trim && rand() % 2;
		if (is_pareto_optimal(initial_y->at(i), initial_y, dim) && !ignore)
		{
			next_x->push_back(initial_x->at(i));
			next_y->push_back(initial_y->at(i));
		}
	}

	for (unsigned int i = 0; i < initial_y->size(); i++)
	{
		if (std::find(next_y->begin(), next_y->end(), initial_y->at(i)) != next_y->end())
		{
			continue;
		}

		free(initial_x->at(i));
		free(initial_y->at(i));
	}

	initial_x->clear();
	initial_y->clear();
}


void IncrementalSampleSolver::solve(BoundedMopStats *board, int num_turns, long timeout)
{
	time_t start_time = time(0);

	int xdim = board->get_x_dimension();
	int ydim = board->get_y_dimension();

	std::vector<double *> *initial_x = new std::vector<double *>();
	std::vector<double *> *initial_y = new std::vector<double *>();

	for (unsigned int i=0;i<initial_seed;i++)
	{
		double *x = (double *) malloc(sizeof (*x) * xdim);
		double *y = (double *) malloc(sizeof (*y) * ydim);

		board->sample_feasible(x);
		board->make_guess(x, y);

		initial_x->push_back(x);
		initial_y->push_back(y);
	}

	std::vector<double *> *next_x = new std::vector<double *>();
	std::vector<double *> *next_y = new std::vector<double *>();

	int count = 1;
	do
	{
		if (count++ >= 0)
		{
			bool trim = initial_x->size() > block_size / 2 && /*not first*/ count==1;
			set_pareto_optimals_with_x(initial_x, next_x, initial_y, next_y, ydim, trim);

			std::vector<double *> *tmp = initial_x;
			initial_x = next_x;
			next_x = tmp;

			tmp = initial_y;
			initial_y = next_y;
			next_y = tmp;

			count = -block_size;
		}

		double *x = (double *) malloc(sizeof (*x) * xdim);
		double *y = (double *) malloc(sizeof (*y) * ydim);

		int index = rand() % initial_x->size();

		double *old_x = initial_x->at(index);

		for (int i=0;i<xdim;i++)
		{
			x[i] = old_x[i] - range + (rand() / (double) INT_MAX * 2 * range);
		}

		while (!board->is_feasible(x))
		{
			for (int i=0; i<xdim; i++)
			{
				x[i] += (old_x[i] - x[i]) / 2;
			}
		}

		board->make_guess(x, y);

		initial_x->push_back(x);
		initial_y->push_back(y);
	} while ((time(0) - start_time) < timeout
			&& board->get_num_points() < num_turns);

	for (unsigned int i=0;i<initial_x->size();i++)
	{
		free(initial_x->at(i));
		free(initial_y->at(i));
	}

	delete initial_x;
	delete initial_y;

	delete next_x;
	delete next_y;
}
