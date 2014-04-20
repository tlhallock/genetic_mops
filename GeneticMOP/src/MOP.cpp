/*
 * MOP.cpp
 *
 *  
 *      Author: thallock
 */

#include "MOP.h"

#include <stdlib.h>
#include <limits.h>

void BoundedMopStats::make_guess(double *x, double *y_out)
{
	points++;
	objective_function(x,y_out);

	if (!is_feasible(x))
	{
		return;
	}

	image->add_point(y_out);

	if (log_file != NULL)
	{
		log_guess(log_file, x, xdim, y_out, ydim);
	}
}

void BoundedMopStats::sample_bounds(double *x_out)
{
	for (int i = 0; i < xdim; i++)
	{
		x_out[i] = xbounds_min[i] + (rand() / (double) INT_MAX) * (xbounds_max[i] - xbounds_min[i]);
	}
}

void BoundedMopStats::sample_feasible(double x_out[])
{
	do
	{
		sample_bounds(x_out);
	} while (!is_feasible(x_out));
}


ImageSet *BoundedMopStats::get_image()
{
	return image;
}

BoundedMopStats::~BoundedMopStats()
{
	stop_logging();
	free(xbounds_min);
	free(xbounds_max);
}


void BoundedMopStats::start_logging(FILE *_log_file)
{
	if (log_file != NULL)
	{
		fclose(log_file);
	}
	log_file = _log_file;
}
void BoundedMopStats::stop_logging()
{
	if (log_file != NULL)
	{
		fclose(log_file);
		log_file = NULL;
	}
}

void BoundedMopStats::reset()
{
	points = 0;
	image->clear();
}

bool BoundedMopStats::is_feasible(double *x)
{
	for (int i = 0; i < xdim; i++)
	{
		if (x[i] < xbounds_min[i] || x[i] > xbounds_max[i])
		{
			return false;
		}
	}
	return is_constrained(x);
}









