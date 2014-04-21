/*
 * QtreeBranch.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */



#include "qtree.h"



namespace qtree
{
int QtreeBranch::get_dim()
{

}

int QtreeBranch::get_two_2_dim()
{
}

QtreeBranch::QtreeBranch(QtreeBranch* _parent, qtree_point* _lb, qtree_point* _ub, int two_2_dim)
{
	parent = _parent;

	lb = _lb;
	ub = _ub;

	types = (qtree_type *) malloc (sizeof (*types) * two_2_dim);
	branches = (void **) malloc (sizeof (*branches) * two_2_dim);

	int i;
	for (i=0; i<two_2_dim; i++)
	{
		types[i] = QTREE_TYPE_NULL;
		branches[i] = NULL;
	}
}

QtreeBranch::~QtreeBranch()
{
	int i;
	for (i = 0; i < get_two_2_dim(); i++)
	{
		switch (types[i])
		{
		case QTREE_TYPE_BRANCH:
			delete ((QtreeBranch *) branches[i]);
			break;
		case QTREE_TYPE_LEAF:
			delete (QtreeLeaf *) branches[i];
			break;
		case QTREE_TYPE_NULL:
			break;
		default:
			puts("Error 195493955");
			exit(1);
		}
	}

	free(lb);
	free(ub);
	free(types);
	free(branches);
	free(branch);
}

void QtreeBranch::print(FILE* out, int depth)
{
	int i, j;

	for (j = 0; j < depth; j++)
	{
		fputc('\t', out);
	}
	fprintf(out, "branch d=%d: lb=", depth / 2);
	qtree_point_print(out, lb, get_dim(), false);
	fprintf(out, "; ub=");
	qtree_point_print(out, ub, get_dim(), false);
	fprintf(out, ";\n");


	for (i = 0; i < get_two_2_dim(); i++)
	{
		char *type = NULL;
		switch (types[i])
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

		for (j = 0; j < depth + 1; j++)
		{
			fputc('\t', out);
		}

		fprintf(out, "%d: %s\n", i, type);

		switch(types[i])
		{
		case QTREE_TYPE_BRANCH:
			((QtreeBranch *) branches[i])->print(out, depth + 2);
			break;
		case QTREE_TYPE_LEAF:
			((QtreeLeaf *) branches[i])->print(out, depth + 2);
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

}

void QtreeBranch::get_min(double* y_out, int dim_of_interest)
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		if (i & (1 << dim_of_interest))
		{
			continue;
		}

		qtree_type type = types[i];
		switch (type)
		{
		case QTREE_TYPE_BRANCH:
			((QtreeBranch *) branches[i])->get_min(y_out, dim_of_interest);
			break;
		case QTREE_TYPE_LEAF:
			((QtreeLeaf *) branches[i], y_out, dim_of_interest);
			break;
		case QTREE_TYPE_NULL:
			break;
		default:
			puts("Error 14721p854");
		}
	}

}

void QtreeBranch::apply(void (*fctn)(qtree_point* pnt, void* arg), void* arg)
{
	for (int i=0;i<two_2_dim;i++)
	{
		qtree_type type = types[i];
		switch(type)
		{
			case QTREE_TYPE_BRANCH:
				((QtreeBranch *) branches[i])->apply(fctn, arg);
				break;
			case QTREE_TYPE_LEAF:
				((QtreeLeaf *) branches[i])->apply(fctn, arg);
				break;
			case QTREE_TYPE_NULL:
				break;
			default:
				puts("Error 10656710874");
				exit(1);
		}
	}
}

int QtreeBranch::count()
{
	int count = 0;

	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_type type = types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			count += ((QtreeBranch *) branches[i])->count();
			break;
		case QTREE_TYPE_LEAF:
			count += ((QtreeLeaf *) branches[i])->size;
			break;
		case QTREE_TYPE_NULL:
			break;
		}
	}

	return count;
}

QtreeLeaf* QtreeBranch::get_leaf(int quadrant)
{
	int type = types[quadrant];
	if (type == QTREE_TYPE_LEAF)
	{
		return (QtreeLeaf *) branches[quadrant];
	}
	else if (type == QTREE_TYPE_NULL)
	{
		QtreeLeaf *new_leaf = new QtreeLeaf(branch, dim);
		types[quadrant] = QTREE_TYPE_LEAF;
		branches[quadrant] = new_leaf;

		return new_leaf;
	}
	else
	{
		puts("Error 204810463");
		exit(1);
	}
}

QtreeLeaf* QtreeBranch::find(qtree_point* point)
{
	int quadrant;
	qtree_type type = QTREE_TYPE_BRANCH;

	for(;;)
	{
		quadrant = qtree_get_quadrant(lb, ub, point, dim);
		type = ((QtreeBranch *) branch)->types[quadrant];

		if (type != QTREE_TYPE_BRANCH)
		{
			break;
		}

		branch = (QtreeBranch *) branches[quadrant];
	}

	return QtreeBranch_get_leaf(branch, quadrant, dim);
}

int QtreeBranch::get_parents_quad()
{
	QtreeBranch *parent = parent;
	if (parent == NULL)
	{
		puts("Error 15285298750");
		exit(1);
	}

	for (int i = 0; i < two_2_dim; i++)
	{
		if (parent->branches[i] == branch)
		{
			return i;
		}
	}
	return -587658765;
}

bool QtreeBranch::is_empty()
{
	for (int i = 0; i < get_two_2_dim(); i++)
	{
		qtree_type type = types[i];
		switch(type)
		{
		case QTREE_TYPE_BRANCH:
			if (!((QtreeBranch *) branches[i])->is_empty())
			{
				return false;
			}
			break;
		case QTREE_TYPE_LEAF:
			if (((QtreeLeaf *) branches[i])->size != 0)
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

}
