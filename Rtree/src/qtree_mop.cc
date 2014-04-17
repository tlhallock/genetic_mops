/*
 * qtree_mop.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: thallock
 */


#include "qtree.h"

bool qtree_branch_is_empty(qtree_branch *branch);
int qtree_branch_count(qtree_branch *branch);


static bool qtree_is_pareto(void *ptr, qtree_type type, qtree_point *point, double llx, double lly, double urx, double ury)
{
	if (type == QTREE_TYPE_NULL)
	{
		return true;
	}

	bool all_children_more_optimal = false;
	bool within_x = llx < point->x && point->x < urx;
	bool within_y = lly < point->y && point->y < ury;
	if (!within_x && !within_y)
	{
		if (urx < point->x && ury < point->y)
		{
			all_children_more_optimal = true;
		}
		else if(llx > point->x && lly > point->y)
		{
			return true;
		}
	}

	if (type == QTREE_TYPE_LEAF)
	{
		qtree_leaf *leaf = (qtree_leaf *)ptr;
		if (all_children_more_optimal)
		{
			return leaf->size == 0;
		}

		for (int i=0;i<BRANCH_FACTOR;i++)
		{
			if (leaf->points[i].x > point->x)
			{
				continue;
			}
			if (leaf->points[i].y > point->y)
			{
				continue;
			}
			return false;
		}
		return true;
	}

	// type == QTREE_TYPE_BRANCH
	qtree_branch *branch = (qtree_branch *) ptr;
	if (all_children_more_optimal)
	{
		return qtree_branch_is_empty(branch);
	}

	double n_llx;
	double n_lly;
	double n_urx;
	double n_ury;
	bool is_pareto;
	double width = urx - llx;

	void *n_ptr;

//	LOWER_LEFT:
	n_llx = llx;
	n_lly = lly;
	n_urx = urx - width / 2;
	n_ury = ury - width / 2;
	type = branch->types[LOWER_LEFT];
	n_ptr = branch->branches[LOWER_LEFT];
	is_pareto = qtree_is_pareto(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);
	if (!is_pareto)
	{
		return false;
	}
//	UPPER_LEFT:
	n_llx = llx;
	n_lly = lly + width / 2;
	n_urx = urx - width / 2;
	n_ury = ury;
	type = branch->types[UPPER_LEFT];
	n_ptr = branch->branches[UPPER_LEFT];
	is_pareto = qtree_is_pareto(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);
	if (!is_pareto)
	{
		return false;
	}
//	LOWER_RIGHT:
	n_llx = llx + width / 2;
	n_lly = lly;
	n_urx = urx;
	n_ury = ury - width / 2;
	type = branch->types[LOWER_RIGHT];
	n_ptr = branch->branches[LOWER_RIGHT];
	is_pareto = qtree_is_pareto(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);
	if (!is_pareto)
	{
		return false;
	}

//	UPPER_RIGHT:
	n_llx = llx + width / 2;
	n_lly = lly + width / 2;
	n_urx = urx;
	n_ury = ury;
	type = branch->types[UPPER_RIGHT];
	n_ptr = branch->branches[UPPER_RIGHT];
	is_pareto = qtree_is_pareto(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);
	if (!is_pareto)
	{
		return false;
	}

	return true;
}


static int count_dominating(void *ptr, qtree_type type, qtree_point *point, double llx, double lly, double urx, double ury)
{
	if (type == QTREE_TYPE_NULL)
	{
		return 0;
	}

	bool all_children_more_optimal = false;
	bool within_x = llx < point->x && point->x < urx;
	bool within_y = lly < point->y && point->y < ury;
	if (!within_x && !within_y)
	{
		if (urx < point->x && ury < point->y)
		{
			all_children_more_optimal = true;
		}
		else if(llx > point->x && lly > point->y)
		{
			return 0;
		}
	}

	if (type == QTREE_TYPE_LEAF)
	{
		qtree_leaf *leaf = (qtree_leaf *)ptr;
		if (all_children_more_optimal)
		{
			return leaf->size;
		}

		int count = 0;
		for (int i=0;i<BRANCH_FACTOR;i++)
		{
			if (leaf->points[i].x > point->x)
			{
				continue;
			}
			if (leaf->points[i].y > point->y)
			{
				continue;
			}
			count++;
		}
		return count;
	}

	// type == QTREE_TYPE_BRANCH
	qtree_branch *branch = (qtree_branch *) ptr;
	if (all_children_more_optimal)
	{
		return qtree_branch_count(branch);
	}

	int count = 0;
	double n_llx;
	double n_lly;
	double n_urx;
	double n_ury;
	double width = urx - llx;

	void *n_ptr;

//	LOWER_LEFT:
	n_llx = llx;
	n_lly = lly;
	n_urx = urx - width / 2;
	n_ury = ury - width / 2;
	type = branch->types[LOWER_LEFT];
	n_ptr = branch->branches[LOWER_LEFT];
	count += count_dominating(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);

//	UPPER_LEFT:
	n_llx = llx;
	n_lly = lly + width / 2;
	n_urx = urx - width / 2;
	n_ury = ury;
	type = branch->types[UPPER_LEFT];
	n_ptr = branch->branches[UPPER_LEFT];
	count += count_dominating(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);

//	LOWER_RIGHT:
	n_llx = llx + width / 2;
	n_lly = lly;
	n_urx = urx;
	n_ury = ury - width / 2;
	type = branch->types[LOWER_RIGHT];
	n_ptr = branch->branches[LOWER_RIGHT];
	count += count_dominating(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);


//	UPPER_RIGHT:
	n_llx = llx + width / 2;
	n_lly = lly + width / 2;
	n_urx = urx;
	n_ury = ury;
	type = branch->types[UPPER_RIGHT];
	n_ptr = branch->branches[UPPER_RIGHT];
	count += count_dominating(n_ptr, type, point, n_llx, n_lly, n_urx, n_ury);

	return count;
}




