/*
 * DepthFirstRecursiveStencil.cpp
 *
 *  
 *      Author: rever
 */

#include "DepthFirstRecursiveStencil.h"

#include <stdlib.h>

#define MAX_QUAD 4
#define Y_DIM 2
#define X_DIM 2


static FILE *svg_file = NULL;
static const char *svg_name = "recursion.svg";

static void set_bound(double *x_lb, double *x_up, double *next_point, int dim, int quad)
{
	for(int i=0;i<dim;i++)
	{
		if (quad & (1 << i))
		{
			next_point[i] = x_up[i];
		}
		else
		{
			next_point[i] = x_lb[i];
		}
	}
}

static void set_quad(double *x_lb, double *x_up, double *next_lb, double *next_up, int xdim, int quad)
{
	for(int i=0; i<xdim; i++)
	{
		double mid = (x_lb[i] + x_up[i]) / 2;
		if (quad & (1 << i))
		{
			next_lb[i] = mid;
			next_up[i] = x_up[i];
		}
		else
		{
			next_lb[i] = x_lb[i];
			next_up[i] = mid;
		}
	}
}

void DepthFirstRecursiveStencil::back_track(double *x_lb, double *x_up, int depth)
{
	if (depth == max_depth_feasible)
	{
		FILE *debug = fopen("debug_dfrs.txt", "a");
		for (int t = 0; t < depth; t++) fputc('\t', debug);
		fprintf(debug, "max feasible depth hit\n");
		fclose(debug);
		return;
	}

	double svgx = 1000 * x_lb[0];
	double svgy = 1000 * x_lb[1];
	double svgw = 1000 * (x_up[0]-x_lb[0]);
	double svgh = 1000 * (x_up[1]-x_lb[1]);

	// map from normal cooridates to svg coords
	svgy = 1000 - svgy - svgh;

	fprintf(svg_file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"stroke:red;stroke-width:5\" />\n",
			svgx, svgy, svgw, svgh);

	FILE *debug = fopen("debug_dfrs.txt", "a");

	for (int t=0;t<depth;t++) fputc('\t', debug);
	fprintf(debug, "lower bounds = ");
	for (int i=0; i<stats->get_x_dimension(); i++)
	{
		fprintf(debug, "%lf ", x_lb[i]);
	}
	fputc('\n', debug);

	for (int t=0;t<depth;t++) fputc('\t', debug);
	fprintf(debug, "upper bounds = ");
	for (int i=0; i<stats->get_x_dimension(); i++)
	{
		fprintf(debug, "%lf ", x_up[i]);
	}
	fputc('\n', debug);
	fclose(debug);

	{
		bool satisfies_epsilon = true;

		double xs[MAX_QUAD * X_DIM];
		double ys[MAX_QUAD * Y_DIM];

		int count = 0;
		for (int i = 0; i < MAX_QUAD; i++)
		{
			set_bound(x_lb, x_up, &xs[i * stats->get_x_dimension()], stats->get_x_dimension(), i);
			if (!stats->is_feasible(&xs[i * stats->get_x_dimension()]))
			{
				continue;
			}

			stats->make_guess(&xs[i * stats->get_x_dimension()], &ys[i * stats->get_y_dimension()]);
			if (count < 2)
			{
				count++;
			}
		}

		if (count == 0 && depth >= max_depth_infeasible)
		{
			FILE *debug = fopen("debug_dfrs.txt", "a");
			for (int t=0;t<depth;t++) fputc('\t', debug);
			fprintf(debug, "hit infeasible depth=%d\n", depth);
			fclose(debug);
			return;
		}

		debug = fopen("debug_dfrs.txt", "a");
		for (int i = 0; i < MAX_QUAD && satisfies_epsilon; i++)
		{
			if (!stats->is_feasible(&xs[i * stats->get_x_dimension()]))
			{
				continue;
			}


			for (int j = 0; j < MAX_QUAD && satisfies_epsilon; j++)
			{
				if (i == j)
				{
					continue;
				}

				if (!stats->is_feasible(&xs[j * stats->get_x_dimension()]))
				{
					continue;
				}

				for (int t=0;t<depth;t++) fputc('\t', debug);
				fprintf(debug, "comparing: \n");
				for (int t=0;t<depth;t++) fputc('\t', debug);
				print_point(debug, &ys[i * stats->get_y_dimension()], stats->get_y_dimension(), true);
				for (int t=0;t<depth;t++) fputc('\t', debug);
				print_point(debug, &ys[j * stats->get_y_dimension()], stats->get_y_dimension(), true);

				if (!stats->is_feasible(&xs[j * stats->get_x_dimension()]))
				{
					for (int t=0;t<depth;t++) fputc('\t', debug);
					fprintf(debug, "not feasible\n");
					continue;
				}

				double dist = norm(&ys[i * stats->get_y_dimension()], &ys[j * stats->get_y_dimension()],
						stats->get_y_dimension());

				for (int t=0;t<depth;t++) fputc('\t', debug);
				fprintf(debug, "norm = %lf\n", dist);

				if (dist > epsilon)
				{
					satisfies_epsilon = false;
				}
			}
		}
		fclose(debug);

		if (satisfies_epsilon && count >= 2)
		{
			FILE *debug = fopen("debug_dfrs.txt", "a");
			for (int t=0;t<depth;t++) fputc('\t', debug);
			fprintf(debug, "satisfied depth=%d\n", depth);
			fclose(debug);
			return;
		}
	}

	double *next_lb = (double *) malloc (sizeof (*next_lb) * stats->get_x_dimension());
	double *next_up = (double *) malloc (sizeof (*next_lb) * stats->get_y_dimension());

	for (int i=0; i<MAX_QUAD; i++)
	{
		FILE *debug = fopen("debug_dfrs.txt", "a");
		for (int t=0;t<depth;t++) fputc('\t', debug);
		fprintf(debug, "trying quad %d\n", i);
		fclose(debug);

		set_quad(x_lb, x_up, next_lb, next_up, stats->get_x_dimension(), i);
		back_track(next_lb, next_up, depth + 1);
	}

	free(next_lb);
	free(next_up);
}

void DepthFirstRecursiveStencil::solve(BoundedMopStats *board)
{
	stats = board;

	if (max_depth_feasible < max_depth_infeasible)
	{
		printf("not intended: max depth feasible(%d) < max depth infeasible(%d)\n", max_depth_feasible, max_depth_infeasible);
	}

	svg_file = fopen(svg_name, "w");
	fprintf(svg_file, "<svg width=\"1000\" height=\"1000\">\n");

	double *x_lb = (double *) malloc (sizeof (*x_lb) * stats->get_x_dimension());
	double *x_up = (double *) malloc (sizeof (*x_up) * stats->get_x_dimension());

	for (int i=0; i<stats->get_x_dimension(); i++)
	{
		x_lb[i] = stats->get_lw_bound(i);
		x_up[i] = stats->get_up_bound(i);
	}

	back_track(x_lb, x_up, 0);

	free(x_lb);
	free(x_up);

	fprintf(svg_file, "</svg>\n");
	fclose(svg_file);
}
