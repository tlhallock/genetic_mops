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
#include "GeneticSolver.h"
#include "RandomSampleSolver.h"
#include "IncrementalSampleSolver.h"
#include "DepthFirstRecursiveStencil.h"

#define NUM_GUESSES 500

double get_index(double r)
{
	return r * r * r * r * r * r * r;
}

void test_pareto_equivalence();
static void compare_solvers();


int main()
{
	long seed = time(NULL);
	srand(seed);

//	test_pareto_equivalence();

	compare_solvers();

	return 0;
}

void compare_dfs()
{
	VectorImageSet image(2);

	// unit square
	double xmin[2];
	xmin[0] = 0;
	xmin[1] = 0;
	double xmax[2];
	xmax[0] = 1;
	xmax[1] = 1;

	BoundedMopStats board(2, 2, (double *) &xmin, (double *) &xmax, &image, &objective_function_4, &is_feasible_function_2);

	DepthFirstRecursiveStencil solver(.01, &l_inf, 7, 3);
	solver.solve(&board);
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

	std::cout << "epsilon = " << image->get_epsilon(&l_inf) << std::endl;
	std::cout << "delta   = " << image->get_delta(&l_inf)   << std::endl;

	std::vector<double*> *paretos = image->get_pareto_solutions();
	std::cout << "Number of pareto points: " << paretos->size() << std::endl;

	FILE *pareto_file = fopen(filename, "w");
	for (std::vector<double *>::iterator it = paretos->begin(); it != paretos->end(); ++it)
	{
		print_point(pareto_file, (*it), image->get_dim(), true);
	}
	fclose(pareto_file);

	printf("\n\n\n");
}

static void compare_solvers()
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
		GeneticSolver gasolver(2, 2, 3);

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
}
