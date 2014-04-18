/*
 * qtree.h
 *
 *  Created on: Apr 3, 2014
 *      Author: rever
 */

#ifndef qtree_H_
#define qtree_H_

#include "config.h"
#include "qtree_point.h"

#include <stdio.h>

#define BRANCH_FACTOR 1

#define UPPER_RIGHT 0
#define UPPER_LEFT 1
#define LOWER_LEFT 2
#define LOWER_RIGHT 3

typedef enum qtree_type_
{
	QTREE_TYPE_BRANCH = 13,
	QTREE_TYPE_LEAF = 56,
	QTREE_TYPE_NULL = 165,
} qtree_type;


typedef struct qtree_branch_
{
	qtree_point *lb;
	qtree_point *ub;
	struct qtree_branch_ *parent;
	qtree_type *types;
	void **branches;
} qtree_branch;

typedef struct
{
	int size;
	qtree_branch *parent;
	qtree_point *points[BRANCH_FACTOR];
} qtree_leaf;

typedef struct
{
	int dim;
	int two_2_dim;
	qtree_point *lb;
	qtree_point *ub;
	double width;
	qtree_branch *root;
} qtree;

typedef struct
{
	int depth;
	size_t pos_size;
	int *branch_locs;
	qtree_leaf *leaf;
	qtree_branch *parent;
} qtree_it;

qtree *qtree_new(qtree_point *lb, qtree_point *up, int dim);
void qtree_del(qtree *tree);

bool qtree_add(qtree *tree, qtree_point *point);
bool qtree_remove(qtree *tree, qtree_point *point);
bool qtree_clean_up(qtree *tree);

qtree_it *qtree_it_new(qtree *tree);
void qtree_it_del(qtree_it *it);

bool qtree_contains(qtree *tree, qtree_point *point);

void qtree_print(FILE *out, qtree *tree);

bool qtree_verify_bounds(qtree *tree, bool verbose);

bool qtree_point_equals(qtree_point *point1, qtree_point *point2);

bool is_pareto(qtree_point *point);

#endif /* qtree_H_ */
