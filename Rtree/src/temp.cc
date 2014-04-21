/*
 * temp.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */




/*
 * qtree.c
 *
 *
 *      Author: rever
 */

#include "qtree.h"

#include "qtree_point.h"

#include <stdlib.h>
#include <stdbool.h>


namespace qtree
{



// the points are kept...
int qtree_get_quadrant(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim);
static void qtree_point_assign(qtree_leaf *leaf, qtree_point *point, int dim);
void qtree_bounds_select(qtree_point *lb, qtree_point *ub, int quad, int dim);


static void qtree_leaf_get_min(qtree_leaf *leaf, double *y_out, int dim_of_interest, int dim);
static void qtree_leaf_apply(qtree_leaf *leaf, void (*fctn)(qtree_point *pnt, void *arg), void *arg);
static int qtree_leaf_index_of(qtree_leaf *leaf, qtree_point *point, int dim);
static int qtree_leaf_get_parents_quad(qtree_leaf *leaf, int two_2_dim);
static qtree_leaf *qtree_leaf_new(qtree_branch *parent, int dim);
static void qtree_leaf_del(qtree_leaf *leaf);
static void qtree_leaf_print(FILE *out, qtree_leaf *leaf, int depth, int dim);


static void qtree_branch_print(FILE *out, qtree_branch *branch, int depth, int dim, int two_2_dim);
static void qtree_branch_get_min(qtree_branch *branch, double *y_out, int dim_of_interest, int dim, int two_2_dim);
static void qtree_branch_apply(qtree_branch *branch, void (*fctn)(qtree_point *pnt, void *arg), void *arg, int two_2_dim);
static int qtree_branch_count(qtree_branch *branch, int two_2_dim);
static qtree_branch *qtree_branch_new(qtree_branch *parent, qtree_point *lb, qtree_point *ub, int two_2_dim);
static void qtree_branch_del(qtree_branch *branch, int two_2_dim);
static qtree_leaf *qtree_branch_get_leaf(qtree_branch *branch, int quadrant, int dim);
static qtree_leaf *qtree_branch_find(qtree_branch *branch, qtree_point *point, int dim);
static int qtree_branch_get_parents_quad(qtree_branch *branch, int two_2_dim);
bool qtree_branch_is_empty(qtree_branch *branch, int two_2_dim);


void qtree_get_min(qtree *tree, double *y_out, int dim);
bool qtree_in_bounds(qtree *tree, qtree_point *point);
void qtree_apply(qtree *tree, void (*fctn) (qtree_point *pnt, void *arg), void *arg);
size_t qtree_get_memory_usage(qtree *tree);
void qtree_clear(qtree *tree);
int qtree_count(qtree *tree);
bool qtree_contains(qtree *tree, qtree_point *point);
qtree_leaf *qtree_find(qtree *tree, qtree_point *point);
bool qtree_remove(qtree *tree, qtree_point *point);
qtree_it *qtree_it_new(qtree *tree);
qtree *qtree_new(qtree_point *lb, qtree_point *ub, int dim);
void qtree_del(qtree *tree);
static qtree_branch *qtree_root_grow(qtree_branch *branch, qtree_point *direction, int dim, int two_2_dim);
bool qtree_add(qtree *tree, qtree_point *point);
void qtree_print(FILE *out, qtree *tree);

}
