/*
 * utils.cc
 *
 *  
 *      Author: thallock
 */


#include <stdlib.h>

#include "utils.h"
#include "float.h"

double *copy_point(double *vec, int dim)
{
	double *ret_val = (double *) malloc (sizeof (*vec) * dim);
	for (int i=0;i<dim;i++)
	{
		ret_val[i] = vec[i];
	}
	return ret_val;
}



void print_point(FILE *file, double *vec, int dim, bool new_line)
{
	for (int i = 0; i < dim; i++)
	{
		fprintf(file, "%lf ", vec[i]);
	}
	if (new_line)
	{
		fputc('\n', file);
	}
}

bool first_dominates_second(double *first, double *second, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		if (first[i] >= second[i])
		{
			return false;
		}
	}

	return true;
}

bool is_pareto_optimal(double *y, std::vector<double *> *all, int dim)
{
	for (std::vector<double *>::iterator it = all->begin(); it != all->end(); ++it)
	{
		if (first_dominates_second(*it, y, dim))
		{
			return false;
		}
	}
	return true;
}

void set_pareto_optimals(std::vector<double *> *all, std::vector<double *> *pareto, int dim)
{
	pareto->clear();

	for (std::vector<double *>::iterator it = all->begin(); it != all->end(); ++it)
	{
		if(is_pareto_optimal(*it, all, dim))
		{
			pareto->push_back(*it);
		}
	}
}


void log_guess(FILE *log, double *x, int xdim, double *y, int ydim)
{
	fputc('[', log);
	for (int i=0; i<xdim; i++)
	{
		fprintf(log, "%lf ", x[i]);
	}
	fprintf(log, "] -> [");
	for (int i=0; i<ydim; i++)
	{
		fprintf(log, "%lf ", y[i]);
	}
	fputc(']', log);
	fputc('\n', log);
}










