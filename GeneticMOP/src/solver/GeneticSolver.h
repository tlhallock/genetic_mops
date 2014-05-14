/*
 * GeneticSolver.h
 *
 *  
 *      Author: thallock
 */

#ifndef GENETICSOLVER_H_
#define GENETICSOLVER_H_

#include "../solver/Solver.h"

#define TARGET_PNTS 100

typedef struct {
	double *x;
	double *y;
} xy_pair;

class GeneticSolver : public Solver
{
private:
	int breed_size;
	int xdim;
	int ydim;
	double **current_fit;
	double *offspring;
	xy_pair *pop;

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
