/*
 * QtreeLeaf.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */


#include "Qtree.h"

#include <float.h>
#include <stdlib.h>


namespace qtree
{

QtreeLeaf::QtreeLeaf(QtreeBranch* _parent, qtree_point *_lb, qtree_point *_ub) :
		tree(_parent->get_tree()),
		size(0),
		lb(_lb),
		ub(_ub),
		parent(_parent),
		points((qtree_point **) malloc(sizeof(*points) * _parent->get_tree()->get_branch_factor())),
		refs((void **) malloc(sizeof(*points) * _parent->get_tree()->get_branch_factor()))
{
	for (int i = 0; i < get_branch_factor(); i++)
	{
		points[i] = qtree_point_new(get_dim());
		refs[i] = NULL;
	}
}

QtreeLeaf::~QtreeLeaf()
{
	for (int i = 0; i < get_branch_factor(); i++)
	{
		qtree_point_del(points[i]);
	}
	qtree_point_del(lb);
	qtree_point_del(ub);
	free(points);
	free(refs);
}


int QtreeLeaf::get_dim() { return tree->get_dim(); }
int QtreeLeaf::get_two_2_dim() { return tree->get_two_2_dim(); }
int QtreeLeaf::get_branch_factor() { return tree->get_branch_factor(); }

void QtreeLeaf::get_min(double* y_out, int dim_of_interest)
{
	for (int i = 0; i < get_branch_factor(); i++)
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

void QtreeLeaf::verify()
{
	for (int i = 0; i < size; i++)
	{
		if (!qtree_region_contains(lb, ub, points[i], get_dim()))
		{
			puts("Error 54432198760");
			exit(1);
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
	fprintf(out, "leaf: %p d=%d s=%d lb=", this, depth, size);
	qtree_point_print(out, lb, get_dim(), false);
	fprintf(out, "; ub=");
	qtree_point_print(out, ub, get_dim(), true);

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
		double *cmin, qtree_point *clb, qtree_point *cub, bool can_prune)
{
	if (can_prune && !qtree_region_contains(lb, ub, point, get_dim()) &&
			!qtree_regions_are_adjacent(lb, ub, clb, cub, get_dim()))
	{
		printf("Pruning %p\n", this);
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

bool QtreeLeaf::get_random(qtree_point *out)
{
	if (size == 0)
	{
		return false;
	}

	qtree_point_assign(out, points[rand() % size], get_dim());

	return true;
}

}
