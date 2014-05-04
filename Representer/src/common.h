/*
 * common.h
 *
 *  Created on: May 4, 2014
 *      Author: thallock
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "GeneticRepresenter.h"
#include "math_utils.h"

#include "../../Rtree/src/Qtree.h"

#include "metrics/RepresentationMetric.h"
#include "metrics/Delta.h"
#include "metrics/Epsilon.h"
#include "metrics/DistToClosest.h"
#include "metrics/Scalarization.h"
#include "metrics/Variation.h"

#include <limits.h>
#include <float.h>
#include <alloca.h>
#include <math.h>
#include <vector>
#include <memory>

void plot(const char *filename, InitialSet *set, char *mask, double fitness);
void plot_initial_set(const char *filename, InitialSet *set);
void bread_first_represent(InitialSet *set, int num_points, char *mask_out);

InitialSet *get_equidistant_initial_set(double spacing, int dim);
InitialSet *get_simple_pareto_initial_set(int num_points, int dim, bool equidistant);
InitialSet *get_uniform_random_initial_set(int num_points, int dim);
InitialSet *get_wavy_initial_set(int num_points, int dim);
InitialSet *get_bias_initial_set(int num_points, int dim);

void test_breadth_first();
void test_metric();
void represent();
void test_initial_sets();

#endif /* COMMON_H_ */
