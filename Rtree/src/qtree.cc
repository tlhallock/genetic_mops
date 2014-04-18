/*
 * qtree.c
 *
 *  Created on: Apr 3, 2014
 *      Author: rever
 */

#include "qtree.h"

#include "qtree_point.h"

#include <stdlib.h>
#include <stdbool.h>


static void qtree_leaf_print(FILE *out, qtree_leaf *tree, int depth, int dim)
{
	int i, j;

	for(j=0; j<depth; j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "leaf d=%d s=%d:\n", depth, tree->size);

	for (i=0;i<tree->size;i++)
	{
		for(j=0;j<depth+1;j++)
		{
			fputc('\t', out);
		}
		qtree_point_print(out, tree->points[i], dim, true);
	}
}


static void qtree_branch_print(FILE *out, qtree_branch *tree, int depth, int dim, int two_2_dim)
{
	int i, j;

	for (j = 0; j < depth; j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "branch d=%d:\n", depth / 2);

	for (i = 0; i < two_2_dim; i++)
	{
		char *type = NULL;
		switch (tree->types[i])
		{
		case QTREE_TYPE_BRANCH:
			type = "branch";
			break;
		case QTREE_TYPE_LEAF:
			type = "leaf";
			break;
		case QTREE_TYPE_NULL:
			type = "null";
			break;
		}

		for (j = 0; j < depth + 1; j++)
		{
			fputc('\t', out);
		}

		fprintf(out, "%d: %s\n", i, type);

		switch(tree->types[i])
		{
		case QTREE_TYPE_BRANCH:
			qtree_branch_print(out, (qtree_branch *) tree->branches[i], depth + 2, dim, two_2_dim);
			break;
		case QTREE_TYPE_LEAF:
			qtree_leaf_print(out, (qtree_leaf *) tree->branches[i], depth + 2, dim);
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}
}

void qtree_print(FILE *out, qtree *tree)
{
	qtree_branch_print(out, tree->root, 0, tree->dim, tree->two_2_dim);
}


static qtree_leaf *qtree_leaf_new(qtree_branch *parent, int dim)
{
	qtree_leaf *ret_val = (qtree_leaf *) malloc (sizeof (*ret_val));

	ret_val->size = 0;
	ret_val->parent = parent;

	for (int i=0;i<BRANCH_FACTOR;i++)
	{
		ret_val->points[i] = qtree_point_new0(dim);
	}

	return ret_val;
}

static void qtree_leaf_del(qtree_leaf *leaf)
{
	for (int i=0;i<BRANCH_FACTOR;i++)
	{
		free(leaf->points[i]);
	}
	free(leaf);
}

// the points are kept...
static qtree_branch *qtree_branch_new(qtree_branch *parent, qtree_point *lb, qtree_point *ub, int two_2_dim)
{
	qtree_branch *ret_val = (qtree_branch *) malloc (sizeof (*ret_val));
	ret_val->parent = parent;

	ret_val->lb = lb;
	ret_val->ub = ub;

	ret_val->types = (qtree_type *) malloc (sizeof (*ret_val->types) * two_2_dim);
	ret_val->branches = (void **) malloc (sizeof (*ret_val->branches) * two_2_dim);

	int i;
	for (i=0; i<two_2_dim; i++)
	{
		ret_val->types[i] = QTREE_TYPE_NULL;
		ret_val->branches[i] = NULL;
	}

	return ret_val;
}

static void qtree_branch_del(qtree_branch *branch, int two_2_dim)
{
	int i;
	for (i=0; i<two_2_dim; i++)
	{
		qtree_type type = branch->types[i];
		if (type == QTREE_TYPE_BRANCH)
		{
			qtree_branch_del((qtree_branch *) branch->branches[i], two_2_dim);
		}
		else if (type == QTREE_TYPE_LEAF)
		{
			qtree_leaf_del((qtree_leaf *) branch->branches[i]);
		}
		else if (type == QTREE_TYPE_NULL)
		{
			continue;
		}
		else
		{
			puts("Error 195493955");
		}
	}

	free(branch->types);
	free(branch->branches);
	free(branch);
}

qtree *qtree_new(qtree_point *lb, qtree_point *ub, int dim)
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

void qtree_del(qtree *tree)
{
	qtree_branch_del(tree->root, tree->two_2_dim);
	qtree_point_del(tree->lb);
	qtree_point_del(tree->ub);
	free(tree);
}


int qtree_get_quadrant(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim)
{
	int quad = 0;
	int i;
	for (i=0;i<dim;i++)
	{
		double mid = (ub[i] - lb[i]) / 2;
		if (point[i] > mid)
		{
			quad |= (1 << i);
		}

		if (ERROR_CHECKING)
		{
			if (point[i] < lb[i])
			{
				puts("Error 104801");
				exit (1);
			}
			if (point[i] > ub[i])
			{
				puts("Error 10472994");
				exit (1);
			}
		}
	}
	return quad;
}

qtree_leaf *qtree_branch_get_leaf(qtree_branch *branch, int quadrant, int dim)
{
	int type = branch->types[quadrant];
	if (type == QTREE_TYPE_LEAF)
	{
		return (qtree_leaf *) branch->branches[quadrant];
	}
	else if (type == QTREE_TYPE_NULL)
	{
		qtree_leaf *new_leaf = qtree_leaf_new(branch, dim);
		branch->types[quadrant] = QTREE_TYPE_LEAF;
		branch->branches[quadrant] = new_leaf;

		return new_leaf;
	}
	else
	{
		puts("Error 204810463");
		exit(1);
	}
}

static qtree_leaf *qtree_branch_find(qtree_branch *branch, qtree_point *point, int dim)
{
	int quadrant;
	qtree_type type = QTREE_TYPE_BRANCH;

	for(;;)
	{
		quadrant = qtree_get_quadrant(branch->lb, branch->ub, point, dim);
		type = ((qtree_branch *) branch)->types[quadrant];

		if (type != QTREE_TYPE_BRANCH)
		{
			break;
		}

		branch = (qtree_branch *) branch->branches[quadrant];
	}

	return qtree_branch_get_leaf(branch, quadrant, dim);
}

static void qtree_point_assign(qtree_leaf *leaf, qtree_point *point, int dim)
{
	int size = leaf->size++;
	for (int i = 0; i < dim; i++)
	{
		leaf->points[size][i] = point[i];
	}
}

static int qtree_leaf_index_of(qtree_leaf *leaf, qtree_point *point, int dim)
{
	for (int i=0; i<leaf->size; i++)
	{
		if (qtree_point_equals(leaf->points[i], point, dim))
		{
			return i;
		}
	}

	return -19571938;
}

static int qtree_leaf_get_parents_quad(qtree_leaf *leaf, int two_2_dim)
{
	qtree_branch *parent = leaf->parent;
	for (int i=0;i<two_2_dim;i++)
	{
		if (parent->branches[i] == leaf)
		{
			return i;
		}
	}
	return -587658765;
}

static int qtree_branch_get_parents_quad(qtree_branch *branch, int two_2_dim)
{
	qtree_branch *parent = branch->parent;
	if (parent == NULL)
	{
		puts("Error 15285298750");
		exit(1);
	}

	for (int i = 0; i < two_2_dim; i++)
	{
		if (parent->branches[i] == branch)
		{
			return i;
		}
	}
	return -587658765;
}

void qtree_bounds_select(qtree_point *lb, qtree_point *ub, int quad, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		double mid = (lb[i] + ub[i]) / 2;

		if (i & (1 << i))
		{
			lb[i] = mid;
		}
		else
		{
			ub[i] = mid;
		}
	}
}

bool qtree_add(qtree *tree, qtree_point *point)
{
	double width = tree->width;
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

		leaf = (qtree_leaf *) qtree_branch_get_leaf(new_branch, quad, tree->dim);
	}

	qtree_point_assign(leaf, point, tree->dim);
	return true;
}

bool qtree_branch_is_empty(qtree_branch *branch, int two_2_dim)
{
	for (int i = 0; i < two_2_dim; i++)
	{
		qtree_type type = branch->types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			if (!qtree_branch_is_empty((qtree_branch *) branch->branches[i], two_2_dim))
			{
				return false;
			}
			break;
		case QTREE_TYPE_LEAF:
			if (((qtree_leaf *) branch->branches[i])->size != 0)
			{
				return false;
			}
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return true;
}

bool qtree_remove(qtree *tree, qtree_point *point)
{
	double width = tree->width;
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

	while (parent->parent != NULL
			&& qtree_branch_is_empty(parent, tree->two_2_dim))
	{
		int quad = qtree_branch_get_parents_quad(parent, tree->two_2_dim);
		parent->parent->types[quad] = QTREE_TYPE_NULL;
		parent->parent->branches[quad] = NULL;
		qtree_branch_del(parent, tree->two_2_dim);
	}

	return true;
}

qtree_it *qtree_it_new(qtree *tree)
{
//	qtree_it *ret_val = (qtree_it *) malloc (sizeof (*ret_val));
//
//	ret_val->pos_size = 128;
//	ret_val->branch_locs = (int *) malloc (sizeof *ret_val->branch_locs);
//
//	ret_val->depth = 0;
//	qtree_branch *b = tree->root;
//	while (b->types[0] == QTREE_TYPE_BRANCH)
//	{
//		ret_val->branch_locs[ret_val->depth++] = 0;
//		b = (qtree_branch *) b->branches[0];
//
//		if (ret_val->depth > 128)
//		{
//			puts("Error 7210720570");
//		}
//	}
//
//	if (b->types[0] == QTREE_TYPE_NULL)
//	{
//		b->types[0] = QTREE_TYPE_LEAF;
//		b->branches[0] = qtree_leaf_new(b);
//	}
//
//	ret_val->leaf = (qtree_leaf *) b->branches[0];
//
//	return ret_val;
	return NULL;
}

qtree_leaf *qtree_find(qtree *tree, qtree_point *point)
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
				puts("Error 20827605");
				exit(1);
			}
		}
	}

	return qtree_branch_find(tree->root, point, tree->dim);
}


bool qtree_contains(qtree *tree, qtree_point *point)
{
	qtree_point lower_left_out;
	double width_out;
	qtree_leaf *leaf = qtree_find(tree, point);
	return qtree_leaf_index_of(leaf, point, tree->dim) >= 0;
}

int qtree_branch_count(qtree_branch *branch, int two_2_dim)
{
	int count = 0;

	for (int i = 0; i < two_2_dim; i++)
	{
		qtree_type type = branch->types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			count += qtree_branch_count((qtree_branch *) branch->branches[i], two_2_dim);
			break;
		case QTREE_TYPE_LEAF:
			count += ((qtree_leaf *) branch->branches[i])->size;
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return count;
}
