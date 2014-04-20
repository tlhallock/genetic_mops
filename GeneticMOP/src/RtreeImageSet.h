/*
 * RtreeImageSet.h
 *
 *  
 *      Author: thallock
 */

#ifndef RTREEIMAGESET_H_
#define RTREEIMAGESET_H_

#include "qtree.h"
#include "ImageSet.h"

#include <vector>

class RtreeImageSet : public ImageSet
{
private:
	qtree::qtree *image;
	std::vector<double*> *pareto_set;
	double *nadir_point;
	double *ideal_point;

	bool is_pareto_optimal(double *y);
public:
	RtreeImageSet(int _ydim, double *lb, double *ub) :
		image(qtree::qtree_new(lb, ub, _ydim)),
		pareto_set(NULL) {};

	virtual ~RtreeImageSet();

	std::vector<double *> *get_pareto_solutions();
	void get_nadir_point(double *y_out);
	void get_ideal_point(double *y_out);

	int get_dim();

	void add_point(double *y);
	double get_epsilon(double (*norm)(double *, double *, int dim));
	double get_delta(double (*norm)(double *, double *, int dim));

	void clear();
};

#endif /* RTREEIMAGESET_H_ */
