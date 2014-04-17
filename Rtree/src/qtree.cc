/*
 * qtree.c
 *
 *  Created on: Apr 3, 2014
 *      Author: rever
 */

#include "qtree.h"

#include <stdlib.h>
#include <stdbool.h>



static void qtree_leaf_print(FILE *out, qtree_leaf *tree, int depth)
{
	int i, j;

	for(j=0;j<depth;j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "leaf d=%d s=%d:\n", depth, tree->size);

	for (i=0;i<tree->size;i++)
	{
		for(j=0;j<depth+1;j++)
		{
			fputc('\t', out);
		}
		fprintf(out, "%lf %lf\n", tree->points[i].x, tree->points[i].y);
	}
}


static void qtree_branch_print(FILE *out, qtree_branch *tree, int depth)
{
	int i, j;

	for(j=0;j<depth;j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "branch d=%d:\n", depth / 2);

	char * desc[4] =
	{
			"upper right",
			"upper left",
			"lower left",
			"lower right",
	};

	for (i=0;i<4;i++)
	{
		char *type = NULL;
		switch(tree->types[i])
		{
		case QTREE_TYPE_BRANCH:
			type = "branch";
			break;
		case QTREE_TYPE_LEAF:
			type = "leaf";
			break;
		case QTREE_TYPE_NULL:
			type = "null";
			break;
		}

		for(j=0;j<depth+1;j++)
		{
			fputc('\t', out);
		}

		fprintf(out, "%s: %s\n", desc[i], type);

		switch(tree->types[i])
		{
		case QTREE_TYPE_BRANCH:
			qtree_branch_print(out, (qtree_branch *) tree->branches[i], depth + 2);
			break;
		case QTREE_TYPE_LEAF:
			qtree_leaf_print(out, (qtree_leaf *) tree->branches[i], depth + 2);
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}
}

void qtree_print(FILE *out, qtree *tree)
{
	qtree_branch_print(out, tree->root, 0);
}


qtree_point *qtree_point_new(double x, double y)
{
	qtree_point *ret_val = (qtree_point *) malloc(sizeof (*ret_val));

	ret_val->x = x;
	ret_val->y = y;

	return ret_val;
}

void qtree_point_del(qtree_point *point)
{
	free(point);
}


static qtree_leaf *qtree_leaf_new(qtree_branch *parent)
{
	qtree_leaf *ret_val = (qtree_leaf *) malloc (sizeof (*ret_val));

	ret_val->size = 0;
	ret_val->parent = parent;

	return ret_val;
}

static qtree_branch *qtree_branch_new(qtree_branch *parent)
{
	qtree_branch *ret_val = (qtree_branch *) malloc (sizeof (*ret_val));
	ret_val->parent = parent;

	int i;
	for (i=0; i<4; i++)
	{
		ret_val->types[i] = QTREE_TYPE_NULL;
		ret_val->branches[i] = NULL;
	}

	return ret_val;
}

static void qtree_leaf_del(qtree_leaf *leaf)
{
	free(leaf);
}

static void qtree_branch_del(qtree_branch *branch)
{
	int i;
	for (i=0; i<4; i++)
	{
		qtree_type type = branch->types[i];
		if (type == QTREE_TYPE_BRANCH)
		{
			qtree_branch_del((qtree_branch *) branch->branches[i]);
		}
		else if (type == QTREE_TYPE_LEAF)
		{
			qtree_leaf_del((qtree_leaf *) branch->branches[i]);
		}
		else if (type == QTREE_TYPE_NULL)
		{
			continue;
		}
		else
		{
			puts("Error 195493955");
		}
	}

	free(branch);
}

qtree *qtree_new(qtree_point *lower_left, double width)
{
	qtree *ret_val = (qtree *) malloc (sizeof(*ret_val));

	ret_val->lower_left.x = lower_left->x;
	ret_val->lower_left.y = lower_left->y;
	ret_val->width = width;
	ret_val->root = qtree_branch_new(NULL);

	return ret_val;
}

void qtree_del(qtree *tree)
{
	qtree_branch_del(tree->root);
	free(tree);
}


int qtree_get_quadrant(qtree_point *lower_left, double width, qtree_point *point)
{
//		printf("quradrant of %lf %lf in %lf %lf %lf %lf is ", point->x, point->y,
//				lower_left->x, lower_left->y, lower_left->x + width,
//				lower_left->y + width);
	if (ERROR_CHECKING)
	{
		if (point->x < lower_left->x)
		{
			puts("Error 27467hfsadh");
			exit(1);
		}
		if (point->x > lower_left->x + width)
		{
			puts("Error 27467hfsadh");
			exit(1);
		}
		if (point->y < lower_left->y)
		{
			puts("Error 27467hfsadh");
			exit(1);
		}
		if (point->y > lower_left->y + width)
		{
			puts("Error 27467hfsadh");
			exit(1);
		}
	}

	bool lt_x = point->x < lower_left->x + width / 2;
	bool lt_y = point->y < lower_left->y + width / 2;

	if (lt_x)
	{
		if (lt_y)
		{
			return LOWER_LEFT;
		}
		else
		{
			return UPPER_LEFT;
		}
	}
	else
	{
		if (lt_y)
		{
			return LOWER_RIGHT;
		}
		else
		{
			return UPPER_RIGHT;
		}
	}
}

static void qtree_select(qtree_point *point, double width, int quadrant)
{
	switch (quadrant)
	{
	case LOWER_LEFT:
		return;
	case LOWER_RIGHT:
		point->x += width / 2;
		return;
	case UPPER_LEFT:
		point->y += width / 2;
		return;
	case UPPER_RIGHT:
		point->x += width / 2;
		point->y += width / 2;
		return;
	default:
		puts("Error 0184107493");
	}
}


static int qtree_descend(qtree_point *lower_left, double *width, qtree_point *point)
{
	int quadrant = qtree_get_quadrant(lower_left, *width, point);
	qtree_select(lower_left, *width, quadrant);
	*width /= 2;
	return quadrant;
}

qtree_leaf *qtree_branch_get_leaf(qtree_branch *branch, int quadrant)
{
	int type = branch->types[quadrant];
	if (type == QTREE_TYPE_LEAF)
	{
		return (qtree_leaf *) branch->branches[quadrant];
	}
	else if (type == QTREE_TYPE_NULL)
	{
		qtree_leaf *new_leaf = qtree_leaf_new(branch);
		branch->types[quadrant] = QTREE_TYPE_LEAF;
		branch->branches[quadrant] = new_leaf;

		return new_leaf;
	}
	else
	{
		puts("Error 204810463");
		return NULL;
	}
}

static qtree_leaf *qtree_branch_find(qtree_branch *branch, qtree_point *point, qtree_point *lower_left, double *width)
{
	int quadrant;
	qtree_type type;

	for(;;)
	{
		quadrant = qtree_descend(lower_left, width, point);
		type = ((qtree_branch *) branch)->types[quadrant];

		if (type != QTREE_TYPE_BRANCH)
		{
			break;
		}

		branch = (qtree_branch *) branch->branches[quadrant];
	}

	return qtree_branch_get_leaf(branch, quadrant);
}

static void qtree_point_assign(qtree_leaf *leaf, qtree_point *point)
{
	int size = leaf->size++;
	leaf->points[size].x = point->x;
	leaf->points[size].y = point->y;
}


bool qtree_point_equals(qtree_point *point1, qtree_point *point2)
{
	double diff;

	diff = point1->x - point2->x;
	if (!IS_ZERO(diff))
	{
		return false;
	}

	diff = point1->y - point2->y;
	if (!IS_ZERO(diff))
	{
		return false;
	}

	return true;
}

static int qtree_leaf_index_of(qtree_leaf *leaf, qtree_point *point)
{
	int i;

	for (i=0; i<leaf->size; i++)
	{
		if (qtree_point_equals(&leaf->points[i], point))
		{
			return i;
		}
	}

	return -19571938;
}

static int qtree_leaf_get_parents_quad(qtree_leaf *leaf)
{
	qtree_branch *parent = leaf->parent;
	for (int i=0;i<4;i++)
	{
		if (parent->branches[i] == leaf)
		{
			return i;
		}
	}
	return -587658765;
}

static int qtree_branch_get_parents_quad(qtree_branch *branch)
{
	qtree_branch *parent = branch->parent;
	if (parent == NULL)
	{
		puts("Error 15285298750");
		exit(1);
	}

	for (int i=0;i<4;i++)
	{
		if (parent->branches[i] == branch)
		{
			return i;
		}
	}
	return -587658765;
}

bool qtree_add(qtree *tree, qtree_point *point)
{
	qtree_point lower_left;
	lower_left.x = tree->lower_left.x;
	lower_left.y = tree->lower_left.y;

	double width = tree->width;
	qtree_leaf *leaf = qtree_branch_find(tree->root, point, &lower_left, &width);

	if (qtree_leaf_index_of(leaf, point) >= 0)
	{
		return false;
	}

	while (leaf->size == BRANCH_FACTOR)
	{
		qtree_branch *parent = leaf->parent;
		int quad = qtree_leaf_get_parents_quad(leaf);

		qtree_branch *new_branch = qtree_branch_new(parent);
		parent->types[quad] = QTREE_TYPE_BRANCH;
		parent->branches[quad] = new_branch;

		for (int i=0; i<BRANCH_FACTOR; i++)
		{
			quad = qtree_get_quadrant(&lower_left, width, &leaf->points[i]);
			qtree_leaf *new_leaf = qtree_branch_get_leaf(new_branch, quad);
			qtree_point_assign(new_leaf, &leaf->points[i]);
		}

		qtree_leaf_del(leaf);

		quad = qtree_descend(&lower_left, &width, point);
		leaf = (qtree_leaf *) qtree_branch_get_leaf(new_branch, quad);
	}

	qtree_point_assign(leaf, point);
	return true;
}

bool qtree_branch_is_empty(qtree_branch *branch)
{
	for (int i=0;i<4;i++)
	{
		qtree_type type = branch->types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			if (!qtree_branch_is_empty((qtree_branch *) branch->branches[i]))
			{
				return false;
			}
			break;
		case QTREE_TYPE_LEAF:
			if (((qtree_leaf *) branch->branches[i])->size != 0)
			{
				return false;
			}
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return true;
}

bool qtree_remove(qtree *tree, qtree_point *point)
{
	qtree_point lower_left;
	lower_left.x = tree->lower_left.x;
	lower_left.y = tree->lower_left.y;

	double width = tree->width;
	qtree_leaf *leaf = qtree_branch_find(tree->root, point, &lower_left, &width);

	int index = qtree_leaf_index_of(leaf, point);
	if (index < 0)
	{
		return false;
	}

	int size = --leaf->size;

	if (size != index)
	{
		leaf->points[index].x = leaf->points[size].x;
		leaf->points[index].y = leaf->points[size].y;
	}

	if (size != 0)
	{
		return true;
	}

	// clean up
	qtree_branch *parent = leaf->parent;
	int quad = qtree_leaf_get_parents_quad(leaf);
	parent->types[quad] = QTREE_TYPE_NULL;
	parent->branches[quad] = NULL;

	qtree_leaf_del(leaf);

	while (parent->parent != NULL
			&& qtree_branch_is_empty(parent))
	{
		int quad = qtree_branch_get_parents_quad(parent);
		parent->parent->types[quad] = QTREE_TYPE_NULL;
		parent->parent->branches[quad] = NULL;
		qtree_branch_del(parent);
	}

	return true;
}

qtree_it *qtree_it_new(qtree *tree)
{
	qtree_it *ret_val = (qtree_it *) malloc (sizeof (*ret_val));

	ret_val->pos_size = 128;
	ret_val->branch_locs = (int *) malloc (sizeof *ret_val->branch_locs);

	ret_val->depth = 0;
	qtree_branch *b = tree->root;
	while (b->types[0] == QTREE_TYPE_BRANCH)
	{
		ret_val->branch_locs[ret_val->depth++] = 0;
		b = (qtree_branch *) b->branches[0];

		if (ret_val->depth > 128)
		{
			puts("Error 7210720570");
		}
	}

	if (b->types[0] == QTREE_TYPE_NULL)
	{
		b->types[0] = QTREE_TYPE_LEAF;
		b->branches[0] = qtree_leaf_new(b);
	}

	ret_val->leaf = (qtree_leaf *) b->branches[0];

	return ret_val;
}

qtree_leaf *qtree_find(qtree *tree, qtree_point *point, qtree_point *lower_left, double *width)
{
	if (point->y < tree->lower_left.y || point->y > tree->lower_left.y + tree->width)
	{
		puts("Error 048277174");
		return NULL;
	}
	if (point->x < tree->lower_left.x || point->x > tree->lower_left.x + tree->width)
	{
		puts("Error 048277174");
		return NULL;
	}

	lower_left->x = tree->lower_left.x;
	lower_left->y = tree->lower_left.y;
	*width = tree->width;

	return qtree_branch_find(tree->root, point, lower_left, width);
}


bool qtree_contains(qtree *tree, qtree_point *point)
{
	qtree_point lower_left_out;
	double width_out;
	qtree_leaf *leaf = qtree_find(tree, point, &lower_left_out, &width_out);
	return qtree_leaf_index_of(leaf, point) >= 0;
}

int qtree_branch_count(qtree_branch *branch)
{
	int count = 0;

	for (int i=0;i<4;i++)
	{
		qtree_type type = branch->types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			count += qtree_branch_count((qtree_branch *) branch->branches[i]);
			break;
		case QTREE_TYPE_LEAF:
			count += ((qtree_leaf *) branch->branches[i])->size;
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return count;
}
