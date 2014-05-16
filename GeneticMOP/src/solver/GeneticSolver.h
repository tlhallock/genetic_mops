/*
 * GeneticSolver.h
 *
 *  
 *      Author: thallock
 */

#ifndef GENETICSOLVER_H_
#define GENETICSOLVER_H_

#include "../solver/Solver.h"
#include "../represent/RepresentationBreeder.h"

#include <set>

#define TARGET_PNTS 100

class GeneticSolver : public Solver
{
private:
	int cap;
	int breed_size;
	int npoints;
	int xdim;
	int ydim;
	double **current_fit;
	double *offspring;

	std::set<int> current_rep;

	DistCache dcache;
	ClosestCache ccache;
	RepresentationBreeder gene_selector;

	int num_in_gen;

	void euthanasia();
	void breed();
	void find_fittest();
	bool mutate(BoundedMopStats *mop);
	void select();

	double get_isolation(int index, double *minsL, double *minsU);
public:
	GeneticSolver(int xdim, int ydim, int npoints);
	virtual ~GeneticSolver();

	void solve(BoundedMopStats *board, int num_to_find, long timeout);
};

#endif /* GENETICSOLVER_H_ */
