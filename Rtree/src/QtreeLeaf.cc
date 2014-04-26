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
		lb(qtree_point_new(_dim)),
		ub(qtree_point_new(_dim)),
		parent(_parent),
		dim(_dim),
		two_2_dim(_two_2_dim)
{
	for (int i = 0; i < BRANCH_FACTOR; i++)
	{
		points[i] = qtree_point_new(dim);
		refs[i] = NULL;
	}

	int q = get_parents_quad();
	for (int i = 0; i < _two_2_dim; i++)
	{
		double mid = (_parent->lb[i] + _parent->ub[i]) / 2;
		if (q & (1 << i))
		{
			lb[i] = _parent->lb[i];
			ub[i] = mid;
		}
		else
		{
			lb[i] = mid;
			ub[i] = _parent->ub[i];
		}
	}
}

QtreeLeaf::~QtreeLeaf()
{
	for (int i = 0; i < BRANCH_FACTOR; i++)
	{
		qtree_point_del(points[i]);
	}
	qtree_point_del(lb);
	qtree_point_del(ub);
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

void QtreeLeaf::apply(void (*fctn)(qtree_point* pnt, void *ref, void* arg), void* arg)
{
	int i;
	for (i = 0; i < size; i++)
	{
		fctn(points[i], refs[i], arg);
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

void *QtreeLeaf::get_ref(int index)
{
	return refs[index];
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

void QtreeLeaf::assign(qtree_point *point, void *ref)
{
	int osize = size++;
	for (int i = 0; i < get_dim(); i++)
	{
		points[osize][i] = point[i];
	}
	refs[osize] = ref;
}


bool QtreeLeaf::find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int),
		double *cmin, qtree_point *clb, qtree_point *cub)
{
	if (!qtree_region_contains(lb, ub, point, get_dim()))
	{
		return false;
	}
	if (!qtree_regions_are_adjacent(lb, ub, clb, cub, get_dim()))
	{
		return false;
	}

	bool ret_val = false;
	for (int i = 0; i < get_size(); i++)
	{
		if (qtree_point_equals(point, points[i], get_dim()))
		{
			continue;
		}

		double distance = norm(point, points[i], get_dim());

		if (IS_ZERO(distance))
		{
			continue;
		}

		ret_val = true;

		if (distance < *cmin)
		{
			*cmin = distance;
			qtree_point_assign(out, points[i], get_dim());
		}
	}

	return ret_val;
}

}
