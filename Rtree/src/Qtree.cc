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
#include <stdio.h>

#include <float.h>


namespace qtree
{

bool qtree_region_contains(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim)
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

bool qtree_regions_are_adjacent(qtree_point *lb1, qtree_point *ub1, qtree_point *lb2, qtree_point *ub2, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		if (lb1[i] == lb2[i])
		{
			return true;
		}
		if (lb1[i] == ub2[i])
		{
			return true;
		}
		if (ub1[i] == lb2[i])
		{
			return true;
		}
		if (ub1[i] == ub2[i])
		{
			return true;
		}
	}
	return false;
}


void qtree_print_quadrant(FILE *out, int quadrant, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		if (quadrant & (1 << i))
		{
			fputc('1', out);
		}
		else
		{
			fputc('0', out);
		}
	}
}

Qtree::Qtree(int _dim) :
		dim(_dim),
		two_2_dim(1 << _dim),
		branch_factor(3),
		lb(qtree_point_new(_dim, 0.0)),
		ub(qtree_point_new(_dim, 1.0)),
		root(new QtreeBranch(this, NULL, lb, ub)) {}

Qtree::Qtree(qtree_point* _lb, qtree_point* _ub, int _dim, int _branch_factor) :
		dim(_dim),
		two_2_dim(1 << _dim),
		branch_factor(_branch_factor),
		lb(qtree_point_dup(_dim, _lb)),
		ub(qtree_point_dup(_dim, _ub)),
		root(new QtreeBranch(this, NULL, lb, ub)) {}

Qtree::Qtree(double _lb, double _ub, int _dim, int _branch_factor) :
		dim(_dim),
		two_2_dim(1 << _dim),
		branch_factor(_branch_factor),
		lb(qtree_point_new(_dim, _lb)),
		ub(qtree_point_new(_dim, _ub)),
		root(new QtreeBranch(this, NULL, lb, ub)) {}

Qtree::Qtree(int _dim, int _branch_factor) :
		dim(_dim),
		two_2_dim(1 << _dim),
		branch_factor(_branch_factor),
		lb(qtree_point_new(_dim, 0.0)),
		ub(qtree_point_new(_dim, 1.0)),
		root(new QtreeBranch(this, NULL, lb, ub)) {}

Qtree::~Qtree()
{
	delete root;
	free(lb);
	free(ub);
}

bool Qtree::in_current_bounds(qtree_point* point)
{
	return qtree_region_contains(lb, ub, point, dim);
}

QtreeLeaf* Qtree::find(qtree_point* point)
{
	return root->find(point);
}

void Qtree::grow_root(qtree_point* direction)
{
	int old_quad = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = ub[i] - lb[i];
		if (direction[i] > ub[i])
		{
			// grow upper...
			ub[i] += diff;
		}
		else
		{
			// grow lower...
			lb[i] -= diff;

			old_quad |= (1 << i);
		}
	}

	printf("Growing to lb=");
	qtree_point_print(stdout, lb, get_dim(), false);
	printf(" ub=");
	qtree_point_print(stdout, ub, get_dim(), false);
	printf(" for ");
	qtree_point_print(stdout, direction, get_dim(), true);
	fflush(stdout);

	printf("Old root is %d (", old_quad);
	qtree_print_quadrant(stdout, old_quad, get_dim());
	printf(")\n");

	QtreeBranch *old_root = root;

	QtreeBranch * new_root = new QtreeBranch(this, NULL, lb, ub);
	new_root->types[old_quad] = QTREE_TYPE_BRANCH;
	new_root->branches[old_quad] = old_root;
	new_root->count = old_root->count;
	old_root->parent = new_root;

	root = new_root;

	if (old_root->combine())
	{
		delete old_root;
	}
}

// should refactor this to be part of the leaf/branch class
bool Qtree::add(qtree_point* point, void *ref)
{
	while (!in_current_bounds(point))
	{
		grow_root(point);
	}

	QtreeLeaf *leaf = root->find(point);

	if (leaf->index_of(point) >= 0)
	{
		return false;
	}

	while (leaf->get_size() == get_branch_factor())
	{
		QtreeBranch *parent = leaf->get_parent();
		int quad = leaf->get_parents_quad();

		qtree_point *new_lb = qtree_point_dup(dim, parent->lb);
		qtree_point *new_ub = qtree_point_dup(dim, parent->ub);
		qtree_bounds_select(new_lb, new_ub, quad, dim);

		QtreeBranch *new_branch = new QtreeBranch(this, parent, new_lb, new_ub);
		new_branch->count = get_branch_factor();

		qtree_point_del(new_lb);
		qtree_point_del(new_ub);

		parent->set_branch(quad, QTREE_TYPE_BRANCH, new_branch);

		for (int i = 0; i < get_branch_factor(); i++)
		{
			quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, leaf->points[i], dim);
			QtreeLeaf *new_leaf = new_branch->get_leaf(quad);
			new_leaf->assign(leaf->points[i], leaf->refs[i]);
		}

		delete leaf;

		quad = qtree_get_quadrant(new_branch->lb, new_branch->ub, point, dim);
		leaf = (QtreeLeaf *) new_branch->get_leaf(quad);
	}

	leaf->parent->add_to_size(1);
	leaf->assign(point, ref);
	return true;
}

void Qtree::clear()
{
	delete root;
	root = new QtreeBranch(this, NULL, lb, ub);
}

// should refactor this to be part of the leaf/branch class
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

	leaf->parent->add_to_size(-1);

	// clean up
	QtreeBranch *parent = leaf->get_parent();

	if (leaf->is_empty())
	{
		int quad = leaf->get_parents_quad();
		parent->set_branch(quad, QTREE_TYPE_NULL, NULL);
		delete leaf;
	}

	QtreeBranch *node = parent;
	parent = node->get_parent();

	while (node != NULL)
	{
		parent = node->get_parent();
		if (!node->combine())
		{
			return true;
		}
		delete node;
		node = parent;
	}

	return true;
}

int Qtree::count()
{
	return root->size();
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

void Qtree::apply(void (*fctn)(qtree_point* pnt, void *ref, void* arg), void* arg)
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
	for (int i = 0; i < dim; i++)
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


double Qtree::get_nearest_point(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int))
{
	if (root->size() < 1)
	{
		return DBL_MAX;
	}

	while (!in_current_bounds(point))
	{
		grow_root(point);
	}

	// find containing leaf.
	QtreeLeaf *leaf = find(point);

	qtree_point *clb;
	qtree_point *cub;
	if (leaf->is_empty())
	{
		clb = qtree_point_dup(get_dim(), leaf->get_parent()->lb);
		cub = qtree_point_dup(get_dim(), leaf->get_parent()->ub);
	} else
	{
		clb = qtree_point_dup(get_dim(), leaf->lb);
		cub = qtree_point_dup(get_dim(), leaf->ub);
	}

	int leaf_depth = 1 + leaf->parent->get_depth();

	double cmin = DBL_MAX;
	while (!root->find_nearest(point, out, norm, &cmin, clb, cub, leaf_depth))
	{
		bool grew = false;
		for (int i = 0; i < get_dim(); i++)
		{
			double diff = cub[i] - clb[i];

			double tmp = clb[i] - diff;
			if (tmp >= lb[i])
			{
				grew = true;
				clb[i] = tmp;
			}
			tmp = cub[i] + diff;
			if (tmp <= ub[i])
			{
				grew = true;
				cub[i] = tmp;
			}
		}

		if (!grew)
		{
			exit(1);
			return false;
		}
	}

	qtree_point_del(clb);
	qtree_point_del(cub);

	return cmin;
}


bool Qtree::get_random(qtree_point *out)
{
	return root->get_random(out);
}

}
