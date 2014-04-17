/*
 * qtree.h
 *
 *  Created on: Apr 3, 2014
 *      Author: rever
 */

#ifndef qtree_H_
#define qtree_H_

#include "config.h"
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

typedef struct
{
	double x;
	double y;
} qtree_point;

typedef struct qtree_branch_
{
	struct qtree_branch_ *parent;
	qtree_type types[4];
	void *branches[4];
} qtree_branch;

typedef struct
{
	qtree_branch *parent;
	int size;
	qtree_point points[BRANCH_FACTOR];
} qtree_leaf;

typedef struct
{
	qtree_point lower_left;
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

qtree_point *qtree_point_new(double x, double y);
void qtree_point_del(qtree_point *point);

qtree *qtree_new(qtree_point *lower_left, double width);
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
