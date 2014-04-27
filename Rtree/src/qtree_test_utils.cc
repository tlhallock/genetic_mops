/*
 * qtree_test_utils.cc
 *
 *  
 *      Author: thallock
 */

#include "Qtree.h"

#include <iostream>

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#define NPOINTS 1000

namespace qtree
{

static double dist(qtree_point *p1, qtree_point *p2, int dim)
{
	double sum = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = p1[i] - p2[i];
		sum += diff * diff;
	}

	return sqrt(sum);
}

//
//static bool qtree_leaf_verify_bounds(qtree_leaf *leaf, double llx, double lly, double urx, double ury, int depth, bool verbose)
//{
//	for (int i = 0; i < leaf->size; i++)
//	{
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
//	}
//
//	return true;
//}
//
//static bool qtree_branch_verify_bounds(qtree_branch *branch, double llx, double lly, double urx, double ury, int depth, bool verbose)
//{
//	if (branch == NULL)
//	{
//		return true;
//	}
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (verbose)
//		{
//			for (int j = 0; j < depth; j++)
//			{
//				fputc('\t', stdout);
//			}
//		}
//
//		double width = urx - llx;
//		double n_llx = llx;
//		double n_lly = lly;
//		double n_urx = urx;
//		double n_ury = ury;
//
//		switch (i)
//		{
//		case UPPER_RIGHT:
//			n_llx += width / 2;
//			n_lly += width / 2;
//			if (verbose)
//			{
//				printf("upper right: ");
//			}
//			break;
//		case UPPER_LEFT:
//			n_lly += width / 2;
//			n_urx -= width / 2;
//			if (verbose)
//			{
//				printf("upper left : ");
//			}
//			break;
//		case LOWER_LEFT:
//			n_urx -= width / 2;
//			n_ury -= width / 2;
//			if (verbose)
//			{
//				printf("lower left : ");
//			}
//			break;
//		case LOWER_RIGHT:
//			n_llx += width / 2;
//			n_ury -= width / 2;
//			if (verbose)
//			{
//				printf("lower right: ");
//			}
//			break;
//		}
//		if (verbose)
//		{
//			printf("%lf %lf %lf %lf ", n_llx, n_lly, n_urx, n_ury);
//		}
//
//		qtree_type type = branch->types[i];
//		switch (type)
//		{
//		case QTREE_TYPE_BRANCH:
//			if (verbose)
//			{
//				puts("branch");
//			}
//			if (!qtree_branch_verify_bounds((qtree_branch *) branch->branches[i], n_llx, n_lly, n_urx, n_ury, depth + 1, verbose))
//			{
//				return false;
//			}
//			break;
//		case QTREE_TYPE_LEAF:
//			if (verbose)
//			{
//				puts("leaf");
//			}
//			if (!qtree_leaf_verify_bounds((qtree_leaf *) branch->branches[i], n_llx, n_lly, n_urx, n_ury, depth + 1, verbose))
//			{
//				return false;
//			}
//			break;
//		case QTREE_TYPE_NULL:
//			if (verbose)
//			{
//				puts("null");
//			}
//			break;
//		default:
//			puts("Error 0760147");
//			exit(1);
//		}
//	}
//
//	return true;
//}


//bool qtree_verify_bounds(qtree *tree, bool verbose)
//{
////	double llx = 0;//tree->lower_left.x;
////	double lly = 0;//tree->lower_left.y;
////	double urx = llx + tree->width;
////	double ury = lly + tree->width;
////	return qtree_branch_verify_bounds(tree->root, llx, lly, urx, ury, 0, verbose);
//	return true;
//}




void test_add_remove_contains()
{
	srand(500000);

	int dim = 3;

	qtree_point *lb = qtree_point_new(dim);
	for (int i = 0; i < dim; i++)
	{
		lb[i] = 0;
	}
	qtree_point *ub = qtree_point_new(dim);
	for (int i = 0; i < dim; i++)
	{
		ub[i] = 1;
	}
	Qtree *tree = new Qtree(dim, 1);

	qtree_point_del(lb);
	qtree_point_del(ub);

	qtree_point *random_points[NPOINTS];
	bool added[NPOINTS];

	for (int i = 0; i < NPOINTS; i++)
	{
		bool is_new_point;

		random_points[i] = qtree_point_new(dim);

		do
		{
			qtree_point_del(random_points[i]);
			random_points[i] = qtree_point_new_rand(dim);
			for (int j = 0; j < dim; j++)
			{
				random_points[i][j] *= 100;
			}

			is_new_point = true;
			for (int j = 0; j < i; j++)
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

	for (int i = 0; i < 10000; i++)
	{
		int index;

		qtree_point *point;
		if (rand() % 3 || tree->count() == 0)
		{
			index = rand() % NPOINTS;
			point = random_points[index];
		} else
		{
			point = qtree_point_new(dim);
			index = -17593267;
			if (!tree->get_random(point))
			{
				puts("Error 1076184761986799643222876");
				exit(1);
			}
			for (int j = 0; j < NPOINTS; j++)
			{
				if (qtree_point_equals(point, random_points[j], dim))
				{
					index = j;
					break;
				}
			}
			if (index < 0)
			{
				puts("Error 157320720985");
				exit(1);
			}
			qtree_point_del(point);
			point = random_points[index];
		}

		if (rand() < (INT_MAX / 2))
		{
			std::cout << "Adding ";
			qtree_point_print(stdout, point, dim, true);

			bool contained = tree->contains(point);
			if (added[index] != contained)
			{
				fflush(stdout);
				puts("Fail 725728262682");
				exit(1);
			}

			if (contained == tree->add(point, NULL))
			{
				puts("Fail 407");
				exit(1);
			}
			added[index] = true;

			if (!tree->contains(point))
			{
				puts("Fail 1533513");
				exit(1);
			}
		} else
		{
			std::cout << "Removing ";
			qtree_point_print(stdout, point, dim, true);

			bool contained = tree->contains(point);
			if (added[index] != contained)
			{
				puts("Fail 420802");
				exit(1);
			}

			if (tree->remove(point) != contained)
			{
				puts("Fail 62462");
				exit(1);
			}
			added[index] = false;

			if (tree->contains(point))
			{
				puts("Fail 1533513");
				exit(1);
			}
		}

		if (!(rand() % 50))
		{
			tree->clear();

			for (int i=0;i<NPOINTS;i++)
			{
				added[i] = false;
			}

			puts("cleared...");
		}

		tree->print(stdout);
		tree->verify();

		qtree_point *random_point = qtree_point_new_rand(dim);
		for (int j = 0; j < dim; j++)
		{
			random_point[j] *= 100;
		}

		printf("Getting nearest point to ");
		qtree_point_print(stdout, random_point, dim, true);

		double d = DBL_MAX;
		int closest_index = -1;
		for (int j = 0; j < NPOINTS; j++)
		{
			if (!added[j])
			{
				continue;
			}
			double di = dist(random_point, random_points[j], dim);
			if (di < d)
			{
				closest_index = j;
				d = di;
			}
		}

		qtree_point *tmp = qtree_point_new(dim);
		double nearest = tree->get_nearest_point(random_point, tmp, &dist);

		if (!IS_ZERO(d - nearest))
		{
			printf("actual closest = ");
			qtree_point_print(stdout, random_points[closest_index], dim, false);
			printf(" with distance %lf\n", d);
			printf("found closest = ");
			qtree_point_print(stdout, tmp, dim, false);
			printf(" with distance %lf\n", nearest);
			puts("Error 11111235781235812635");

			tree->get_nearest_point(random_point, tmp, &dist);
			exit(1);
		}
		qtree_point_del(tmp);
	}

	for (int i = 0; i < NPOINTS; i++)
	{
		qtree_point_del(random_points[i]);
	}

	delete tree;
}



}
