/*
 * RtreeImageSet.h
 *
 *  
 *      Author: thallock
 */

#ifndef RTREEIMAGESET_H_
#define RTREEIMAGESET_H_

#include "Qtree.h"
#include "ImageSet.h"

#include <vector>

class RtreeImageSet : public ImageSet
{
private:
	qtree::Qtree image;
	std::vector<double*> *pareto_set;

	bool is_pareto_optimal(double *y);
public:
	RtreeImageSet(int _ydim) :
		image(_ydim),
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
