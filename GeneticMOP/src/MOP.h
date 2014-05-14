/*
 * MOP.h
 *
 *  
 *      Author: thallock
 */

#ifndef MOP_H_
#define MOP_H_

#include "image/ImageSet.h"

#include <stdio.h>

class BoundedMopStats
{
private:
	int xdim;
	int ydim;
	double *xbounds_min;
	double *xbounds_max;
	int points;
	ImageSet *image;
	void (*objective_function)(double *x, double *y_out);
	bool (*is_constrained)(double *x);
	FILE *log_file;
public:
	BoundedMopStats(int _xdim, int _ydim, double *_xmin, double *_xmax,
			ImageSet *_image,
			void (*_objective_function)(double *x, double *y_out),
			bool (*constaints)(double *x));

	virtual ~BoundedMopStats();

	int get_x_dimension() { return xdim; };
	int get_y_dimension() { return ydim; };
	int get_num_points() { return points ; }

	void make_guess(double x[], double y_out[]);

	void sample_bounds(double x_out[]);
	void sample_feasible(double x_out[]);

	void start_logging(FILE *_log_file);
	void stop_logging();

	bool is_feasible(double *x);

	double get_up_bound(int dim)
		{ return xbounds_max[dim]; }
	double get_lw_bound(int dim)
		{ return xbounds_min[dim]; }

	ImageSet *get_image();

	void reset();
};

#endif /* MOP_H_ */
