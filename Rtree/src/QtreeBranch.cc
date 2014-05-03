/*
 * QtreeBranch.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */



#include "Qtree.h"

#include <stdlib.h>
#include <limits.h>

namespace qtree
{

#define SQRT_TWO 1.41421356237

/*
 * should duplicate the points...
 */
QtreeBranch::QtreeBranch(Qtree *_tree, QtreeBranch* _parent, qtree_point* _lb, qtree_point* _ub) :
	tree(_tree),
	lb(qtree_point_dup(_tree->get_dim(), _lb)),
	ub(qtree_point_dup(_tree->get_dim(), _ub)),
	parent(_parent),
	types((qtree_type *) malloc (sizeof (*types) * _tree->get_two_2_dim())),
	branches((void **) malloc (sizeof (*branches) * _tree->get_two_2_dim())),
	count(0)
{
	for (int i = 0; i < _tree->get_two_2_dim(); i++)
	{
		types[i] = QTREE_TYPE_NULL;
		branches[i] = NULL;
	}
}

QtreeBranch::~QtreeBranch()
{
	int i;
	for (i = 0; i < get_two_2_dim(); i++)
	{
		switch (types[i])
		{
		case QTREE_TYPE_BRANCH:
			delete ((QtreeBranch *) branches[i]);
			break;
		case QTREE_TYPE_LEAF:
			delete (QtreeLeaf *) branches[i];
			break;
		case QTREE_TYPE_NULL:
			break;
		default:
			puts("Error 195493955");
			exit(1);
		}
	}

	free(lb);
	free(ub);
	free(types);
	free(branches);
}

int QtreeBranch::get_dim() { return tree->get_dim(); }
int QtreeBranch::get_two_2_dim() { return tree->get_two_2_dim(); }
int QtreeBranch::get_branch_factor() { return tree->get_branch_factor() ; }


int QtreeBranch::get_depth()
{
	if (parent == NULL)
	{
		return 0;
	}
	else
	{
		return parent->get_depth() + 1;
	}
}

void QtreeBranch::print(FILE* out, int depth)
{
	int i, j;

	for (j = 0; j < depth; j++)
	{
		fputc('\t', out);
	}


	qtree_point *mid = qtree_point_new(get_dim());
	for (int i = 0; i < get_dim(); i++)
	{
		mid[i] = (lb[i] + ub[i]) / 2;
	}

	fprintf(out, "branch %p: size=%d d=%d lb=", this, count, depth / 2);
	qtree_point_print(out, lb, get_dim(), false);
	fprintf(out, "; ub=");
	qtree_point_print(out, ub, get_dim(), false);
	fprintf(out, "; mid=");
	qtree_point_print(out, mid, get_dim(), false);
	fprintf(out, ";\n");

	qtree_point_del(mid);

	for (i = 0; i < get_two_2_dim(); i++)
	{
		const char *type = NULL;
		switch (types[i])
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

		fprintf(out, "%d (", i);
		qtree_print_quadrant(out, i, get_dim());
		fprintf(out, "): %s\n", type);


		switch(types[i])
		{
		case QTREE_TYPE_BRANCH:
			((QtreeBranch *) branches[i])->print(out, depth + 2);
			break;
		case QTREE_TYPE_LEAF:
			((QtreeLeaf *) branches[i])->print(out, depth + 2);
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

}

void QtreeBranch::get_min(double* y_out, int dim_of_interest)
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		if (i & (1 << dim_of_interest))
		{
			continue;
		}

		qtree_type type = types[i];
		switch (type)
		{
		case QTREE_TYPE_BRANCH:
			((QtreeBranch *) branches[i])->get_min(y_out, dim_of_interest);
			break;
		case QTREE_TYPE_LEAF:
			((QtreeLeaf *) branches[i])->get_min(y_out, dim_of_interest);
			break;
		case QTREE_TYPE_NULL:
			break;
		default:
			puts("Error 14721p854");
		}
	}

}

void QtreeBranch::apply(void (*fctn)(qtree_point* pnt, void *, void* arg), void* arg)
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_type type = types[i];
		switch (type)
		{
		case QTREE_TYPE_BRANCH:
			((QtreeBranch *) branches[i])->apply(fctn, arg);
			break;
		case QTREE_TYPE_LEAF:
			((QtreeLeaf *) branches[i])->apply(fctn, arg);
			break;
		case QTREE_TYPE_NULL:
			break;
		default:
			puts("Error 10656710874");
			exit(1);
		}
	}
}

int QtreeBranch::recount()
{
	int count = 0;

	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_type type = types[i];
		switch (type)
		{
		case QTREE_TYPE_BRANCH:
			count += ((QtreeBranch *) branches[i])->recount();
			break;
		case QTREE_TYPE_LEAF:
			count += ((QtreeLeaf *) branches[i])->size;
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return count;
}


void QtreeBranch::add_to_size(int inc)
{
	count += inc;
	if (parent != NULL)
	{
		parent->add_to_size(inc);
	}
}

static void qtree_point_select(int quad, qtree_point *lb, qtree_point *ub, qtree_point *nlb, qtree_point *nub, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		double mid = (lb[i] + ub[i]) / 2;
		if (quad & (1 << i))
		{
			nlb[i] = mid;
			nub[i] = ub[i];
		}
		else
		{
			nlb[i] = lb[i];
			nub[i] = mid;
		}
	}
}

QtreeLeaf* QtreeBranch::get_leaf(int quadrant)
{
	QtreeLeaf *new_leaf;

	qtree_point *nlb;
	qtree_point *nub;

	switch (types[quadrant])
	{
	case QTREE_TYPE_LEAF:
		return (QtreeLeaf *) branches[quadrant];
	case QTREE_TYPE_NULL:
		nlb = qtree_point_new(get_dim());
		nub = qtree_point_new(get_dim());

		qtree_point_select(quadrant, lb, ub, nlb, nub, get_dim());

		new_leaf = new QtreeLeaf(this, nlb, nub);
		set_branch(quadrant, QTREE_TYPE_LEAF, new_leaf);
		return new_leaf;
	case QTREE_TYPE_BRANCH:
	default:
		puts("Error 204810463");
		exit(1);
	}
	return NULL;
}

QtreeLeaf* QtreeBranch::find(qtree_point* point)
{
	int quadrant;
	qtree_type type = QTREE_TYPE_BRANCH;

	QtreeBranch *branch = this;
	for(;;)
	{
		quadrant = qtree_get_quadrant(branch->lb, branch->ub, point, get_dim());
		type = ((QtreeBranch *) branch)->types[quadrant];

		if (type != QTREE_TYPE_BRANCH)
		{
			break;
		}

		branch = (QtreeBranch *) branch->branches[quadrant];
	}

	return branch->get_leaf(quadrant);
}

int QtreeBranch::get_parents_quad()
{
	if (parent == NULL)
	{
		puts("Error 15285298750");
		exit(1);
	}

	for (int i = 0; i < get_two_2_dim(); i++)
	{
		if (parent->branches[i] == this)
		{
			return i;
		}
	}
	return -587658765;
}

bool QtreeBranch::is_empty()
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_type type = types[i];
		switch (type)
		{
		case QTREE_TYPE_BRANCH:
			if (!((QtreeBranch *) branches[i])->is_empty())
			{
				return false;
			}
			break;
		case QTREE_TYPE_LEAF:
			if (((QtreeLeaf *) branches[i])->size != 0)
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

bool QtreeBranch::find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int), double *cmin,
		qtree_point *clb, qtree_point *cub, int leaf_depth)
{
	int depth = get_depth();
	if (depth < leaf_depth && !qtree_region_contains(lb, ub, point, get_dim()) &&
			!qtree_regions_are_adjacent(lb, ub, clb, cub, get_dim()))
	{
		return false;
	}

	bool ret_val = false;

	for (int i = 0; i < get_two_2_dim(); i++)
	{
		switch(types[i])
		{
		case QTREE_TYPE_NULL:
			break;
		case QTREE_TYPE_LEAF:
			ret_val |= ((QtreeLeaf *) branches[i])->find_nearest(point, out, norm, cmin, clb, cub, depth + 1 < leaf_depth);
			break;
		case QTREE_TYPE_BRANCH:
			ret_val |= ((QtreeBranch *) branches[i])->find_nearest(point, out, norm, cmin, clb, cub, leaf_depth);
			break;
		}
	}

	return ret_val;
}

static int select_random(int *probs, int total_count, int length)
{
	if (total_count == 0)
	{
		return -15793;
	}

	int choice = rand() % total_count;
	for (int i = 0; i < length; i++)
	{
		choice += probs[i];
		if (choice >= total_count)
		{
			return i;
		}
	}
	return -10285;
}


bool QtreeBranch::get_random(qtree_point *out)
{
	int *probs = (int *) malloc (sizeof(*probs) * get_two_2_dim());

	int count = 0;
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		switch(types[i])
		{
		case QTREE_TYPE_BRANCH:
			probs[i] = ((QtreeBranch *) branches[i])->size();
			count += probs[i];
			break;
		case QTREE_TYPE_LEAF:
			probs[i] = ((QtreeLeaf *) branches[i])->get_size();
			count += probs[i];
			break;
		case QTREE_TYPE_NULL:
			probs[i] = 0;
		}
	}

	int choice = select_random(probs, count, get_two_2_dim());
	if (choice < 0)
	{
		puts("Error 1841741");
		exit(-1);
//		return false;
	}

	free(probs);

	switch(types[choice])
	{
	case QTREE_TYPE_BRANCH:
		return ((QtreeBranch *) branches[choice])->get_random(out);
		break;
	case QTREE_TYPE_LEAF:
		return ((QtreeLeaf *) branches[choice])->get_random(out);
		break;
	case QTREE_TYPE_NULL:
		puts("Error 10672047248502");
		exit(1);
	}
}

static void accumulate_points(qtree_point *pnt, void *ref, void *arg)
{
	((QtreeLeaf *) arg)->assign(pnt, ref);
}

bool QtreeBranch::combine()
{
	if (count > get_branch_factor())
	{
		return false;
	}

	if (parent == NULL)
	{
		return false;
	}

	QtreeLeaf *leaf = new QtreeLeaf(parent,
			qtree_point_dup(get_dim(), lb), qtree_point_dup(get_dim(), ub));

	apply(&accumulate_points, (void *) leaf);

	int quad = get_parents_quad();
	parent->types[quad] = QTREE_TYPE_LEAF;
	parent->branches[quad] = leaf;

	return true;
}

void QtreeBranch::verify()
{
	if (count < get_branch_factor() && parent != NULL)
	{
		puts("Error 1750173509870135");
		exit(1);
	}

	if (count != recount())
	{
		puts("Error 1751079835");
		exit(1);
	}

	QtreeBranch *child;
	QtreeLeaf *leaf;

	for (int i = 0; i < get_two_2_dim(); i++)
	{
		switch (types[i])
		{
		case QTREE_TYPE_BRANCH:
			child = (QtreeBranch *) branches[i];

			for (int d = 0; d < get_dim(); d++)
			{
				double clb;
				double cub;
				double mid = (lb[d] + ub[d]) / 2;

				if (i & (1 << d))
				{
					clb = mid;
					cub = ub[d];
				}
				else
				{
					clb = lb[d];
					cub = mid;
				}

				if (child->lb[d] != clb || child->ub[d] != cub)
				{
					puts("Error 169817632918632");
					exit(1);
				}
			}
			child->verify();
			break;
		case QTREE_TYPE_LEAF:
			leaf = (QtreeLeaf *) branches[i];
			for (int d = 0; d < get_dim(); d++)
			{
				double clb;
				double cub;
				double mid = (lb[d] + ub[d]) / 2;

				if (i & (1 << d))
				{
					clb = mid;
					cub = ub[d];
				}
				else
				{
					clb = lb[d];
					cub = mid;
				}

				if (leaf->lb[d] != clb || leaf->ub[d] != cub)
				{
					puts("Error 69879866999");
					exit(1);
				}
			}
			leaf->verify();
			break;
		case QTREE_TYPE_NULL:
			if (branches[i] != NULL)
			{
				puts("1653976315867915386");
				exit(1);
			}
			break;
		default:
			puts("Error 1659160574602837465024");
			exit(1);
		}
	}

	// make sure all children are adjacent to each other.
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_point *lb1 = NULL;
		qtree_point *ub1 = NULL;
		switch (types[i])
		{
		case QTREE_TYPE_BRANCH:
			child = (QtreeBranch *) branches[i];
			lb1 = child->lb;
			ub1 = child->ub;
			break;
		case QTREE_TYPE_LEAF:
			leaf = (QtreeLeaf *) branches[i];
			lb1 = leaf->lb;
			ub1 = leaf->ub;
			break;
		case QTREE_TYPE_NULL:
			break;
		}

		if (lb1 == NULL)
		{
			continue;
		}

		for (int j = 0; j < get_two_2_dim(); j++)
		{
			if (i == j)
			{
				continue;
			}

			qtree_point *lb2 = NULL;
			qtree_point *ub2 = NULL;

			switch (types[j])
			{
			case QTREE_TYPE_BRANCH:
				child = (QtreeBranch *) branches[j];
				lb2 = child->lb;
				ub2 = child->ub;
				break;
			case QTREE_TYPE_LEAF:
				leaf = (QtreeLeaf *) branches[j];
				lb2 = leaf->lb;
				ub2 = leaf->ub;
				break;
			case QTREE_TYPE_NULL:
				break;
			}

			if (lb2 == NULL)
			{
				continue;
			}

			if (!qtree_regions_are_adjacent(lb1, ub1, lb2, ub2, get_dim()))
			{
				puts("Error 1084711356981765398176395");
				exit(1);
			}
		}
	}
}

qtree_type QtreeBranch::get_type(int i)
{
	return types[i];

}
QtreeBranch *QtreeBranch::get_branch(int i)
{
	if (types[i] != QTREE_TYPE_BRANCH)
	{
		puts("Error 1716436143187365");
		exit(1);
	}
	return (QtreeBranch *) branches[i];
}

qtree_point *QtreeBranch::get_lb()
{
	return lb;
}
qtree_point *QtreeBranch::get_ub()
{
	return ub;
}
}
