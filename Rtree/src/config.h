/*
 * config.h
 *
 *  Created on: Apr 3, 2014
 *      Author: rever
 */

#ifndef CONFIG_H_
#define CONFIG_H_


#define DIMENSION 4

#define TOL .00000001

#define IS_ZERO(x)  ((x) >= 0 ? ((x) < TOL) : ((x) > TOL))


#define ERROR_CHECKING 1

#endif /* CONFIG_H_ */
