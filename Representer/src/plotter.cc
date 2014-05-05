/*
 * plotter.cc
 *
 *  Created on: Apr 30, 2014
 *      Author: rever
 */

#include "InitialSet.h"

void plot(const char *filename, InitialSet *set, char *mask,
//		double (*represent_metric)(InitialSet *set, char *to_use, char *to_represent),
		double fitness)
{
		FILE *out = fopen(filename, "w");

		fprintf(out, "graphics_toolkit gnuplot\n");
		fprintf(out, "x = [\n");
		for (int i = 0; i < set->size(); i++)
		{
			if (mask[i])
			{
				continue;
			}

			for (int j = 0; j < set->get_dim(); j++)
			{
				fprintf(out, "\t%lf, ", set->get(i)[j]);
			}

			fprintf(out, " ; ...\n");
		}
		fprintf(out, "]; s = [\n");
		for (int i = 0; i < set->size(); i++)
		{
			if (!mask[i])
			{
				continue;
			}

			for (int j = 0; j < set->get_dim(); j++)
			{
				fprintf(out, "\t%lf, ", set->get(i)[j]);
			}

			fprintf(out, " ; ...\n");
		}
		fprintf(out, "];\n");

		fprintf(out, "scatter(x(:,1), x(:,2)%s);\n", true ? ", 10" : ", 's'");
		fprintf(out, "hold on\n");
		fprintf(out, "scatter(s(:,1), s(:,2)%s);\n", true ? ", 30" : ", 'x'");
		fprintf(out, "drawnow\n");
	//	fprintf(out, "input(\"enter something...\")\n");
		fprintf(out, "hold off\n");
		fprintf(out, "fitness = %lf\n", fitness);
		fprintf(out, "input('enter something to continue');\n");

		fclose(out);
}

void plot_initial_set(const char *filename, InitialSet *set)
{
	FILE *file = fopen(filename, "w");

	fprintf(file, "graphics_toolkit gnuplot\n");
	fprintf(file, "xs = [...\n");

	int length = set->size();
	int dim = set->get_dim();

	for (int i = 0; i < length; i++)
	{
		fprintf(file, "\t");
		for (int j = 0; j < dim; j++)
		{
			fprintf(file, "%lf ", set->get(i)[j]);
		}
		fprintf(file, "; ...\n");
	}

	fprintf(file, "];\n");
	fprintf(file, "scatter(xs(:, 1), xs(:, 2));\n");
	fprintf(file, "drawnow;\n");
	fprintf(file, "input(\"Press any key to continue\");\n");

	fclose(file);
}
