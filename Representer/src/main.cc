/*
 * main.cc
 *
 *  Created on: Apr 27, 2014
 *      Author: rever
 */

#include <math.h>

#include "GeneticRepresenter.h"

double l_2(double *p1, double *p2, int dim)
{
	double sum = 0;
	for (int i=0;i<dim;i++)
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

double represent_metric(InitialSet *set, char *mask)
{
	return 1.0;
}

int main( int argc, char **argv)
{
	int dim = 3;

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
