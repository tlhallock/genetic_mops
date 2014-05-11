/*
 * test.cc
 *
 *  Created on: May 3, 2014
 *      Author: rever
 */

#include "common.h"

void test_breadth_first()
{
	int dim = 2;
	int size = 100;
	int num_to_use = 20;

	char *mask = (char *) alloca(sizeof(*mask) * size);
	char *all = (char *) alloca(sizeof(*mask) * size);
	for (int i = 0; i < size; i++)
	{
		all[i] = 1;
		if (i < num_to_use)
		{
			mask[i] = 1;
		}
		else
		{
			mask[i] = 0;
		}
	}

	InitialSet set(size, dim, &l_2);

	bread_first_represent(&set, num_to_use, mask);

	const char *filename = "a-foo-a-foo-a-bar.m";
	fclose(fopen(filename, "w"));
	plot(filename, &set, mask, 0.0);
}

void test_metric()
{
	int dim = 2;
	int size = 50;
	int num_to_use = 10;

	char *mask = (char *) alloca(sizeof(*mask) * size);
	char *all = (char *) alloca(sizeof(*mask) * size);
	for (int i = 0; i < size; i++)
	{
		all[i] = 1;
		if (i < num_to_use)
		{
			mask[i] = 1;
		}
		else
		{
			mask[i] = 0;
		}
	}

	InitialSet set(size, dim, &l_2);

	const char * filename = "tester.m";

	fclose(fopen(filename, "w"));

	double max = -DBL_MAX;

	for (int i = 0; i < 100000; i++)
	{
		for (int j = 0; j < 2 * size; j++)
		{
			int i1 = rand() % size;
			int i2 = rand() % size;

			char tmp = mask[i1];
			mask[i1] = mask[i2];
			mask[i2] = tmp;
		}

		double fitness = -13;//represent_metric(&set, mask, all);
		if (fitness < max)
		{
			continue;
		}
		max = fitness;
//		i++;
	}
	plot(filename, &set, mask, max);
}




void represent()
{
	srand(5000);

	int dim = 2;

	InitialSet set(50, 3, &l_2);

	GeneticRepresenter rep(50, 5);
	RepresentationMetric *m = new DistToClosest(&set, 2);
	std::auto_ptr<RepresentationMetric> metric(m);

	char *out_mask = (char *) malloc (sizeof(*out_mask) * set.size());

	rep.represent(10, metric.get(), out_mask);

	for (int i = 0; i < set.size(); i++)
	{
		if (out_mask[i])
		{
			printf("++ ");
		} else
		{
			printf("-- ");
		}

		for (int j = 0; j < dim; j++)
		{
			printf("%lf ", set.get(i)[j]);
		}
		fputc('\n', stdout);
	}

	free(out_mask);
}

static void summarize_metric(const char *filename, RepresentationMetric *metric, int num_to_use)
{
	printf("Summarizing %s\n", filename);
	char file_buff[256];

	int size = metric->get_set()->size();
	char *mask = (char *) malloc (sizeof(*mask) * size);
	double *costs = (double *) malloc (sizeof (*costs) * size);

	// Genetic algorithm...
	GeneticRepresenter rep(size, 10);
	rep.represent(num_to_use, metric, mask);

	double fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);
	sprintf(file_buff, "plots/%s_ga", filename);
	plot(file_buff, metric->get_set(), mask, fitness);

	// Random algorithm...
	sprintf(file_buff, "plots/%s_random", filename);
	random_sample(metric, num_to_use, mask, 100, false);
	fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);
	plot(file_buff, metric->get_set(), mask, fitness);

	// greedy random algorithm...
	sprintf(file_buff, "plots/%s_random_greedy", filename);
	random_sample(metric, num_to_use, mask, 100, true);
	fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);
	plot(file_buff, metric->get_set(), mask, fitness);

	if (metric->get_set()->size() < 25)
	{
		printf("backtracking...\n");
		sprintf(file_buff, "plots/%s_backtrack", filename);
		back_track_to_find_optimal(metric, mask, num_to_use);
		fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);
		plot(file_buff, metric->get_set(), mask, fitness);

//		sprintf(file_buff, "plots/%s_backtrack_any", filename);
//		back_track_to_find_optimal(metric, mask);
//		fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);
//		plot(file_buff, metric->get_set(), mask, fitness);
	}

	free(mask);
	free(costs);
}

void test_initial_sets()
{
	int dim = 2;
	int num_points = 100;
	int num_to_use = 10;
	double spacing = .1;

	char filename[256];

#define num_initial_sets 8
	const char *initial_names[num_initial_sets] =
	{
		"equidistant",
		"simple_equidistant",
		"simple_sample",
		"simple_equi",
		"uniform_random",
		"wavy",
		"bias",
		"bias_simple",
	};

	InitialSet **sets = (InitialSet **) malloc (sizeof(*sets) * num_initial_sets);

	{
		int index = 0;
		sets[index++] = get_equidistant_initial_set(spacing, dim);
		sprintf(filename, "plots/0_1_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_equidistant_initial_set(.05, 1);
		sprintf(filename, "plots/0_2_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_simple_pareto_initial_set(num_points, dim, false);
		sprintf(filename, "plots/0_3_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_simple_pareto_initial_set(num_points, dim, true);
		sprintf(filename, "plots/0_4_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_uniform_random_initial_set(num_points, dim);
		sprintf(filename, "plots/0_6_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_wavy_initial_set(num_points, dim);
		sprintf(filename, "plots/0_7_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_bias_initial_set(num_points, dim);
		sprintf(filename, "plots/0_8_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_bias_simple_set(num_points, dim);
		sprintf(filename, "plots/0_5_%s_initial_set", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
	}

	for (int i = 0; i < num_initial_sets; i++)
	{
		InitialSet *set = sets[i];
		printf("With initial set %s\n", initial_names[i]);

//		{
//			sprintf(filename, "plots/1_sort_%s", initial_names[i]);
//			char *mask = (char *) malloc (sizeof (*mask) * set->size());
//			sort_represent(set, num_to_use, mask);
//			plot(filename, set, mask, 0);
//			free(mask);
//		}

//		{
//			sprintf(filename, "plots/2_rtree_%s", initial_names[i]);
//			char *mask = (char *) malloc (sizeof (*mask) * set->size());
//			bread_first_represent(set, num_to_use, mask);
//			plot(filename, set, mask, 0);
//			free(mask);
//		}

//		{
//			sprintf(filename, "3_dist_to_closest_1_%s", initial_names[i]);
//			DistToClosest metric(set, 1);
//			summarize_metric(filename, &metric, num_to_use);
//		}

//		{
//			sprintf(filename, "4_dist_to_closest_10_%s", initial_names[i]);
//			DistToClosest metric(set, num_to_use);
//			summarize_metric(filename, &metric, num_to_use);
//		}

		{
			sprintf(filename, "5_max_log_dist_%s", initial_names[i]);
			MaximizeDists metric(set, 1);
			summarize_metric(filename, &metric, num_to_use);
		}

//		{
//			double *weights = (double *) malloc(sizeof(*weights) * 2);
//			weights[0] = .01;
//			weights[1] = 1;
//
//			std::vector<RepresentationMetric *> scalar1;
//			Epsilon epsilon(set, 1);
//			Delta delta(set, 1);
//
//			scalar1.push_back(&epsilon);
//			scalar1.push_back(&delta);
//
//			Scalarization metric(&scalar1, weights);
//			sprintf(filename, "6_epsilon_delta_scalar_01_1_%s", initial_names[i]);
//			summarize_metric(filename, &metric, num_to_use);
//
//
//			free(weights);
//		}

//		{
//			double *weights = (double *) malloc(sizeof(*weights) * 2);
//			weights[0] = 1;
//			weights[1] = .01;
//
//			std::vector<RepresentationMetric *> scalar1;
//			Epsilon epsilon(set, 1);
//			Delta delta(set, 1);
//
//			scalar1.push_back(&epsilon);
//			scalar1.push_back(&delta);
//
//			Scalarization metric(&scalar1, weights);
//			sprintf(filename, "7_epsilon_delta_scalar_10_1_%s", initial_names[i]);
//			summarize_metric(filename, &metric, num_to_use);
//
//
//			free(weights);
//		}

//		{
//			double *weights = (double *) malloc(sizeof(*weights) * 2);
//			weights[0] = 1;
//			weights[1] = 1;
//
//			std::vector<RepresentationMetric *> scalar1;
//			Epsilon epsilon(set, 1);
//			Delta delta(set, 1);
//
//			scalar1.push_back(&epsilon);
//			scalar1.push_back(&delta);
//
//			Scalarization metric(&scalar1, weights);
//			sprintf(filename, "8_epsilon_delta_scalar_11_1_%s", initial_names[i]);
//			summarize_metric(filename, &metric, num_to_use);
//
//
//			free(weights);
//		}

//		{
//			double *weights = (double *) malloc(sizeof(*weights) * 2);
//			weights[0] = 1;
//			weights[1] = 1;
//
//			std::vector<RepresentationMetric *> scalar1;
//			Epsilon epsilon(set, 3);
//			Delta delta(set, 3);
//
//			scalar1.push_back(&epsilon);
//			scalar1.push_back(&delta);
//
//			Scalarization metric(&scalar1, weights);
//			sprintf(filename, "9_epsilon_delta_scalar_11_2_%s", initial_names[i]);
//			summarize_metric(filename, &metric, num_to_use);
//
//
//			free(weights);
//		}

//		{
//			double *weights = (double *) malloc(sizeof(*weights) * 2);
//			weights[0] = 1;
//			weights[1] = .01;
//
//			std::vector<RepresentationMetric *> scalar1;
//			DistToClosest dist(set, 1);
//			Variation var(set);
//
//			scalar1.push_back(&dist);
//			scalar1.push_back(&var);
//
//			Scalarization metric(&scalar1, weights);
//			sprintf(filename, "9_dist_variation_scalar_%s", initial_names[i]);
//			summarize_metric(filename, &metric, num_to_use);
//
//
//			free(weights);
//		}
	}

	for (int i = 0; i < num_initial_sets; i++)
	{
		delete sets[i];
	}
	free(sets);
}
