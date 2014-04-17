/*
 * utils.h
 *
 *  Created on: Apr 6, 2014
 *      Author: thallock
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <vector>

double *copy_point(double *vec, int dim);
void print_point(FILE *file, double *vec, int dim, bool new_line);

bool first_dominates_second(double *first, double *second, int dim);
bool is_pareto_optimal(double *y, std::vector<double *> *all, int dim);
void set_pareto_optimals(std::vector<double *> *all, std::vector<double *> *pareto, int dim);

void log_guess(FILE *log, double *x, int xdim, double *y, int ydim);


#endif /* UTILS_H_ */
