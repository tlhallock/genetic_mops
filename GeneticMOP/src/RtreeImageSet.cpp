/*
 * RtreeImageSet.cpp
 *
 *  
 *      Author: thallock
 */

#include "RtreeImageSet.h"

#include "Qtree.h"

#include <float.h>

typedef struct
{
	qtree::Qtree *tree;
	std::vector<double *> *vector;
} acc_arg;

void accumulate(double *point, void *ref, void *arg)
{
	acc_arg *acc = (acc_arg *) arg;
	if (acc->tree->is_pareto(point))
	{
		acc->vector->push_back(point);
	}
}

std::vector<double *> *RtreeImageSet::get_pareto_solutions()
{
	if (pareto_set != NULL)
	{
		return pareto_set;
	}

	pareto_set = new std::vector<double *>;

	acc_arg arg;
	arg.vector = pareto_set;
	arg.tree = &image;

	image.apply(&accumulate, (void *) &arg);

	return pareto_set;
}

void RtreeImageSet::get_nadir_point(double *y_out)
{
	for (int i = 0; i < get_dim(); i++)
	{
		y_out[i] = -DBL_MAX;
	}

	// This could be done much faster using the quadtree structure...
	get_pareto_solutions();
	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		for (int i = 0; i < get_dim(); i++)
		{
			if ((*it)[i] > y_out[i])
			{
				y_out[i] = (*it)[i];
			}
		}
	}
}
void RtreeImageSet::get_ideal_point(double *y_out)
{
	for (int i = 0; i < get_dim(); i++)
	{
		y_out[i] = DBL_MAX;
	}

	// This could be done much faster using the quadtree structure...
	get_pareto_solutions();
	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		for (int i = 0; i < get_dim(); i++)
		{
			if ((*it)[i] < y_out[i])
			{
				y_out[i] = (*it)[i];
			}
		}
	}
}

int RtreeImageSet::get_dim()
{
	return image.get_dim();
}

void RtreeImageSet::add_point(double *y)
{
	if (pareto_set != NULL)
	{
		delete pareto_set;
		pareto_set = NULL;
	}

	image.add(y, NULL);
}

double RtreeImageSet::get_epsilon(double (*norm)(double *, double *, int dim))
{
	return 0;
}
double RtreeImageSet::get_delta(double (*norm)(double *, double *, int dim))
{
	return 0;
}

void RtreeImageSet::clear()
{
	image.clear();
}

RtreeImageSet::~RtreeImageSet()
{
	if (pareto_set != NULL)
	{
		delete pareto_set;
	}
}
