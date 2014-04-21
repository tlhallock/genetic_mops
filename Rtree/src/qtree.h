/*
 * qtree.h
 *
 *  
 *      Author: rever
 */

#ifndef qtree_H_
#define qtree_H_

#include "config.h"
#include "qtree_point.h"

#include <stdio.h>

namespace qtree
{

#define BRANCH_FACTOR 50

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


class QtreeLeaf
{
private:
	int size;
	QtreeBranch *parent;
	qtree_point *points[BRANCH_FACTOR];

	int get_dim();
	int get_two_2_dim();
public:
	QtreeLeaf(QtreeBranch *parent, int dim);
	~QtreeLeaf();

	void qtree_leaf_get_min(double *y_out, int dim_of_interest);
	void qtree_leaf_apply(void (*fctn)(qtree_point *pnt, void *arg), void *arg);
	int qtree_leaf_index_of(qtree_point *point);
	int qtree_leaf_get_parents_quad();
	void qtree_leaf_print(FILE *out, int depth);
};


class QtreeBranch
{
private:
	qtree_point *lb;
	qtree_point *ub;
	struct qtree_branch_ *parent;
	qtree_type *types;
	void **branches;


	int get_dim();
	int get_two_2_dim();
public:
	QtreeBranch(QtreeBranch *parent, qtree_point *lb, qtree_point *ub, int two_2_dim);
	~QtreeBranch();

	void print(FILE *out, int depth);
	void get_min(double *y_out, int dim_of_interest);
	void apply(void (*fctn)(qtree_point *pnt, void *arg), void *arg);
	int count();
	QtreeLeaf *get_leaf(int quadrant);
	QtreeLeaf *find(qtree_point *point);
	int get_parents_quad();
	bool is_empty();
};

class Qtree
{
private:
	int dim;
	int two_2_dim;
	qtree_point *lb;
	qtree_point *ub;
	QtreeBranch *root;
	static QtreeBranch *qtree_root_grow(QtreeBranch *branch, qtree_point *direction, int dim, int two_2_dim);
	bool in_bounds(qtree *tree, qtree_point *point);
	QtreeLeaf *find(qtree *tree, qtree_point *point);
public:
	qtree *Qtree(qtree_point *lb, qtree_point *ub, int dim);
	void ~Qtree(qtree *tree);

	bool add(qtree_point *point);
	void clear();
	bool remove(qtree_point *point);
	int count();
	bool contains(qtree_point *point);

	void get_min(double *y_out, int dim);
	void apply(void (*fctn) (qtree_point *pnt, void *arg), void *arg);

	size_t get_memory_usage();
	qtree_it *it_new();

	void print(FILE *out);
};

typedef struct
{
	int depth;
	size_t pos_size;
	int *branch_locs;
	QtreeLeaf *leaf;
	QtreeBranch *parent;
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

size_t qtree_get_memory_usage(qtree *tree);
int qtree_count(qtree *tree);
void qtree_clear(qtree *tree);

void qtree_apply(qtree *tree, void (*fctn) (qtree_point *pnt, void *arg), void *arg);

void qtree_get_min(qtree *tree, qtree_point *y_out, int dim);
bool qtree_in_bounds(qtree *tree, qtree_point *point);

}

#endif /* qtree_H_ */
