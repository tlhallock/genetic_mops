/*
 * math_functions.h
 *
 *  Created on: Apr 20, 2014
 *      Author: thallock
 */

#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_

#include "common.h"

void objective_function_1(double *x, double *y_out);
void objective_function_2(double *x, double *y_out);
void objective_function_3(double *x, double *y_out);
void objective_function_4(double *x, double *y_out);

bool is_feasible_function_1(double *x);
bool is_feasible_function_2(double *x);

double l_inf(double *p1, double *p2, int dim);
double l_2(double *p1, double *p2, int dim);


#endif /* MATH_FUNCTIONS_H_ */
