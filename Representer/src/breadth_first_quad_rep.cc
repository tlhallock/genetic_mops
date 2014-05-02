/*
 * breadth_first_quad_rep.cc
 *
 *  Created on: May 1, 2014
 *      Author: rever
 */

#include <stdlib.h>
#include "../../Rtree/src/Qtree.h"


typedef struct _list_node
{
	int count;
	qtree::QtreeLeaf *leaf;
	qtree::QtreeBranch *branch;
	struct _list_node *next;
} list_node;




void GeneticRepresenter::represent(InitialSet* set, int num_points,
		double (*represent_metric)(InitialSet *set, char *to_use, char *to_represent),
		char *mask_out)
{

	qtree::Qtree tree(dim);

	list_node *nodes = (list_node *) malloc (sizeof(*nodes) * num_points);

	list_node *last;
	list_node *first;
	list_node *empty;


	for (int i = 1; i < num_points; i++)
	{
		nodes[i - 1].next = &(nodes[i]);
	}
	nodes[num_points-1].next = NULL;

	int list_size = 0;

	int two_2_dim = (1 << 2);

	while(list_size < num_points)
	{
		if (is branch)
		{
			int count = 0;
			for (int i = 0; i < two_2_dim; i++)
			{
				switch (first->branch->types[i])
				{
				case qtree::QTREE_TYPE_LEAF:
					count += ((qtree::QtreeLeaf *) first->branch->branches[i])->size();
					break;
				case qtree::QTREE_TYPE_BRANCH:
					empty->branch = (qtree::QtreeBranch *) first->branch->branches[i];
					empty->next = NULL;
					empty->count = 1;

					last->next = empty;
					empty = empty->next;

					break;
				case qtree::QTREE_TYPE_NULL:
					// nothing to add in this case
					break;
				}
			}
		}
		else
		{
			first->count++;
			// some how update the count....

			last->next = first;
			last = first;
			first = first->next;
			last->next = NULL;
		}
		// look at the branch in first...
		// for all the sub branches that contain branches
		// {
		//        add those to the end of the list
		// }
	}


	free(nodes);
}
