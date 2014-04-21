/*
 * removing.cc
 *
 *  Created on: Apr 20, 2014
 *      Author: rever
 */

int qtree_get_quadrant(qtree_point *lb, qtree_point *ub, qtree_point *point, int dim)
{
//	fprintf(stdout, "the quadrand of ");
//	qtree_point_print(stdout, point, dim, true);
//	fprintf(stdout, " in lb=");
//	qtree_point_print(stdout, lb, dim, false);
//	fprintf(stdout, " in ub=");
//	qtree_point_print(stdout, ub, dim, true);
//	fprintf(stdout, " is ");

	int quad = 0;
	int i;
	for (i=0;i<dim;i++)
	{
		double mid = (ub[i] + lb[i]) / 2;
		if (point[i] > mid)
		{
			quad |= (1 << i);
		}

		if (ERROR_CHECKING)
		{
			if (point[i] < lb[i])
			{
				puts("Error 104801");
				exit (1);
			}
			if (point[i] > ub[i])
			{
				puts("Error 10472994");
				exit (1);
			}
		}
	}

//	fprintf(stdout, "%d\n", quad);

	return quad;
}

static void qtree_point_assign(qtree_leaf *leaf, qtree_point *point, int dim)
{
	int size = leaf->size++;
	for (int i = 0; i < dim; i++)
	{
		leaf->points[size][i] = point[i];
	}
}

void qtree_bounds_select(qtree_point *lb, qtree_point *ub, int quad, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		double mid = (lb[i] + ub[i]) / 2;

		if (quad & (1 << i))
		{
			lb[i] = mid;
		}
		else
		{
			ub[i] = mid;
		}
	}
}

}
