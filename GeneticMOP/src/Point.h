/*
 * Point.h
 *
 *  Created on: Apr 6, 2014
 *      Author: thallock
 */

#ifndef POINT_H_
#define POINT_H_

class Point
{
private:
	double vals[];
public:
	Point(double _vals[]) :
		vals(_vals) {};

	virtual ~Point(){}

	void set(double _vals[])
	{

	}
};

#endif /* POINT_H_ */
