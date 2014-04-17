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
#include <math.h>

#include "VectorImageSet.h"
#include "GeneticSolver.h"
#include "RandomSampleSolver.h"
#include "IncrementalSampleSolver.h"
#include "DepthFirstRecursiveStencil.h"

#define NUM_GUESSES 500

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

void summarize(ImageSet *image, const char *filename)
{
	double ideal[image->get_dim()];
	double nadir[image->get_dim()];
	double diff [image->get_dim()];

	image->get_ideal_point(ideal);
	std::cout << "Ideal point: " << std::endl;
	print_point(stdout, ideal, image->get_dim(), true);

	image->get_nadir_point(nadir);
	std::cout << "Nadir point: " << std::endl;
	print_point(stdout, nadir, image->get_dim(), true);

	for (int i=0;i<image->get_dim();i++)
	{
		diff[i] = nadir[i] - ideal[i];
	}

	image->get_nadir_point(nadir);
	std::cout << "Difference: " << std::endl;
	print_point(stdout, diff, image->get_dim(), true);

//	std::cout << "epsilon = " << image->get_epsilon(&l_inf) << std::endl;
//	std::cout << "delta   = " << image->get_delta(&l_inf)   << std::endl;

	std::vector<double*> *paretos = image->get_pareto_solutions();
	std::cout << "Number of ideal points: " << paretos->size() << std::endl;

	FILE *pareto_file = fopen(filename, "w");
	for (std::vector<double *>::iterator it = paretos->begin(); it != paretos->end(); ++it)
	{
		print_point(pareto_file, (*it), image->get_dim(), true);
	}
	fclose(pareto_file);

	printf("\n\n\n");
}

double get_index(double r)
{
	return r * r * r * r * r * r * r;
}

int main()
{
	long seed = time(NULL);
	srand(seed);

	FILE *debug = fopen("ga_debug.txt", "w");
	fprintf(debug, "%ld\n", seed);
	fclose(debug);


	debug = fopen("debug_dfrs.txt", "w");
	fprintf(debug, "%ld\n", seed);
	fclose(debug);

	VectorImageSet image(2);

	// unit square
	double xmin[2];
	xmin[0] = 0;
	xmin[1] = 0;
	double xmax[2];
	xmax[0] = 1;
	xmax[1] = 1;

	BoundedMopStats board(2, 2, (double *)&xmin, (double *)&xmax, &image,
//			&objective_function_1, &is_feasible_function_1);
			&objective_function_4, &is_feasible_function_2);

	{
		DepthFirstRecursiveStencil solver(.01, &l_inf, 7, 3);
		solver.solve(&board);
		return 0;
	}

	{
		puts("Using random sampling.");
		RandomSampleSolver solver;

		FILE *log = fopen("sampled_points.txt", "w");

		board.start_logging(log);
		solver.solve(&board, NUM_GUESSES, LONG_MAX);
		board.stop_logging();

		summarize(&image, "sampled_pareto.txt");
	}

	board.reset();

	{
		puts("Using genetic algorithm.");
		GeneticSolver gasolver(2, board.get_x_dimension(), 200, 50, .1, &get_index);

		FILE *log = fopen("ga_points.txt", "w");

		board.start_logging(log);
		gasolver.solve(&board, NUM_GUESSES, LONG_MAX);
		board.stop_logging();

		summarize(&image, "ga_pareto.txt");
	}

	board.reset();

	{
		puts("Using adaptive random.");
		IncrementalSampleSolver incsolver(50, 50, .1);

		FILE *log = fopen("inc_points.txt", "w");

		board.start_logging(log);
		incsolver.solve(&board, NUM_GUESSES, LONG_MAX);
		board.stop_logging();

		summarize(&image, "inc_pareto.txt");
	}

	return 0;
}
