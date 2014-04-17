/*
 * RandomSampleSolver.h
 *
 *  Created on: Apr 5, 2014
 *      Author: thallock
 */

#ifndef RANDOMSAMPLESOLVER_H_
#define RANDOMSAMPLESOLVER_H_

#include "Solver.h"

class RandomSampleSolver : public Solver
{
public:
	RandomSampleSolver() {}
	virtual ~RandomSampleSolver() {}

	void solve(BoundedMopStats *board, int num_to_find, long timeout);
};

#endif /* RANDOMSAMPLESOLVER_H_ */
