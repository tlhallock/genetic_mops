/*
 * QtreeLeaf.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */


#include "Qtree.h"

#include <float.h>


namespace qtree
{

QtreeLeaf::QtreeLeaf(QtreeBranch* _parent, int _dim, int _two_2_dim) :
		size(0),
		parent(_parent),
		dim(_dim),
		two_2_dim(_two_2_dim)
{
	for (int i = 0; i < BRANCH_FACTOR; i++)
	{
		points[i] = qtree_point_new(dim);
	}
}

QtreeLeaf::~QtreeLeaf()
{
	for (int i=0;i<BRANCH_FACTOR;i++)
	{
		qtree_point_del(points[i]);
	}
}

void QtreeLeaf::get_min(double* y_out, int dim_of_interest)
{
	for (int i = 0; i < BRANCH_FACTOR; i++)
	{
		if (y_out[dim_of_interest] < points[i][dim_of_interest])
		{
			for (int j = 0; j < get_dim(); j++)
			{
				y_out[j] = points[i][j];
			}
		}
	}
}

void QtreeLeaf::apply(void (*fctn)(qtree_point* pnt, void* arg), void* arg)
{
	int i;
	for(i=0;i<size;i++)
	{
		fctn(points[i], arg);
	}

}

int QtreeLeaf::index_of(qtree_point* point)
{
	for (int i=0; i<size; i++)
	{
		if (qtree_point_equals(points[i], point, get_dim()))
		{
			return i;
		}
	}

	return -19571938;
}

int QtreeLeaf::get_parents_quad()
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		if (parent->branches[i] == this)
		{
			return i;
		}
	}
	return -587658765;

}

void QtreeLeaf::print(FILE* out, int depth)
{
	int i, j;

	for(j=0; j<depth; j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "leaf d=%d s=%d:\n", depth, size);

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < depth + 1; j++)
		{
			fputc('\t', out);
		}
		qtree_point_print(out, points[i], get_dim(), true);
	}
}

void QtreeLeaf::assign(qtree_point *point)
{
	int osize = size++;
	for (int i = 0; i < get_dim(); i++)
	{
		points[osize][i] = point[i];
	}
}


void QtreeLeaf::find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *), double *cmin)
{
	for (int i = 0; i < get_size(); i++)
	{
		if (qtree_point_equals(point, points[i], get_dim()))
		{
			continue;
		}
		double distance = norm(point, points[i]);

		if (!IS_ZERO(distance) && distance < *cmin)
		{
			*cmin = distance;
			qtree_point_assign(out, points[i], get_dim());
		}
	}
}

}
