/*
 * qtree_point.h
 *
 *  Created on: Apr 17, 2014
 *      Author: rever
 */

#ifndef QTREE_POINT_H_
#define QTREE_POINT_H_

#include <stdio.h>

typedef double qtree_point;

void qtree_point_print(FILE *out, qtree_point *point, int dim, bool newline);

qtree_point *qtree_point_new0(int dim);
qtree_point *qtree_point_new(int dim, double *values);
qtree_point *qtree_point_new_rand(int dim);

qtree_point *qtree_point_dup(int dim, qtree_point *point);

void qtree_point_del(qtree_point *point);



bool qtree_point_equals(qtree_point *point1, qtree_point *point2, int dim);

bool qtree_point_dominates(qtree_point *point1,  qtree_point *point2, int dim);

#endif /* QTREE_POINT_H_ */
