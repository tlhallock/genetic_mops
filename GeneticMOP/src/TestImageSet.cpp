/*
 * TestImageSet.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: thallock
 */



//============================================================================
// Name        : GeneticMOP.cpp
// Author      : rever
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include<limits.h>
#include <stdio.h>
#include <float.h>

#include "math_functions.h"

#include "VectorImageSet.h"
#include "RtreeImageSet.h"

#include "MOP.h"

#include "Qtree.h"

#define DIM 5
#define NUM_GUESSES 100000


void objective_func(double *x, double *y_out)
{
	for (int i = 0; i < DIM; i++)
	{
		y_out[i] = x[i];
	}
}

void test_pareto_equivalence()
{
	long seed = time(NULL);
	srand(seed);

	VectorImageSet vimage(DIM);

	qtree::qtree_point *lb = qtree::qtree_point_new(DIM);
	for (int i = 0; i < DIM; i++)
	{
		lb[i] = -10;
	}
	qtree::qtree_point *ub = qtree::qtree_point_new(DIM);
	for (int i = 0; i < DIM; i++)
	{
		ub[i] = 10;
	}
	RtreeImageSet qimage(DIM);

	BoundedMopStats vboard(DIM, DIM, lb, ub, &vimage, &objective_func, &is_feasible_function_2);
	BoundedMopStats qboard(DIM, DIM, lb, ub, &qimage, &objective_func, &is_feasible_function_2);

	qtree::qtree_point_del(lb);
	qtree::qtree_point_del(ub);

	double *x_val = qtree::qtree_point_new(DIM);
	double *y_val = qtree::qtree_point_new(DIM);

	for (int i = 0; i < NUM_GUESSES; i++)
	{
		vboard.sample_feasible(x_val);
		vboard.make_guess(x_val, y_val);
		qboard.make_guess(x_val, y_val);
	}

	long start;
	long vtime;
	long qtime;

	start = time(NULL);
	int vsize = vimage.get_pareto_solutions()->size();
	vtime = time(NULL) - start;

	start = time(NULL);
	int qsize = qimage.get_pareto_solutions()->size();
	qtime = time(NULL) - start;

	std::cout << "Number of pareto optimal points in vector: " << vsize << " in " << vtime << std::endl;
	std::cout << "Number of pareto optimal points in qtree:  " << qsize << " in " << qtime << std::endl;

	qtree::qtree_point_del(x_val);
	qtree::qtree_point_del(y_val);
}

void test_nearest()
{
	int dim = 3;

	int num_points = 100;

	qtree::Qtree qtree(dim);
	std::vector<double *> vec;

	double *nearest = qtree::qtree_point_new(dim);

	for (int i = 0; i < num_points; i++)
	{
		qtree::qtree_point *point = qtree::qtree_point_new_rand(dim);

		qtree.add(point, NULL);
		vec.push_back(point);

		qtree.print(stdout);
		fflush(stdout);
	}

	for (int i = 0; i < 100; i++)
	{
		qtree::qtree_point *point = qtree::qtree_point_new_rand(dim);

		double nearest_q = qtree.get_nearest_point(point, nearest, &l_2);

		double *other_nearest = *vec.begin();
		double other_nearest_dist = l_2(other_nearest, point, dim);
		for (std::vector<double *>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			double d = l_2(*it, point, dim);
			if (d < other_nearest_dist)
			{
				other_nearest_dist = d;
				qtree::qtree_point_assign(other_nearest, *it, dim);
			}
		}

		printf("quad tree: dist = %lf, point = \n", nearest_q);
		qtree::qtree_point_print(stdout, nearest, dim, true);
		printf("vector: dist = %lf, point = \n", other_nearest_dist);
		qtree::qtree_point_print(stdout, other_nearest, dim, true);

		qtree::qtree_point_del(point);
	}


	for (int i = 0; i < num_points; i++)
	{
		free(vec.at(i));
	}
}
