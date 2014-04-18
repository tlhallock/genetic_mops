/*
 * qtree_test_utils.cc
 *
 *  Created on: Apr 5, 2014
 *      Author: thallock
 */

#include "qtree.h"

#include <iostream>

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

#define NPOINTS 1000

static bool qtree_leaf_verify_bounds(qtree_leaf *leaf, double llx, double lly, double urx, double ury, int depth, bool verbose)
{
	for (int i = 0; i < leaf->size; i++)
	{
//		if (verbose)
//		{
//			for (int j = 0; j < depth; j++)
//			{
//				fputc('\t', stdout);
//			}
//			printf("%lf %lf\n", leaf->points[i].x, leaf->points[i].y);
//		}
//		if (leaf->points[i].x < llx)
//		{
//			if (verbose)
//			{
//				puts("fail");
//			}
//			else
//			{
//				return false;
//			}
//		}
//		if (leaf->points[i].x > urx)
//		{
//			if (verbose)
//			{
//				puts("fail");
//			}
//			else
//			{
//				return false;
//			}
//		}
//		if (leaf->points[i].y < lly)
//		{
//			if (verbose)
//			{
//				puts("fail");
//			}
//			else
//			{
//				return false;
//			}
//		}
//		if (leaf->points[i].y > ury)
//		{
//			if (verbose)
//			{
//				puts("fail");
//			}
//			else
//			{
//				return false;
//			}
//		}
	}

	return true;
}

static bool qtree_branch_verify_bounds(qtree_branch *branch, double llx, double lly, double urx, double ury, int depth, bool verbose)
{
	if (branch == NULL)
	{
		return true;
	}

	for (int i=0;i<4;i++)
	{
		if (verbose)
		{
			for (int j = 0; j < depth; j++)
			{
				fputc('\t', stdout);
			}
		}

		double width = urx - llx;
		double n_llx = llx;
		double n_lly = lly;
		double n_urx = urx;
		double n_ury = ury;

		switch(i)
		{
		case UPPER_RIGHT:
			n_llx += width / 2;
			n_lly += width / 2;
			if (verbose)
			{
				printf("upper right: ");
			}
			break;
		case UPPER_LEFT:
			n_lly += width / 2;
			n_urx -= width / 2;
			if (verbose)
			{
				printf("upper left : ");
			}
			break;
		case LOWER_LEFT:
			n_urx -= width / 2;
			n_ury -= width / 2;
			if (verbose)
			{
				printf("lower left : ");
			}
			break;
		case LOWER_RIGHT:
			n_llx += width / 2;
			n_ury -= width / 2;
			if (verbose)
			{
				printf("lower right: ");
			}
			break;
		}
		if (verbose)
		{
			printf("%lf %lf %lf %lf ", n_llx, n_lly, n_urx, n_ury);
		}

		qtree_type type = branch->types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			if (verbose)
			{
				puts("branch");
			}
			if (!qtree_branch_verify_bounds((qtree_branch *)branch->branches[i], n_llx, n_lly, n_urx, n_ury, depth + 1, verbose))
			{
				return false;
			}
			break;
		case QTREE_TYPE_LEAF:
			if (verbose)
			{
				puts("leaf");
			}
			if (!qtree_leaf_verify_bounds((qtree_leaf *) branch->branches[i], n_llx, n_lly, n_urx, n_ury, depth + 1, verbose))
			{
				return false;
			}
			break;
		case QTREE_TYPE_NULL:
			if (verbose)
			{
				puts("null");
			}
			break;
		default:
			puts("Error 0760147");
			exit(1);
		}
	}

	return true;
}


bool qtree_verify_bounds(qtree *tree, bool verbose)
{
//	double llx = 0;//tree->lower_left.x;
//	double lly = 0;//tree->lower_left.y;
//	double urx = llx + tree->width;
//	double ury = lly + tree->width;
//	return qtree_branch_verify_bounds(tree->root, llx, lly, urx, ury, 0, verbose);
}



void test_add_remove_contains()
{

	srand(500000);

	int dim = 2;

	qtree_point *lb = qtree_point_new0(dim);
	lb[0] = 0;
	lb[1] = 0;
	qtree_point *ub = qtree_point_new0(dim);
	lb[0] = 1;
	lb[1] = 1;
	qtree *tree = qtree_new(lb, ub, dim);

	qtree_point *random_points[NPOINTS];
	bool added[NPOINTS];

	for (int i = 0; i < NPOINTS; i++)
	{
		bool is_new_point;

		random_points[i] = qtree_point_new0(dim);

		do
		{
			qtree_point_del(random_points[i]);
			random_points[i] = qtree_point_new_rand(dim);

			is_new_point = true;
			for (int j=0;j<i;j++)
			{
				if (!qtree_point_equals(random_points[j], random_points[i], dim))
				{
					continue;
				}

				is_new_point = false;
				break;
			}
		} while (!is_new_point);
		added[i] = false;
	}

	for (;;)
	{
		int index = rand() % NPOINTS;
		qtree_point *point = random_points[index];

		if (rand() < (INT_MAX / 2))
		{
			std::cout << "Adding ";
			qtree_point_print(stdout, point, dim, true);

			bool contained = qtree_contains(tree, point);
			if (added[index] != contained)
			{
				puts("Fail 420802");
				exit(1);
			}

			if (contained == qtree_add(tree, point))
			{
				puts("Fail 407");
				exit(1);
			}
			added[index] = true;

			if (!qtree_contains(tree, point))
			{
				puts("Fail 1533513");
				exit(1);
			}
		}
		else
		{
			std::cout << "Removing ";
			qtree_point_print(stdout, point, dim, true);

			bool contained = qtree_contains(tree, point);
			if (added[index] != contained)
			{
				puts("Fail 420802");
				exit(1);
			}

			if (qtree_remove(tree, point) != contained)
			{
				puts("Fail 62462");
				exit(1);
			}
			added[index] = false;

			if (qtree_contains(tree, point))
			{
				puts("Fail 1533513");
				exit(1);
			}
		}

		qtree_print(stdout, tree);
		if (!qtree_verify_bounds(tree, false))
		{
			puts("fail");
			qtree_verify_bounds(tree, true);
			break;
		}
	}

	qtree_del(tree);
}










//	printf("quradrant of %lf %lf in %lf %lf %lf %lf is ", point->x, point->y,
//			lower_left->x, lower_left->y, lower_left->x + width,
//			lower_left->y + width);

//static bool qtree_is_point_contained_in(qtree_point *point, double llx, double lly, double urx, double ury)
//{
//	if (llx > point->x)
//	{
//		return false;
//	}
//	if (lly > point->y)
//	{
//		return false;
//	}
//
//	if (urx < point->x)
//	{
//		return false;
//	}
//
//	if (ury < point->y)
//	{
//		return false;
//	}
//
//	return true;
//}
