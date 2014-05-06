/*
 * initial_set_generator.cc
 *
 *  Created on: May 4, 2014
 *      Author: thallock
 */

#include "common.h"

// This is copied from wikipedia: en.wikipedia.org/wiki/Box_Muller_transform#Implementation
#define TWO_PI 6.2831853071795864769252866

double generateGaussianNoise(const double &variance)
{
	static bool hasSpare = false;
	static double rand1, rand2;

	if (hasSpare)
	{
		hasSpare = false;
		return sqrt(variance * rand1) * sin(rand2);
	}

	hasSpare = true;

	rand1 = rand() / ((double) RAND_MAX);
	if (rand1 < 1e-100)
	{
		rand1 = 1e-100;
	}

	rand1 = -2 * log(rand1);
	rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

	return sqrt(variance * rand1) * cos(rand2);
}

static double *scale_pnt(int dim, double *pnt)
{
	for (int i = 0; i < dim; i++)
	{
		pnt[i] *= 100;
	}
	return pnt;
}

InitialSet *get_equidistant_initial_set(double spacing, int dim)
{
	std::vector<double *> pnts;

	if (dim == 1)
	{
		for (double d1 = 0.0; d1 < 1.0; d1 += spacing)
		{
			double *pnt = (double *) malloc(sizeof(*pnt) * dim);
			pnt[0] = d1;
			pnts.push_back(scale_pnt(dim, pnt));
		}
	} else if (dim == 2)
	{
		for (double d1 = 0.0; d1 < 1.0; d1 += spacing)
		{
			for (double d2 = 0.0; d2 < 1.0; d2 += spacing)
			{
				double *pnt = (double *) malloc(sizeof(*pnt) * dim);
				pnt[0] = d1;
				pnt[1] = d2;
				pnts.push_back(scale_pnt(dim, pnt));
			}
		}
	} else
	{
		puts("Implement me!!! 258609287602");
		exit(1);
	}

	return new InitialSet(&pnts, dim, &l_2);
}


InitialSet *get_simple_pareto_initial_set(int num_points, int dim, bool uniform)
{
	std::vector<double *> pnts;

	for (int i = 0; i < num_points; i++)
	{
		double t;
		if (uniform)
		{
			t = (i / (double) num_points) * TWO_PI / 4;
		} else
		{
			t = (TWO_PI / 4) * (rand() / (double) RAND_MAX);
		}

		double xval = 1 - cos(t);
		double yval = 1 - sin(t);
//		double xval = i / (num_points - 1.0);
//		double yval =(xval - 1.0) * (xval - 1.0);

		double *pnt = (double *) malloc (sizeof(*pnt) * dim);
		pnt[0] = xval;
		pnt[1] = yval;
		pnts.push_back(scale_pnt(dim, pnt));
	}

	return new InitialSet(&pnts, dim, &l_2);
}


InitialSet *get_uniform_random_initial_set(int num_points, int dim)
{
	std::vector<double *> pnts;

	for (int i = 0; i < num_points; i++)
	{
		pnts.push_back(scale_pnt(dim, qtree::qtree_point_new_rand(dim)));
	}

	return new InitialSet(&pnts, dim, &l_2);
}


InitialSet *get_wavy_initial_set(int num_points, int dim)
{
	std::vector<double *> pnts;

	for (int i = 0; i < num_points; i++)
	{
		double *pnt = qtree::qtree_point_new(dim);

		double x = rand() / (double) RAND_MAX;
		pnt[0] = x;

		for (int j = 1; j < dim; j++)
		{
			pnt[j] = sin(TWO_PI * x) + rand() / (double) RAND_MAX;
		}

		pnts.push_back(scale_pnt(dim, pnt));
	}

	return new InitialSet(&pnts, dim, &l_2);
}

InitialSet *get_bias_initial_set(int num_points, int dim)
{
	std::vector<double *> pnts;

	for (int i = 0; i < num_points; i++)
	{
		double *pnt = qtree::qtree_point_new(dim);

		for (int j = 0; j < dim; j++)
		{
			double r;

			do
			{
				r = generateGaussianNoise(0.2);
			} while (r < 0.0 || r > 1.0);

			pnt[j] = r;
		}

		pnts.push_back(scale_pnt(dim, pnt));
	}

	return new InitialSet(&pnts, dim, &l_2);
}

