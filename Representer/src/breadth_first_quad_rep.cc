/*
 * breadth_first_quad_rep.cc
 *
 *  Created on: May 1, 2014
 *      Author: rever
 */

#include <stdlib.h>
#include "../../Rtree/src/Qtree.h"
#include "GeneticRepresenter.h"

double l_2(double *p1, double *p2, int dim);


typedef struct _list_node
{
	qtree::QtreeLeaf *leaf;
	qtree::QtreeBranch *branch;
	struct _list_node *next;
} list_node;

static void print_list(list_node *list, int dim, int depth)
{
	if (list == NULL)
	{
		return;
	}

	if (list->branch == NULL)
	{
		printf("[%d lower bound = ", depth);
		qtree::qtree_point_print(stdout, list->leaf->get_lb(), dim, false);
		printf(" %d upper bound = ", depth);
		qtree::qtree_point_print(stdout, list->leaf->get_ub(), dim, false);
		fputc(']', stdout);
	} else
	{
		printf("[%d lower bound = ", depth);
		qtree::qtree_point_print(stdout, list->branch->get_lb(), dim, false);
		printf(" %d upper bound = ", depth);
		qtree::qtree_point_print(stdout, list->branch->get_ub(), dim, false);
		fputc(']', stdout);
	}

	print_list(list->next, dim, depth + 1);

	if (depth == 0)
	{
		fputc('\n', stdout);
	}
}

void bread_first_represent(InitialSet *set, int num_points, char *mask_out)
{
	qtree::Qtree tree(set->get_dim(), 1);

	for (int i = 0; i < set->size(); i++)
	{
		tree.add(set->get(i), NULL);
	}

	list_node *nodes = (list_node *) malloc (sizeof(*nodes) * num_points);
	for (int i = 1; i < num_points; i++)
	{
		nodes[i - 1].next = &(nodes[i]);
	}
	nodes[num_points-1].next = NULL;
	list_node *empty;
	list_node *last;
	list_node *first;

	empty = nodes;

	first = empty;
	last = empty;
	empty = empty->next;
	last->next = NULL;
	last->leaf = NULL;
	last->branch = tree.get_root();

	int list_size = 1;

	int two_2_dim = (1 << 2);

	while(list_size < num_points)
	{
		print_list(first, set->get_dim(), 0);
		if (first->leaf == NULL)
		{
			for (int i = 0; i < two_2_dim && list_size < num_points; i++)
			{

				list_node *next;
				switch (first->branch->get_type(i))
				{
				case qtree::QTREE_TYPE_LEAF:
					next = empty;
					empty = empty->next;
					next->next = NULL;
					last->next = next;
					last = next;

					next->leaf = first->branch->get_leaf(i);
					next->branch = NULL;

					list_size++;
					break;
				case qtree::QTREE_TYPE_BRANCH:
					next = empty;
					empty = empty->next;
					next->next = NULL;
					last->next = next;
					last = next;

					next->leaf = NULL;
					next->branch = first->branch->get_branch(i);

					list_size++;
					break;
				case qtree::QTREE_TYPE_NULL:
					break;
				}
			}

			list_node *n = first;
			first = n->next;
			n->next = empty;
			empty = n;
		}
		else
		{
			last->next = first;
			last = first;
			first = first->next;
			last->next = NULL;
		}
	}

	double *point = qtree::qtree_point_new(set->get_dim());
	double *close = qtree::qtree_point_new(set->get_dim());
	double *lb;
	double *ub;
	while (first != NULL)
	{
		if (first->branch == NULL)
		{
			lb = first->leaf->get_lb();
			ub = first->leaf->get_ub();
		} else
		{
			lb = first->branch->get_lb();
			ub = first->branch->get_ub();
		}

		for (int i = 0; i < set->get_dim(); i++)
		{
			close[i] = (lb[i] + ub[i]) / 2;
		}

		tree.get_nearest_point(close, point, &l_2);

		int index = set->index_of(point);

		if (index < 0)
		{
			puts("alvhiuhncawliuehincawitgagdsyudsf");
			exit(-1);
		}

		mask_out[index] = 1;

		first = first->next;
	}
	qtree::qtree_point_del(point);
	qtree::qtree_point_del(close);

	free(nodes);
}
