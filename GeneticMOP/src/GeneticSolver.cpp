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
	xdim(_xdim),
	ydim(_ydim),
	breed_size(_breed_size),
	current_fit((double **) malloc (sizeof(*current_fit) * breed_size)),
	offspring((double *) malloc (sizeof (*offspring) * _xdim)),
	x_pop_feas(_xdim),
	y_pop_feas(_ydim),
	x_pop_inf(_xdim),
	y_pop_inf(_ydim)
{
	for (int i = 0; i < _breed_size; i++)
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
//	double *closest_feas = qtree::qtree_point_new(xdim);
//	x_pop_feas.find_nearest(x, closest_feas, &l)
//	double *closest_inf  = qtree::qtree_point_new(xdim);
//
//	double cost = 0;
//	if (!board->is_feasible(x))
//	{
//		void find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *), double *cmin);
//
//
//
//
//		double distance = x_pop_feas.find_nearest(board, &x_pop_feas, x, xdim);
//		cost += infeasible_cost * distance;
//	}
//
//	for (std::vector<double *>::iterator it = y_pop_feas.begin(); it != y_pop_feas.end(); ++it)
//	{
//		if (first_dominates_second((*it), y, board->get_y_dimension()))
//		{
//			cost++;
//		}
//	}

//	FILE *debug_file = fopen("ga_debug.txt", "a");
//	fprintf(debug_file, "The cost of [");
//	print_point(debug_file, x, xdim, false);
//	fprintf(debug_file, "] -> [");
//	print_point(debug_file, y, board->get_y_dimension(), false);
//	fprintf(debug_file, "] is %lf.\n", -cost);
//	fclose(debug_file);

//	return -cost;

}

void GeneticSolver::bread()
{
	for (int i = 0; i < xdim; i++)
	{
		int parent = rand() % breed_size;
		offspring[i] = current_fit[parent][i];
	}
}

void fin_fittest_acc(double *point, void *arg)
{
}

void GeneticSolver::find_fittest()
{
	FILE *debug_file = fopen("ga_debug.txt", "a");
	fprintf(debug_file, "found fittest: ");



	for (int i = 0; i < breed_size; i++)
	{
		int index = (int) (get_index(rand() / (double) INT_MAX) * x_pop_feas.size());
		current_fit[i] = x_pop_feas[index];

		fprintf(debug_file, "\n\t%d\t", index);
		print_point(debug_file, current_fit[i], xdim, false);
	}

	fputc('\n', debug_file);
	fclose(debug_file);
}

void GeneticSolver::mutate()
{
	FILE *debug_file = fopen("ga_debug.txt", "a");
	fprintf(debug_file, "mutating: ");
	print_point(debug_file, offspring, xdim, false);

	for (int i = 0; i < xdim; i++)
	{
		offspring[i] += offspring[i] * mutation_factor * (2 * (rand() / (double) INT_MAX) - 1);
	}

	fprintf(debug_file, " to ");
	print_point(debug_file, offspring, xdim, true);
	fclose(debug_file);
}

void GeneticSolver::solve(BoundedMopStats *mop, int num_to_find, long timeout)
{
	time_t start_time = time(0);

	int xdim = mop->get_x_dimension();
	int ydim = mop->get_y_dimension();

	double *x = (double *) malloc(sizeof(*x) * xdim);
	double *y = (double *) malloc(sizeof(*y) * ydim);

	for (int i = 0; i < 20; i++)
	{
		mop->sample_feasible(x);
		mop->make_guess(x, y);

		x_pop_feas.add(x);
		y_pop_feas.add(y);
	}

	while ((time(0) - start_time) < timeout
			&& mop->get_num_points() < num_to_find)
	{
		find_fittest();
		bread();
		mutate();

		mop->make_guess(offspring, y);
		if (mop->is_feasible(offspring))
		{
			x_pop_feas.add(offspring);
			y_pop_feas.add(y);
		}
		else
		{
			x_pop_inf.add(offspring);
			y_pop_inf.add(y);
		}

		select();

		FILE *debug_file = fopen("ga_debug.txt", "a");
		fprintf(debug_file, "new population: ");
//		for (unsigned int i=0;i<x_pop_feas.size(); i++)
//		{
//			fprintf(debug_file, "\n\t%d\t%lf\t", i, get_fitness(mop, x_pop_feas[i], y_pop_feas[i]));
//			print_point(debug_file, x_pop_feas[i], xdim, false);
//			fprintf(debug_file, "   --->   ");
//			print_point(debug_file, y_pop_feas[i], mop->get_y_dimension(), false);
//		}
		fputc('\n', debug_file);
		fclose(debug_file);
	}

	x_pop_feas.clear();
	y_pop_feas.clear();
	x_pop_inf.clear();
	x_pop_inf.clear();

	free(x);
	free(y);
}
