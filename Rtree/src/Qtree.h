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

typedef enum qtree_type_
{
	QTREE_TYPE_BRANCH = 13,
	QTREE_TYPE_LEAF = 56,
	QTREE_TYPE_NULL = 165,
} qtree_type;

bool qtree_regions_are_adjacent(qtree_point *lb1, qtree_point *ub1, qtree_point *lb2, qtree_point *ub2, int dim);
bool qtree_region_contains(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim);
int qtree_get_quadrant(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim);
void qtree_bounds_select(qtree_point *lb, qtree_point *ub, int quad, int dim);
void qtree_print_quadrant(FILE *out, int quadrant, int dim);


class QtreeLeaf;
class QtreeBranch;
class Qtree;


class QtreeLeaf
{
	friend class QtreeBranch;
	friend class Qtree;
private:
	Qtree *tree;
	int size;
	qtree_point *lb;
	qtree_point *ub;
	QtreeBranch *parent;
	qtree_point **points;
	void **refs;


	int get_dim();
	int get_two_2_dim();
	int get_branch_factor();
public:
	QtreeLeaf(QtreeBranch *parent, qtree_point *lb, qtree_point *ub);
	~QtreeLeaf();

	void get_min(double *y_out, int dim_of_interest);
	void apply(void (*fctn)(qtree_point *pnt, void *ref, void *arg), void *arg);
	int index_of(qtree_point *point);
	int get_parents_quad();
	void print(FILE *out, int depth);
	void assign(qtree_point *point, void *ref);
	int get_size() { return size; }
	void *get_ref(int index);
	QtreeBranch *get_parent() { return parent; }
	bool is_empty() { return size == 0; }

	bool is_pareto(qtree_point *point);
	int count_dominating(qtree_point *point);

	bool find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int),
			double *cmin, qtree_point *clb, qtree_point *cub, bool can_prune);

	void verify();

	bool get_random(qtree_point *out);
};


class QtreeBranch
{
	friend class QtreeLeaf;
	friend class Qtree;
private:
	Qtree *tree;

	qtree_point *lb;
	qtree_point *ub;

	QtreeBranch *parent;
	qtree_type *types;
	void **branches;

	int count;

	int get_dim();
	int get_two_2_dim();
	int get_branch_factor();
	Qtree *get_tree() { return tree; }

	int recount();
	void add_to_size(int inc);
	int get_depth();
public:
	QtreeBranch(Qtree *tree, QtreeBranch *parent, qtree_point *lb, qtree_point *ub);
	~QtreeBranch();

	void print(FILE *out, int depth);
	void get_min(double *y_out, int dim_of_interest);
	void apply(void (*fctn)(qtree_point *pnt, void *ref, void *arg), void *arg);
	QtreeLeaf *get_leaf(int quadrant);
	QtreeLeaf *find(qtree_point *point);
	int get_parents_quad();
	bool is_empty();
	void set_branch(int quad, qtree_type type, void *ptr)
	{
		types[quad] = type;
		branches[quad] = ptr;
	}
	QtreeBranch *get_parent() { return parent; }

	int size() { return count; }

	bool is_pareto(qtree_point *point);
	int count_dominating(qtree_point *point);

	/** This method detached the branch from the tree. **/
	bool combine();

	void verify();

	bool find_nearest(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int), double *cmin, qtree_point *lb, qtree_point *ub, int max_depth);
	bool get_random(qtree_point *out);
};




class Qtree
{
	friend class QtreeLeaf;
	friend class QtreeBranch;
private:
	int dim;
	int two_2_dim;
	int branch_factor;
	qtree_point *lb;
	qtree_point *ub;
	QtreeBranch *root;
	void grow_root(qtree_point *direction);
	QtreeLeaf *find(qtree_point *point);
	bool in_current_bounds(qtree_point *point);
public:
	Qtree(qtree_point *lb, qtree_point *ub, int dim, int branch_factor);
	Qtree(double _lb, double _ub, int _dim, int branch_factor);
	Qtree(int dim, int branch_factor);
	~Qtree();

	bool add(qtree_point *point, void *ref);
	void clear();
	bool remove(qtree_point *point);
	int count();
	bool contains(qtree_point *point);
	void *get(qtree_point *point);

	void get_min(double *y_out, int dim);
	void apply(void (*fctn)(qtree_point *pnt, void *ref, void *arg), void *arg);

	int get_dim() { return dim; }
	int get_two_2_dim() { return two_2_dim; }

	size_t get_memory_usage();

	bool is_pareto(qtree_point *point);
	int count_dominating(qtree_point *point);

	double get_nearest_point(qtree_point *point, qtree_point *out, double (*norm)(qtree_point *, qtree_point *, int));
	bool get_random(qtree_point *out);

	int get_branch_factor() { return branch_factor; }

	void verify() { root->verify(); }

	void print(FILE *out);
};

}

#endif /* qtree_H_ */
