//============================================================================
// Name        : GeneticMOP.cpp
// Author      : rever
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include<limits.h>

#include <stdio.h>

#include "math_functions.h"

#include "VectorImageSet.h"
#include "GeneticSolver.h"
#include "RandomSampleSolver.h"
#include "IncrementalSampleSolver.h"
#include "DepthFirstRecursiveStencil.h"



double get_index(double r)
{
	return r * r * r * r * r * r * r;
}



int main()
{
	long seed = time(NULL);
	srand(seed);

//	test_pareto_equivalence();

	test_nearest();
	//compare_solvers();

	return 0;
}


