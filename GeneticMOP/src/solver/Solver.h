/*
 * Solver.h
 *
 *  
 *      Author: thallock
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "../MOP.h"

class Solver
{
public:
	Solver() {};
	virtual ~Solver() {};

	virtual void solve(BoundedMopStats *board, int num_to_find, long timeout) = 0;
};

#endif /* SOLVER_H_ */
