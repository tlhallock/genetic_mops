/*
 * pareto_set_generator.c
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#define N_POINTS 10

double func(double x)
{
	return 1 + sqrt(1 - (x - 1) * (x - 1));
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	FILE *out = fopen("../ClusterBackTracker/input.txt", "w");

	fprintf(out, "%d %d\n", N_POINTS, 2);

	int i;
	for (i = 0; i < N_POINTS; i++)
	{
		double x = rand() / (double) INT_MAX;
		double y = func(x);

		fprintf(out, "%lf %lf\n", x, y);
	}

	fclose(out);
	return 0;
}
