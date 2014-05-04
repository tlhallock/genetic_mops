/*
 * test.cc
 *
 *  Created on: May 3, 2014
 *      Author: rever
 */


void bread_first_represent(InitialSet *set, int num_points, char *mask_out);

void test_breadth_first()
{
	int dim = 2;
	int size = 100;
	int num_to_use = 20;

	char *mask = (char *) alloca(sizeof(*mask) * size);
	char *all = (char *) alloca(sizeof(*mask) * size);
	for (int i = 0; i < size; i++)
	{
		all[i] = 1;
		if (i < num_to_use)
		{
			mask[i] = 1;
		}
		else
		{
			mask[i] = 0;
		}
	}

	InitialSet set(size, dim, &l_2);

	bread_first_represent(&set, num_to_use, mask);

	const char *filename = "a-foo-a-foo-a-bar.m";
	fclose(fopen(filename, "w"));
	plot(filename, &set, mask, 0.0);
}





void test_metric()
{
	int dim = 2;
	int size = 50;
	int num_to_use = 10;

	char *mask = (char *) alloca(sizeof(*mask) * size);
	char *all = (char *) alloca(sizeof(*mask) * size);
	for (int i = 0; i < size; i++)
	{
		all[i] = 1;
		if (i < num_to_use)
		{
			mask[i] = 1;
		}
		else
		{
			mask[i] = 0;
		}
	}

	InitialSet set(size, dim, &l_2);

	const char * filename = "tester.m";

	fclose(fopen(filename, "w"));

	double max = -DBL_MAX;

	for (int i = 0; i < 100000; i++)
	{
		for (int j = 0; j < 2 * size; j++)
		{
			int i1 = rand() % size;
			int i2 = rand() % size;

			char tmp = mask[i1];
			mask[i1] = mask[i2];
			mask[i2] = tmp;
		}

		double fitness = represent_metric(&set, mask, all);
		if (fitness < max)
		{
			continue;
		}
		max = fitness;
//		i++;
	}
	plot(filename, &set, mask, max);
}




static void represent()
{
	srand(5000);

	int dim = 2;

	InitialSet set(50, 3, &l_2);

	GeneticRepresenter rep(50, 5);

	char *opt_mask = (char *) malloc (sizeof(*opt_mask) * set.size());

	rep.represent(&set, 10, &represent_metric, opt_mask);

	for (int i = 0; i < set.size(); i++)
	{
		if (opt_mask[i])
		{
			printf("++ ");
		} else
		{
			printf("-- ");
		}

		for (int j = 0; j < dim; j++)
		{
			printf("%lf ", set.get(i)[j]);
		}
		fputc('\n', stdout);
	}

	free(opt_mask);
}
