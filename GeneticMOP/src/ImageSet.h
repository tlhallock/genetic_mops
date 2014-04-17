/*
 * ImageSet.h
 *
 *  Created on: Apr 5, 2014
 *      Author: thallock
 */

#ifndef IMAGESET_H_
#define IMAGESET_H_

#include <vector>

class ImageSet
{
public:
	ImageSet() {}
	virtual ~ImageSet() {}

	virtual std::vector<double *> *get_pareto_solutions() = 0;

	virtual void get_nadir_point(double *y_out) = 0;
	virtual void get_ideal_point(double *y_out) = 0;

	virtual void add_point(double *y) = 0;

	virtual int get_dim() = 0;

	virtual void clear() = 0;

	virtual double get_epsilon(double (*norm)(double *, double *, int dim)) = 0;
	virtual double get_delta(double (*norm)(double *, double *, int dim)) = 0;
};

#endif /* IMAGESET_H_ */
