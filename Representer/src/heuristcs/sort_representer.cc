/*
 * sort_representer.cc
 *
 *  Created on: May 5, 2014
 *      Author: thallock
 */

#include "../common.h"


static void sort(int *indices, InitialSet *set, int dimension_index)
{
	int size = set->size();
	if (size < 1)
	{
		return;
	}

	for (;;)
	{
		bool changed = false;

		for (int i = 1; i < size; i++)
		{
			if (set->get(indices[i])[dimension_index] <
					set->get(indices[i-1])[dimension_index])
			{
				int tmp = indices[i-1];
				indices[i-1] = indices[i];
				indices[i] = tmp;

				changed = true;
			}
		}

		if (!changed)
		{
			break;
		}
	}

//	printf("After sorting dimension %d:", dimension_index);
//
//	for (int i = 0; i < size; i++)
//	{
//		printf("%d\t", i);
//		for (int j = 0; j < set->get_dim(); j++)
//		{
//			printf("%lf ", set->get(indices[i])[j]);
//		}
//		fputc('\n', stdout);
//	}

}



void sort_represent(InitialSet *set, int num_points, char *mask_out)
{
	int dim = set->get_dim();
	int size = set->size();
	int num_per_dim = num_points / dim;

	int *indices = (int *) malloc(sizeof(*indices) * size);

	for (int i = 0; i < size; i++)
	{
		indices[i] = i;
		mask_out[i] = 0;
	}

	int count = 0;
	for (int i = 0; i < dim; i++)
	{
		sort(indices, set, i);
		for (int j = 0; j < num_per_dim; j++)
		{
			int index = (j * size) / num_per_dim;
			if (mask_out[index])
			{
				continue;
			}
			mask_out[index] = 1;
			count++;
		}
	}

	while(count < num_points)
	{
		int index = rand() % size;
		if (mask_out[index])
		{
			continue;
		}
		mask_out[index] = 1;
		count++;
	}

	free(indices);
}
