/*
 * qtree_point.cc
 *
 *  
 *      Author: rever
 */

#include "config.h"
#include "qtree_point.h"

#include <limits.h>
#include <stdlib.h>


namespace qtree
{

void qtree_point_print(FILE *out, qtree_point *point, int dim, bool newline)
{
	int j;
	for (j = 0; j < dim; j++)
	{
		fprintf(out, "%lf ", point[j]);
	}
	if (newline)
	{
		fputc('\n', out);
	}
}


qtree_point *qtree_point_new(int dim, double *vals)
{
	qtree_point *ret_val = qtree_point_new0(dim);

	for (int i = 0; i < dim; i++)
	{
		ret_val[i] = vals[i];
	}

	return ret_val;
}

qtree_point *qtree_point_dup(int dim, qtree_point *other)
{
	return qtree_point_new(dim, other);
}

qtree_point *qtree_point_new0(int dim)
{
	return (qtree_point *) malloc(sizeof(qtree_point) * dim);
}

void qtree_point_del(qtree_point *point)
{
	free(point);
}

bool qtree_point_equals(qtree_point *point1, qtree_point *point2, int dim)
{
	double diff;

	for (int i=0;i<dim;i++)
	{
		diff = point1[i] - point2[i];
		if (!IS_ZERO(diff))
		{
			return false;
		}

	}

	return true;
}

qtree_point *qtree_point_new_rand(int dim)
{
	qtree_point *ret = qtree_point_new0(dim);

	for (int i = 0; i < dim; i++)
	{
		ret[i] = rand() / (double) INT_MAX;
	}

	return ret;
}

bool qtree_point_dominates(qtree_point *point1, qtree_point *point2, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		if (point2[i] <= point1[i])
		{
			return false;
		}
	}

	return true;
}

}
