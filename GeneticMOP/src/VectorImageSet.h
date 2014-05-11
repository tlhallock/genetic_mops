/*
 * VectorImageSet.h
 *
 *  
 *      Author: thallock
 */

#ifndef VECTORIMAGESET_H_
#define VECTORIMAGESET_H_

#include "ImageSet.h"

#include "common.h"


class VectorImageSet : public ImageSet
{
private:
	std::vector<double*> image;
	std::vector<double*> *pareto_set;
	int ydim;

	bool is_pareto_optimal(double *y);
public:
	VectorImageSet(int _ydim) :
		image(),
		pareto_set(NULL),
		ydim(_ydim) {}
	virtual ~VectorImageSet();

	std::vector<double *> *get_pareto_solutions();
	void get_nadir_point(double *y_out);
	void get_ideal_point(double *y_out);

	int get_dim();

	void add_point(double *y);
	double get_epsilon(double (*norm)(double *, double *, int dim));
	double get_delta(double (*norm)(double *, double *, int dim));

	void clear();
};

#endif /* VECTORIMAGESET_H_ */
