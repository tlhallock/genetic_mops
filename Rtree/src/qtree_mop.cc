/*
 * qtree_mop.cc
 *
 *  
 *      Author: thallock
 */


#include "Qtree.h"


namespace qtree
{

bool QtreeLeaf::is_pareto(qtree_point *point)
{
	for (int i = 0; i < size; i++)
	{
		if (qtree_point_dominates(points[i], point, dim))
		{
			return false;
		}
	}
	return true;
}

bool QtreeBranch::is_pareto(qtree_point *point)
{
	if (qtree_point_dominates(ub, point, dim))
	{
		return is_empty();
	}

	if (qtree_point_dominates(point, lb, dim))
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
	if (qtree_point_dominates(ub, point, dim))
	{
		return count();
	}

	if (qtree_point_dominates(point, lb, dim))
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
