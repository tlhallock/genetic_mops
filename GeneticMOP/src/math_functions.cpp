/*
 * math_functions.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: thallock
 */

#include "math_functions.h"

#include <math.h>

void objective_function_1(double *x, double *y_out)
{
	y_out[0] = x[0];
	y_out[1] = x[1];
}

void objective_function_2(double *x, double *y_out)
{
	y_out[0] = x[0] * x[0] + x[0] * x[1];
	y_out[1] = x[1] * x[1] + x[0] * x[1];
}

void objective_function_3(double *x, double *y_out)
{
	y_out[0] = x[0];
	y_out[1] = x[1];

	y_out[0] -= .5;
	y_out[1] -= .5;

	double dist = 2 * sqrt(y_out[0] * y_out[0] + y_out[1] * y_out[1]);

	dist *= dist * dist * dist * dist;

	y_out[0] = +100 * dist;
	y_out[1] = -100 * dist;
}

void objective_function_4(double *x, double *y_out)
{
	double x_diff = x[0];
	double y_diff = x[1];

	x_diff *= x_diff;
	y_diff *= y_diff;

	double dist = .02 * exp(sqrt(x_diff + y_diff));

	y_out[0] =  dist;
	y_out[1] = -dist;
}

bool is_feasible_function_1(double *x)
{
	double shift = x[0] - 1;
	return shift * shift < x[1];
}

bool is_feasible_function_2(double *x)
{
	return true;
}

double l_inf(double *p1, double *p2, int dim)
{
	double max_dist = -1;
	for (int i=0; i<dim; i++)
	{
		double diff = p1[i] - p2[i];
		diff = diff > 0 ? diff : -diff;
		if (diff > max_dist)
		{
			max_dist = diff;
		}
	}
	return max_dist;
}
