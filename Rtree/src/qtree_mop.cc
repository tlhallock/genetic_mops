/*
 * qtree_mop.cc
 *
 *  
 *      Author: thallock
 */


#include "Qtree.h"
#include <stdlib.h>


namespace qtree
{

bool QtreeLeaf::is_pareto(qtree_point *point)
{
	for (int i = 0; i < size; i++)
	{
		if (qtree_point_dominates(points[i], point, get_dim()))
		{
			return false;
		}
	}
	return true;
}

bool QtreeBranch::is_pareto(qtree_point *point)
{
	if (qtree_point_dominates(ub, point, get_dim()))
	{
		return is_empty();
	}

	if (qtree_point_dominates(point, lb, get_dim()))
	{
		return true;
	}

	for (int i=0;i<get_two_2_dim();i++)
	{
		switch (types[i])
		{
		case QTREE_TYPE_NULL:
			continue;
		case QTREE_TYPE_LEAF:
			if (!((QtreeLeaf *)branches[i])->is_pareto(point))
			{
				return false;
			}
			continue;
		case QTREE_TYPE_BRANCH:
			if (!((QtreeBranch *) branches[i])->is_pareto(point))
			{
				return false;
			}
		}
	}

	return true;
}


bool Qtree::is_pareto(qtree_point *point)
{
	if (!in_current_bounds(point))
	{
		printf("Not implemented 1057254720");
		exit(1);
	}

	return root->is_pareto(point);
}

int QtreeLeaf::count_dominating(qtree_point *point)
{
	int count = 0;
	for (int i=0;i<size;i++)
	{
		if (qtree_point_dominates(points[i], point, get_dim()))
		{
			++count;
		}
	}
	return count;
}

int QtreeBranch::count_dominating(qtree_point *point)
{
	if (qtree_point_dominates(ub, point, get_dim()))
	{
		return recount();
	}

	if (qtree_point_dominates(point, lb, get_dim()))
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		switch (types[i])
		{
		case QTREE_TYPE_NULL:
			break;
		case QTREE_TYPE_LEAF:
			count += ((QtreeLeaf *) branches[i])->count_dominating(point);
			break;
		case QTREE_TYPE_BRANCH:
			count += ((QtreeBranch *) branches[i])->count_dominating(point);
			break;
		}
	}
	return count;
}

int Qtree::count_dominating(qtree_point *point)
{
	return root->count_dominating(point);
}

}
