/*
 * GeneticSolver.h
 *
 *  Created on: Apr 5, 2014
 *      Author: thallock
 */

#ifndef GENETICSOLVER_H_
#define GENETICSOLVER_H_

#include "Solver.h"

#include <stdlib.h>

class GeneticSolver : public Solver
{
private:
	int population_size;
	int breed_size;
	double **current_fit;
	int xdim;
	double *offspring;
	std::vector<double *> x_population;
	std::vector<double *> y_population;
	double infeasible_cost;
	double mutation_factor;
	double (*get_index)(double);
	void bread();
	void find_fittest();
	void mutate();
	void sort(BoundedMopStats *board);
	double get_fitness(BoundedMopStats *board, double *x, double *y);
public:
	GeneticSolver(int _orgy_size, int _xdim, double _infeasible_cost, int _population_size, double _mutation_factor, double (*_get_index)(double)) :
		population_size(_population_size),
		breed_size(_orgy_size),
		current_fit((double **) malloc(sizeof(*current_fit) * _orgy_size)),
		xdim(_xdim),
		offspring((double *) malloc(sizeof(*offspring) * xdim)),
		x_population(),
		y_population(),
		infeasible_cost(_infeasible_cost),
		mutation_factor(_mutation_factor),
		get_index(_get_index)
		{for (int i=0;i<breed_size;i++) current_fit[i] =
				(double *) malloc (sizeof (*current_fit[i]) * xdim);}
	virtual ~GeneticSolver()
	{/*for (int i=0;i<orgy_size;i++) free(current_fit[i]);
		free(current_fit); free(offspring);*/}

	void solve(BoundedMopStats *board, int num_to_find, long timeout);
};

#endif /* GENETICSOLVER_H_ */
