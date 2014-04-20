/*
 * RtreeImageSet.cpp
 *
 *  
 *      Author: thallock
 */

#include "RtreeImageSet.h"

#include "qtree_mop.h"

#include <float.h>

typedef struct
{
	qtree::qtree *tree;
	std::vector<double *> *vector;
} acc_arg;

void accumulate(double *point, void *arg)
{
	acc_arg *acc = (acc_arg *) arg;
	if (qtree::qtree_is_pareto(acc->tree, point))
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
	arg.tree = image;

	qtree::qtree_apply(image, &accumulate, (void *) &arg);

	return pareto_set;
}


void RtreeImageSet::get_nadir_point(double *y_out)
{
	for (int i=0;i<get_dim(); i++)
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
	for (int i=0;i<get_dim(); i++)
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
	return image->dim;
}

void RtreeImageSet::add_point(double *y)
{
	if (!qtree::qtree_in_bounds(image, y))
	{
		puts("Ignoring invalid point");
		return;
	}

	if (pareto_set != NULL)
	{
		delete pareto_set;
		pareto_set = NULL;
	}

	qtree::qtree_add(image, y);
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
	qtree::qtree_clear(image);
}

RtreeImageSet::~RtreeImageSet()
{
	if (pareto_set != NULL)
	{
		delete pareto_set;
	}
	qtree::qtree_del(image);
}
