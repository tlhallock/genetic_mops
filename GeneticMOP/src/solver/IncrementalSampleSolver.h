/*
 * GaussianSampleSolver.h
 *
 *  
 *      Author: thallock
 */

#ifndef GAUSSIANSAMPLESOLVER_H_
#define GAUSSIANSAMPLESOLVER_H_

#include "Solver.h"

class IncrementalSampleSolver: public Solver
{
private:
	unsigned int initial_seed;
	unsigned int block_size;
	double range;
public:
	IncrementalSampleSolver(unsigned int _initial_seed, unsigned int _block_size, double _range) :
		initial_seed(_initial_seed),
		block_size(_block_size),
		range(_range) {}
	virtual ~IncrementalSampleSolver() {}

	void solve(BoundedMopStats *board, int num_turns, long timeout);
};

#endif /* GAUSSIANSAMPLESOLVER_H_ */
