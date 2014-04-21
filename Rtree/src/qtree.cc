/*
 * qtree.c
 *
 *  
 *      Author: rever
 */

#include "qtree.h"

#include "qtree_point.h"

#include <stdlib.h>
#include <stdbool.h>


namespace qtree
{
QtreeBranch* Qtree::qtree_root_grow(QtreeBranch* branch, qtree_point* direction, int dim, int two_2_dim)
{
	qtree_point *new_lower = qtree_point_new0(dim);
	qtree_point *new_upper = qtree_point_new0(dim);

	int old_quad = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = branch->ub[i] - branch->lb[i];
		if (direction[i] > branch->ub[i])
		{
			// grow upper...
			new_lower[i] = branch->lb[i];
			new_upper[i] = branch->ub[i] + diff;

			old_quad |= (1 << i);
		}
		else
		{
			// grow lower...
			new_lower[i] = branch->lb[i] - diff;
			new_upper[i] = branch->ub[i];
		}
	}

	qtree_branch * ret_val = qtree_branch_new(NULL, new_lower, new_upper, two_2_dim);
	ret_val->types[old_quad] = QTREE_TYPE_BRANCH;
	ret_val->branches[old_quad] = branch;
	branch->parent = ret_val;

	return ret_val;
}

bool Qtree::in_bounds(qtree* tree, qtree_point* point)
{
	for (int i = 0; i < tree->dim; i++)
	{
		if (point[i] < tree->lb[i])
		{
			return false;
		}
		if (point[i] > tree->ub[i])
		{
			return false;
		}
	}
	return true;
}

QtreeLeaf* Qtree::find(qtree* tree, qtree_point* point)
{
	if (ERROR_CHECKING)
	{
		for (int i=0;i<tree->dim;i++)
		{
			if (point[i] < tree->lb[i])
			{
				puts("Error 20827605");
				exit(1);
			}
			if (point[i] > tree->ub[i])
			{
				puts("Error 20agdsg605");
				exit(1);
			}
		}
	}

	return qtree_branch_find(tree->root, point, tree->dim);
}

qtree* Qtree::Qtree(qtree_point* lb, qtree_point* ub, int dim)
{
	qtree *ret_val = (qtree *) malloc (sizeof(*ret_val));

	ret_val->lb = qtree_point_dup(dim, lb);
	ret_val->ub = qtree_point_dup(dim, ub);

	ret_val->dim = dim;
	ret_val->two_2_dim = 1 << dim;

	ret_val->root = qtree_branch_new(NULL,
			qtree_point_dup(ret_val->dim, ret_val->lb),
			qtree_point_dup(ret_val->dim, ret_val->ub),
			ret_val->two_2_dim);

	return ret_val;
}

void Qtree::~Qtree()
{
	qtree_branch_del(tree->root, tree->two_2_dim);
	qtree_point_del(tree->lb);
	qtree_point_del(tree->ub);
	free(tree);
}

bool Qtree::add(qtree_point* point)
{
	while (!qtree_in_bounds(tree, point))
	{
		tree->root = qtree_root_grow(tree->root, point, tree->dim, tree->two_2_dim);
	}

	qtree_leaf *leaf = qtree_branch_find(tree->root, point, tree->dim);

	if (qtree_leaf_index_of(leaf, point, tree->dim) >= 0)
	{
		return false;
	}

	while (leaf->size == BRANCH_FACTOR)
	{
		qtree_branch *parent = leaf->parent;
		int quad = qtree_leaf_get_parents_quad(leaf, tree->two_2_dim);

		qtree_point *new_lb = qtree_point_dup(tree->dim, parent->lb);
		qtree_point *new_ub = qtree_point_dup(tree->dim, parent->ub);
		qtree_bounds_select(new_lb, new_ub, quad, tree->dim);

		qtree_branch *new_branch = qtree_branch_new(parent, new_lb, new_ub, tree->two_2_dim);

		parent->types[quad] = QTREE_TYPE_BRANCH;
		parent->branches[quad] = new_branch;

		for (int i=0; i<BRANCH_FACTOR; i++)
		{
			quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, leaf->points[i], tree->dim);
			qtree_leaf *new_leaf = qtree_branch_get_leaf(new_branch, quad, tree->dim);
			qtree_point_assign(new_leaf, leaf->points[i], tree->dim);
		}

		qtree_leaf_del(leaf);

		quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, point, tree->dim);
		leaf = (qtree_leaf *) qtree_branch_get_leaf(new_branch, quad, tree->dim);
	}

	qtree_point_assign(leaf, point, tree->dim);
	return true;
}

void Qtree::clear()
{
	qtree_branch_del(tree->root, tree->two_2_dim);
	tree->root = qtree_branch_new(NULL,
			qtree_point_dup(tree->dim, tree->lb),
			qtree_point_dup(tree->dim, tree->ub),
			tree->two_2_dim);
}

bool Qtree::remove(qtree_point* point)
{
	qtree_leaf *leaf = qtree_branch_find(tree->root, point, tree->dim);

	int index = qtree_leaf_index_of(leaf, point, tree->dim);
	if (index < 0)
	{
		return false;
	}

	int size = --leaf->size;

	if (size != index)
	{
		qtree_point *tmp = leaf->points[index];
		leaf->points[index] = leaf->points[size];
		leaf->points[size] = tmp;
	}

	if (size != 0)
	{
		return true;
	}

	// clean up
	qtree_branch *parent = leaf->parent;
	int quad = qtree_leaf_get_parents_quad(leaf, tree->two_2_dim);
	parent->types[quad] = QTREE_TYPE_NULL;
	parent->branches[quad] = NULL;

	qtree_leaf_del(leaf);

	qtree_branch *node = parent;
	while ((parent = node->parent) != NULL
			&& qtree_branch_is_empty(node, tree->two_2_dim))
	{
		int quad = qtree_branch_get_parents_quad(node, tree->two_2_dim);
		parent->types[quad] = QTREE_TYPE_NULL;
		parent->branches[quad] = NULL;
		qtree_branch_del(node, tree->two_2_dim);
		node = parent;
	}

	return true;
}

int Qtree::count()
{
	return qtree_branch_count(tree->root, tree->two_2_dim);
}

bool Qtree::contains(qtree_point* point)
{
	qtree_leaf *leaf = qtree_find(tree, point);
	return qtree_leaf_index_of(leaf, point, tree->dim) >= 0;
}

void Qtree::get_min(double* y_out, int dim)
{
	qtree_branch_get_min(tree->root, y_out, dim, tree->dim, tree->two_2_dim);
}

void Qtree::apply(void (*fctn)(qtree_point* pnt, void* arg), void* arg)
{
	qtree_branch_apply(tree->root, fctn, arg, tree->two_2_dim);
}

size_t Qtree::get_memory_usage()
{
}

qtree_it* Qtree::it_new()
{
}

void Qtree::print(FILE* out)
{
	root->print(out, 0);
}
