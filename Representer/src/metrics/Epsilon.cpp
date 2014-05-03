/*
 * Epsilon.cpp
 *
 *  Created on: May 3, 2014
 *      Author: thallock
 */

#include "Epsilon.h"

Epsilon::Epsilon(InitialSet *set, int n_) :
	RepresentationMetric(set),
	num_closest(n_)
{
}


double Epsilon::get_fitness(char *mask, char *to_represent, double *costs)
{

}
