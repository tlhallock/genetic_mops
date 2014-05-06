/*
 * backtracker.cc
 *
 *  Created on: Apr 28, 2014
 *      Author: rever
 */

#include "backtracker.h"

#include "metrics/RepresentationMetric.h"

#include <float.h>
#include <stdlib.h>

namespace backtrack
{
class BacktrackObject
{
public:
	InitialSet *set;
	RepresentationMetric *metric;
	char *out_mask;
	double out_cost;
	char *tmp_mask;
	int to_use;
	int depth;
	double *costs;
	int current_count;

	BacktrackObject(RepresentationMetric *metric_, char *out_mask_, int to_use_) :
		set(metric_->get_set()),
		metric(metric_),
		out_mask(out_mask_),
		out_cost(-DBL_MAX),
		tmp_mask((char *) malloc (sizeof(*tmp_mask) * set->size())),
		to_use (to_use_),
		depth(0),
		costs((double *) malloc (sizeof (*costs) * set->size())),
		current_count(0)
	{
		for (int i = 0; i < set->size(); i++)
		{
			tmp_mask[i] = 0;
		}
	}

	~BacktrackObject()
	{
		free(tmp_mask);
		free(costs);
	}

	bool limit_on_count() { return to_use >= 0; }
};

void backtrack(BacktrackObject *bt)
{
	if (bt->limit_on_count())
	{
		if (bt->current_count > bt->to_use)
		{
			return;
		}
		if (bt->current_count < bt->set->size() - bt->depth)
		{
			return;
		}
	}

	if (bt->depth >= bt->set->size())
	{
		double fitness = bt->metric->get_fitness(bt->tmp_mask, bt->set->get_all_pnts(), bt->costs);
		if (fitness > bt->out_cost)
		{
			bt->out_cost = fitness;
			for (int i = 0; i < bt->set->size(); i++)
			{
				bt->out_mask[i] = bt->tmp_mask[i];
			}
		}
		return;
	}
	int index;

	index = bt->depth++;
	bt->tmp_mask[index] = 0;
	backtrack(bt);
	bt->depth--;

	index = bt->depth++;
	bt->tmp_mask[index] = 1;
	bt->current_count++;
	backtrack(bt);
	bt->current_count--;
	bt->depth--;
}

}

void back_track_to_find_optimal(RepresentationMetric *metric, char *out_mask, int to_use)
{
	backtrack::BacktrackObject bt(metric, out_mask, to_use);
	backtrack::backtrack(&bt);
	for (int i = 0; i < metric->get_set()->size(); i++)
	{
		out_mask[i] = bt.out_mask[i];
	}
}

void back_track_to_find_optimal(RepresentationMetric *metric, char *out_mask)
{
	back_track_to_find_optimal(metric, out_mask, BACKTRACKER_NUM_TO_USE_ANY);
}
