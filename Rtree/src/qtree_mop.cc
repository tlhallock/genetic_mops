/*
 * qtree_mop.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: thallock
 */


#include "qtree.h"


static bool qtree_branch_is_optimal(void *ptr, qtree_type type, qtree_point *point, int dim, int two_2_dim)
{
	if (type == QTREE_TYPE_NULL)
	{
		return true;
	}

	if (type == QTREE_TYPE_LEAF)
	{
		qtree_leaf *leaf = (qtree_leaf *) ptr;

		for (int i = 0; i < BRANCH_FACTOR; i++)
		{
			if (qtree_point_dominates(leaf->points[i], point, dim))
			{
				return false;
			}
		}
		return true;
	}

	// type == QTREE_TYPE_BRANCH
	qtree_branch *branch = (qtree_branch *) ptr;

	// prune regions this point dominates...
	if (qtree_point_dominates(point, branch->lb, dim))
	{
		return true;
	}

	// this could be done in a better fashion
	for (int i = 0; i < two_2_dim; i++)
	{
		if (!qtree_branch_is_optimal(branch->branches[i], branch->types[i], point, dim, two_2_dim))
		{
			return false;
		}
	}

	return true;
}


bool qtree_is_optimal(qtree *tree, qtree_point *point)
{
	return qtree_branch_is_optimal(tree->root, QTREE_TYPE_BRANCH, point, tree->dim, tree->two_2_dim);
}


static int qtree_branch_count_dominating(void *ptr, qtree_type type, qtree_point *point, int dim, int two_2_dim)
{
	if (type == QTREE_TYPE_NULL)
	{
		return true;
	}

	if (type == QTREE_TYPE_LEAF)
	{
		qtree_leaf *leaf = (qtree_leaf *) ptr;

		int count = 0;
		for (int i = 0; i < BRANCH_FACTOR; i++)
		{
			if (qtree_point_dominates(leaf->points[i], point, dim))
			{
				count++;
			}
		}
		return count;
	}

	// type == QTREE_TYPE_BRANCH
	qtree_branch *branch = (qtree_branch *) ptr;

	// prune regions this point dominates...
	if (qtree_point_dominates(point, branch->lb, dim))
	{
		return 0;
	}

	int count = 0;
	// this could be done in a better fashion
	for (int i = 0; i < two_2_dim; i++)
	{
		count += qtree_branch_count_dominating(branch->branches[i], branch->types[i], point, dim, two_2_dim);
	}
	return count;
}

int qtree_count_dominating(qtree *tree, qtree_point *point)
{
	return qtree_branch_count_dominating(tree->root, QTREE_TYPE_BRANCH, point, tree->dim, tree->two_2_dim);
}


