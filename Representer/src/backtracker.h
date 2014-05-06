/*
 * backtracker.h
 *
 *  Created on: May 3, 2014
 *      Author: rever
 */

#ifndef BACKTRACKER_H_
#define BACKTRACKER_H_

#include "metrics/RepresentationMetric.h"

#define BACKTRACKER_NUM_TO_USE_ANY (-1)

void back_track_to_find_optimal(RepresentationMetric *metric, char *out_mask, int to_use);
void back_track_to_find_optimal(RepresentationMetric *metric, char *out_mask);


#endif /* BACKTRACKER_H_ */
