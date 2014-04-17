/*
 * pareto_header.h
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef PARETO_HEADER_H_
#define PARETO_HEADER_H_

typedef struct
{
	unsigned int size;
	size_t capacity;

	unsigned int dim;
	double **points;
} pareto_set;

typedef struct
{
	int size;
	char *mask;
} mask;


typedef struct
{
	int npoints;
	int dim;
	double **distances;
} dist_lookup;

pareto_set *pareto_set_new(unsigned int capacity, unsigned int dim);
void pareto_set_del(pareto_set *set);

void pareto_set_add(pareto_set *set, double *point);

mask *pareto_set_mask_new(pareto_set *set);
void pareto_set_mask_del(mask *msk);
void pareto_set_mask_on(mask *msk, int i, bool on);

bool pareto_set_get_delta(dist_lookup *lookup, mask *msk, double *out_delta, int *p1_out, int *p2_out);
bool pareto_set_get_epsilon(dist_lookup *lookup, mask *msk, double *out_epsilon, int *p1_out, int *p2_out);

dist_lookup *pareto_set_distance_lookup_new(pareto_set *set, double (*metric) (double *p1, double *p2));
void pareto_set_distance_lookup_del(dist_lookup *lookup);
double pareto_set_distance_lookup_get(dist_lookup *lookup, int i, int j);
void pareto_set_distance_lookup_set(dist_lookup *lookup, int i, int j, double value);

pareto_set *pareto_set_read(FILE *in);

void pareto_set_backtrack(pareto_set *set);

#endif /* PARETO_HEADER_H_ */
