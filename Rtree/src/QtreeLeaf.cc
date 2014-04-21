/*
 * QtreeLeaf.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */


#include "qtree.h"


namespace qtree
{

int QtreeLeaf::get_dim()
{
}

int QtreeLeaf::get_two_2_dim()
{
}

QtreeLeaf::QtreeLeaf(QtreeBranch* _parent, int dim)
{
	size = 0;
	parent = _parent;

	for (int i=0;i<BRANCH_FACTOR;i++)
	{
		points[i] = qtree_point_new0(dim);
	}
}

QtreeLeaf::~QtreeLeaf()
{
	for (int i=0;i<BRANCH_FACTOR;i++)
	{
		qtree_point_del(points[i]);
	}
}

void QtreeLeaf::qtree_leaf_get_min(double* y_out, int dim_of_interest)
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

void QtreeLeaf::qtree_leaf_apply(void (*fctn)(qtree_point* pnt, void* arg), void* arg)
{
	int i;
	for(i=0;i<size;i++)
	{
		fctn(points[i], arg);
	}

}

int QtreeLeaf::qtree_leaf_index_of(qtree_point* point)
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

int QtreeLeaf::qtree_leaf_get_parents_quad()
{
	QtreeBranch *parent = parent;
	for (int i=0;i<get_two_2_dim();i++)
	{
		if (parent->branches[i] == this)
		{
			return i;
		}
	}
	return -587658765;

}

void QtreeLeaf::qtree_leaf_print(FILE* out, int depth)
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

}
