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

#include "math_functions.h"

#include "VectorImageSet.h"
#include "RtreeImageSet.h"

#include "MOP.h"

#include "qtree.h"

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

	qtree::qtree_point *lb = qtree::qtree_point_new0(DIM);
	for (int i = 0; i < DIM; i++)
	{
		lb[i] = -10;
	}
	qtree::qtree_point *ub = qtree::qtree_point_new0(DIM);
	for (int i = 0; i < DIM; i++)
	{
		ub[i] = 10;
	}
	RtreeImageSet qimage(DIM, lb, ub);

	BoundedMopStats vboard(DIM, DIM, lb, ub, &vimage, &objective_func, &is_feasible_function_2);
	BoundedMopStats qboard(DIM, DIM, lb, ub, &qimage, &objective_func, &is_feasible_function_2);

	qtree::qtree_point_del(lb);
	qtree::qtree_point_del(ub);

	double *x_val = qtree::qtree_point_new0(DIM);
	double *y_val = qtree::qtree_point_new0(DIM);

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
