/*
 * main.cc
 *
 *  Created on: Apr 27, 2014
 *      Author: rever
 */

#include <math.h>
#include <float.h>
#include <alloca.h>

#include "GeneticRepresenter.h"

double l_2(double *p1, double *p2, int dim)
{
	double sum = 0;
	for (int i = 0; i < dim; i++)
	{
		double diff = p1[i] - p2[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}

double get_weight(int i)
{
	return 1.0;
}

double get_variance(InitialSet *set, char *to_use)
{
	int size = set->size();
	int index = 0;
	double *dists = (double *) alloca(sizeof(*dists) * size);

	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (!to_use[i])
		{
			continue;
		}
		double min = DBL_MAX;
		for (int j = 0; j < size; j++)
		{
			if (!to_use[j] || i == j)
			{
				continue;
			}
			double d = set->get_distance(i, j);
			if (d < min)
			{
				min = d;
			}
		}
		dists[index++] = min;
		sum += min;
	}

	double avg = sum / index;

	double sum2 = 0;
	for (int i = 0; i < index; i++)
	{
		double d = dists[i] - avg;
		sum2 += d * d;
	}
	double std = sqrt(sum2);

	return sum2;
}

static int count(int size, char *set)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (set[i] == 0)
		{
			continue;
		}
		count++;
	}
	return count;
}

double get_deviance_cost(InitialSet *set, char *to_use, char *to_represent)
{
	int size = set->size();
	int index = 0;

	double variance = get_variance(set, to_represent);

	double expected_distance = count(set->size(), to_represent) * sqrt(variance) / count(set->size(), to_use);

	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (!to_use[i])
		{
			continue;
		}
		double min = DBL_MAX;
		for (int j = 0; j < size; j++)
		{
			if (!to_use[j] || i == j)
			{
				continue;
			}
			double d = set->get_distance(i, j);
			if (d < min)
			{
				min = d;
			}
		}

		double c = abs(expected_distance - min);
		sum += c;
	}

	double avg = sum / index;
	return avg;
}

double represent_metric(InitialSet *set, char *to_use, char *to_represent)
{
	int NUM_TO_USE = 5;

	double sum = 0.0;

	for (int i = 0; i < set->size(); i++)
	{
		if (!to_represent[i])
		{
			continue;
		}

		int nearest_index[NUM_TO_USE];
		double nearest_dist[NUM_TO_USE];

		set->get_n_nearest(i, NUM_TO_USE, nearest_index, nearest_dist, to_use);

		double avg = 0;
		for (int j = 0; j < NUM_TO_USE; j++)
		{
			avg += nearest_dist[j];
		}
		sum += avg / NUM_TO_USE;
	}

	double f1 = -sum;
	double f2 = -200 * get_variance(set, to_use);

//	printf("Closest sum = %lf, deviance cost = %lf\n", f1, f2);

	return f1 + f2;
}

static void represent()
{
	srand(5000);

	int dim = 2;

	InitialSet set(50, 3, &l_2);

	GeneticRepresenter rep(50, 5);

	char *opt_mask = (char *) malloc (sizeof(*opt_mask) * set.size());

	rep.represent(&set, 10, &represent_metric, opt_mask);

	for (int i = 0; i < set.size(); i++)
	{
		if (opt_mask[i])
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

	free(opt_mask);
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

		double fitness = represent_metric(&set, mask, all);
		if (fitness < max)
		{
			continue;
		}
		max = fitness;
//		i++;
	}
	plot(filename, &set, mask, max);
}

void bread_first_represent(InitialSet *set, int num_points, char *mask_out);

void test_breadth_first()
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

	bread_first_represent(&set, num_to_use, mask);

	const char *filename = "a-foo-a-foo-a-bar.m";
	fclose(fopen(filename, "w"));
	plot(filename, &set, mask, 0.0);
}


int main(int argc, char **argv)
{
//	represent();
//	test_metric();
	test_breadth_first();
	puts("Done.");
}
