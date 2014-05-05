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

static void summarize_metric(const char *filename, RepresentationMetric *metric)
{
	printf("Summarizing %s\n", filename);

	int size = metric->get_set()->size();
	GeneticRepresenter rep(size, 10);

	char *mask = (char *) malloc (sizeof(*mask) * size);
	double *costs = (double *) malloc (sizeof (*costs) * size);

	rep.represent(10, metric, mask);

	double fitness = metric->get_fitness(mask, metric->get_set()->get_all_pnts(), costs);

	plot(filename, metric->get_set(), mask, fitness);

	free(mask);
	free(costs);
}

void test_initial_sets()
{
	int dim = 2;
	int num_points = 100;
	double spacing = .1;

	char filename[256];

	const char *initial_names[6] =
	{
		"equidistant",
		"simple_sample",
		"simple_equi",
		"uniform_random",
		"wavy",
		"bias",
	};

	int num_initial_sets = 6;
	InitialSet **sets = (InitialSet **) malloc (sizeof(*sets) * num_initial_sets);

	{
		int index = 0;
		sets[index++] = get_equidistant_initial_set(spacing, dim);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_simple_pareto_initial_set(num_points, dim, false);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_simple_pareto_initial_set(num_points, dim, true);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_uniform_random_initial_set(num_points, dim);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_wavy_initial_set(num_points, dim);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
		sets[index++] = get_bias_initial_set(num_points, dim);
		sprintf(filename, "plots/0_%s_initial_set.m", initial_names[index-1]);
		plot_initial_set(filename, sets[index-1]);
	}

	for (int i = 0; i < num_initial_sets; i++)
	{
		InitialSet *set = sets[i];
		printf("With initial set %s\n", initial_names[i]);

		{
			sprintf(filename, "plots/1_dist_to_closest_1_%s.m", initial_names[i]);
			DistToClosest metric(set, 1);
			summarize_metric(filename, &metric);
		}

		{
			sprintf(filename, "plots/2_dist_to_closest_10_%s.m", initial_names[i]);
			DistToClosest metric(set, 10);
			summarize_metric(filename, &metric);
		}

		{
			std::vector<RepresentationMetric *> scalar1;
			Epsilon epsilon(set, 1);
			Delta delta(set, 1);

			double *weights1 = (double *) malloc(sizeof(*weights1) * 2);
			weights1[0] = 1;
			weights1[1] = 2;

			scalar1.push_back(&epsilon);
			scalar1.push_back(&delta);

			Scalarization metric(&scalar1, weights1);
			sprintf(filename, "plots/3_epsilon_delta_scalar_%s.m", initial_names[i]);
			summarize_metric(filename, &metric);

			free(weights1);
		}
	}

	for (int i = 0; i < num_initial_sets; i++)
	{
		delete sets[i];
	}
	free(sets);
}
