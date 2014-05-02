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
		FILE *out = fopen(filename, "a");

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

		fprintf(out, "scatter(x(:,1), x(:,2), 10);\n");
		fprintf(out, "hold on\n");
		fprintf(out, "scatter(s(:,1), s(:,2), 30, 'r');\n");
		fprintf(out, "drawnow\n");
	//	fprintf(out, "input(\"enter something...\")\n");
		fprintf(out, "hold off\n");
		fprintf(out, "fitness = %lf\n", fitness);
		fprintf(out, "pause(1)\n");

		fclose(out);
}
