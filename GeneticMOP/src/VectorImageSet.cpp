/*
 * VectorImageSet.cpp
 *
 *  
 *      Author: thallock
 */

#include "VectorImageSet.h"

#include "utils.h"
#include "float.h"


VectorImageSet::~VectorImageSet()
{
	clear();
}


std::vector<double *> *VectorImageSet::get_pareto_solutions()
{
	if (pareto_set != NULL)
	{
		return pareto_set;
	}

	pareto_set = new std::vector<double*>();
	set_pareto_optimals(&image, pareto_set, ydim);
	return pareto_set;
}

void VectorImageSet::get_nadir_point(double *y_out)
{
	for (int i=0;i<ydim;i++)
	{
		y_out[i] = -DBL_MAX;
	}

	get_pareto_solutions();

	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		for (int i=0; i<ydim; i++)
		{
			if ((*it)[i] > y_out[i])
			{
				y_out[i] = (*it)[i];
			}
		}
	}
}

void VectorImageSet::get_ideal_point(double *y_out)
{
	for (int i=0;i<ydim;i++)
	{
		y_out[i] = DBL_MAX;
	}

	get_pareto_solutions();

	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		for (int i=0; i<ydim; i++)
		{
			if ((*it)[i] < y_out[i])
			{
				y_out[i] = (*it)[i];
			}
		}
	}
}

void VectorImageSet::add_point(double *y)
{
	if (pareto_set != NULL)
	{
		delete pareto_set;
		pareto_set = NULL;
	}

	image.push_back(copy_point(y, ydim));
}

void VectorImageSet::clear()
{
	if (pareto_set != NULL)
	{
		delete pareto_set;
		pareto_set = NULL;
	}

	for (std::vector<double *>::iterator it = image.begin(); it != image.end(); ++it)
	{
		free(*it);
	}

	image.clear();
}



int VectorImageSet::get_dim()
{
	return ydim;
}

static double vector_image_get_min_distance(double *point, std::vector<double *> *all, double (*norm)(double *, double *, int dim), int dim)
{
	double min_dist = DBL_MAX;

	for (std::vector<double *>::iterator it = all->begin(); it != all->end(); ++it)
	{
		bool points_are_equal = true;
		for (int i=0;i<dim;i++)
		{
			if (point[i] != *it[i])
			{
				points_are_equal = false;
				break;
			}
		}
		if (points_are_equal)
		{
			continue;
		}

		double dist = norm(point, *it, dim);
		if (dist < min_dist)
		{
			min_dist = dist;
		}
	}

	return min_dist;
}


double VectorImageSet::get_epsilon(double (*norm)(double *, double *, int dim))
{
	get_pareto_solutions();

	double max_dist = DBL_MAX;

	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		double current_min_dist = 0;

		double dist = vector_image_get_min_distance(*it, pareto_set, norm, ydim);
		if (dist > max_dist)
		{
			max_dist = dist;
		}
	}

	return max_dist;
}

double VectorImageSet::get_delta(double (*norm)(double *, double *, int dim))
{
	get_pareto_solutions();

	double min_dist = DBL_MAX;

	for (std::vector<double *>::iterator it = pareto_set->begin(); it != pareto_set->end(); ++it)
	{
		double current_min_dist = 0;

		double dist = vector_image_get_min_distance(*it, pareto_set, norm, ydim);
		if (dist < min_dist)
		{
			min_dist = dist;
		}
	}

	return min_dist;
}
