/*
 * GeneticSolver.h
 *
 *  
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
	GeneticSolver(int breed_size, int _xdim, double _infeasible_cost, int _population_size, double _mutation_factor, double (*_get_index)(double));
	virtual ~GeneticSolver();

	void solve(BoundedMopStats *board, int num_to_find, long timeout);
};

#endif /* GENETICSOLVER_H_ */
