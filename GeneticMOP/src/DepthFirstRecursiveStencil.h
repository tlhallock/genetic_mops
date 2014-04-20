/*
 * DepthFirstRecursiveStencil.h
 *
 *  
 *      Author: rever
 */

#ifndef DEPTHFIRSTRECURSIVESTENCIL_H_
#define DEPTHFIRSTRECURSIVESTENCIL_H_

#include "MOP.h"

class DepthFirstRecursiveStencil
{
private:
	double epsilon;
	int max_depth_feasible;
	int max_depth_infeasible;
	void back_track(double *x_lb, double *x_up, int depth);
	BoundedMopStats *stats;
	double (*norm)(double *p1, double *p2, int dim);
public:
	DepthFirstRecursiveStencil(double _epsilon,
			double (*_norm)(double *, double *, int),
			int _max_depth_feasible,
			int _max_depth_infeasible) :
			epsilon(_epsilon),
			max_depth_feasible  (_max_depth_feasible),
			max_depth_infeasible(_max_depth_infeasible),
			stats(NULL),
			norm(_norm) {}
	virtual ~DepthFirstRecursiveStencil() {}

	void solve(BoundedMopStats *board);
};

#endif /* DEPTHFIRSTRECURSIVESTENCIL_H_ */
