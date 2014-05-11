/*
 * GeneticSolver.h
 *
 *  
 *      Author: thallock
 */

#ifndef GENETICSOLVER_H_
#define GENETICSOLVER_H_

#include "Solver.h"

#include "common.h"

const double k_infeasible_cost = .1;

class GeneticSolver : public Solver
{
private:
	int breed_size;
	int xdim;
	int ydim;
	double **current_fit;
	double *offspring;
	qtree::Qtree x_pop_feas;
	qtree::Qtree y_pop_feas;
	qtree::Qtree x_pop_inf;
	qtree::Qtree y_pop_inf;

	void bread();
	void find_fittest();
	void mutate();
	void select();
	double get_fitness(BoundedMopStats *board, double *x, double *y);
public:
	GeneticSolver(int xdim, int ydim, int breed_size);
	virtual ~GeneticSolver();

	void solve(BoundedMopStats *board, int num_to_find, long timeout);
};

#endif /* GENETICSOLVER_H_ */
