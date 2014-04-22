/*
 * qtree.c
 *
 *  
 *      Author: rever
 */

#include "Qtree.h"

#include "qtree_point.h"

#include <stdlib.h>
#include <stdbool.h>

#include <float.h>


namespace qtree
{

QtreeBranch* Qtree::grow_root(QtreeBranch* branch, qtree_point* direction)
{
	int old_quad = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = ub[i] - lb[i];
		if (direction[i] > ub[i])
		{
			// grow upper...
			ub[i] += diff;

			old_quad |= (1 << i);
		}
		else
		{
			// grow lower...
			lb[i] -= diff;
		}
	}

	printf("Growing to lb=");
	qtree_point_print(stdout, lb, get_dim(), false);
	printf(" ub=");
	qtree_point_print(stdout, ub, get_dim(), false);
	printf(" for ");
	qtree_point_print(stdout, direction, get_dim(), true);
	fflush(stdout);

	QtreeBranch * ret_val = new QtreeBranch(NULL, lb, ub, dim, two_2_dim);
	ret_val->types[old_quad] = QTREE_TYPE_BRANCH;
	ret_val->branches[old_quad] = branch;
	branch->parent = ret_val;

	return ret_val;
}

bool Qtree::in_current_bounds(qtree_point* point)
{
	for (int i = 0; i < dim; i++)
	{
		if (point[i] < lb[i])
		{
			return false;
		}
		if (point[i] > ub[i])
		{
			return false;
		}
	}
	return true;
}

QtreeLeaf* Qtree::find(qtree_point* point)
{
	return root->find(point);
}

Qtree::Qtree(qtree_point* _lb, qtree_point* _ub, int _dim) :
		dim(_dim),
		two_2_dim(1 << _dim),
		lb(qtree_point_dup(_dim, _lb)),
		ub(qtree_point_dup(_dim, _ub)),
		root(new QtreeBranch(NULL, lb, ub, dim, two_2_dim)) {}

Qtree::Qtree(int _dim) :
		dim(_dim),
		two_2_dim(1 << _dim),
		lb(qtree_point_new(_dim, 0.0)),
		ub(qtree_point_new(_dim, 1.0)),
		root(new QtreeBranch(NULL, lb, ub, dim, two_2_dim)) {}

Qtree::Qtree(double _lb, double _ub, int _dim) :
		dim(_dim),
		two_2_dim(1 << _dim),
		lb(qtree_point_new(_dim, _lb)),
		ub(qtree_point_new(_dim, _ub)),
		root(new QtreeBranch(NULL, lb, ub, dim, two_2_dim)) {}

Qtree::~Qtree()
{
	delete root;
	free(lb);
	free(ub);
}

bool Qtree::add(qtree_point* point)
{
	while (!in_current_bounds(point))
	{
		root = grow_root(root, point);
	}

	QtreeLeaf *leaf = root->find(point);

	if (leaf->index_of(point) >= 0)
	{
		return false;
	}

	while (leaf->get_size() == BRANCH_FACTOR)
	{
		QtreeBranch *parent = leaf->get_parent();
		int quad = leaf->get_parents_quad();

		qtree_point *new_lb = qtree_point_dup(dim, parent->lb);
		qtree_point *new_ub = qtree_point_dup(dim, parent->ub);
		qtree_bounds_select(new_lb, new_ub, quad, dim);

		QtreeBranch *new_branch = new QtreeBranch(parent, new_lb, new_ub, dim, two_2_dim);

		qtree_point_del(new_lb);
		qtree_point_del(new_ub);

		parent->set_branch(quad, QTREE_TYPE_BRANCH, new_branch);

		for (int i = 0; i < BRANCH_FACTOR; i++)
		{
			quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, leaf->points[i], dim);
			QtreeLeaf *new_leaf = new_branch->get_leaf(quad);
			new_leaf->assign(leaf->points[i]);
		}

		delete leaf;

		quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, point, dim);
		leaf = (QtreeLeaf *) new_branch->get_leaf(quad);
	}

	leaf->assign(point);
	return true;
}

void Qtree::clear()
{
	delete root;
	root = new QtreeBranch(NULL, lb, ub, dim, two_2_dim);
}

bool Qtree::remove(qtree_point* point)
{
	if (!in_current_bounds(point))
	{
		return false;
	}

	QtreeLeaf *leaf = root->find(point);

	int index = leaf->index_of(point);
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
	QtreeBranch *parent = leaf->get_parent();
	int quad = leaf->get_parents_quad();

	parent->set_branch(quad, QTREE_TYPE_NULL, NULL);

	delete leaf;

	QtreeBranch *node = parent;
	while ((parent = node->get_parent()) != NULL
			&& node->is_empty())
	{
		int quad = node->get_parents_quad();
		parent->set_branch(quad, QTREE_TYPE_NULL, NULL);
		delete node;
		node = parent;
	}

	return true;
}

int Qtree::count()
{
	return root->count();
}

bool Qtree::contains(qtree_point* point)
{
	if (!in_current_bounds(point))
	{
		return false;
	}
	QtreeLeaf *leaf = root->find(point);
	return leaf->index_of(point) >= 0;
}

void Qtree::get_min(double* y_out, int dim)
{
	// need to assign y_out to default values
	puts("Error 41fgj1=84");
	exit(1);
	root->get_min(y_out, dim);
}

void Qtree::apply(void (*fctn)(qtree_point* pnt, void* arg), void* arg)
{
	root->apply(fctn, arg);
}

size_t Qtree::get_memory_usage()
{
	return 0;
}

void Qtree::print(FILE* out)
{
	root->print(out, 0);
}

int qtree_get_quadrant(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim)
{
//	fprintf(stdout, "the quadrand of ");
//	qtree_point_print(stdout, point, dim, true);
//	fprintf(stdout, " in lb=");
//	qtree_point_print(stdout, lb, dim, false);
//	fprintf(stdout, " in ub=");
//	qtree_point_print(stdout, ub, dim, true);
//	fprintf(stdout, " is ");

	int quad = 0;
	int i;
	for (i=0;i<dim;i++)
	{
		double mid = (ub[i] + lb[i]) / 2;
		if (point[i] > mid)
		{
			quad |= (1 << i);
		}

		if (ERROR_CHECKING)
		{
			if (point[i] < lb[i])
			{
				puts("Error 104801");
				const char *n = NULL;
				printf("%s", n);
				exit (1);
			}
			if (point[i] > ub[i])
			{
				puts("Error 10472994");
				exit (1);
			}
		}
	}

//	fprintf(stdout, "%d\n", quad);

	return quad;
}

void qtree_bounds_select(qtree_point *lb, qtree_point *ub, int quad, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		double mid = (lb[i] + ub[i]) / 2;

		if (quad & (1 << i))
		{
			lb[i] = mid;
		}
		else
		{
			ub[i] = mid;
		}
	}
}


double Qtree::get_nearest_point(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *))
{
	double cmin = DBL_MAX;
	// find a reasonable distance to start with...
	QtreeLeaf *leaf = find(point);
	if (leaf->is_empty())
	{
		QtreeBranch *branch = leaf->get_parent();
		while (branch->is_empty())
		{
			branch = branch->get_parent();
			if (branch == NULL)
			{
				return -DBL_MAX;
			}
		}
		branch->find_nearest(point, out, norm, &cmin);
	}
	else
	{
		leaf->find_nearest(point, out, norm, &cmin);
	}

	root->find_nearest(point, out, norm, &cmin);

	return cmin;
}

}
