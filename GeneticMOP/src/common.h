/* 
 * File:   common.h
 * Author: thallock
 *
 * Created on May 11, 2014, 3:56 PM
 */

#ifndef GA_MOP_COMMON_H
#define	GA_MOP_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>

#include "../../Rtree/src/Qtree.h"

#include "utils.h"
#include "math_functions.h"
#include "MOP.h"
#include "image/ImageSet.h"
#include "image/RtreeImageSet.h"
#include "image/VectorImageSet.h"
#include "solver/IncrementalSampleSolver.h"
#include "solver/Solver.h"
#include "solver/GeneticSolver.h"
#include "solver/RandomSampleSolver.h"
#include "solver/DepthFirstRecursiveStencil.h"


#define NUM_GUESSES 500


void test_nearest();
void test_pareto_equivalence();
void compare_solvers();


#endif	/* COMMON_H */

