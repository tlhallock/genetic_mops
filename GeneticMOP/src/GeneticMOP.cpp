//============================================================================
// Name        : GeneticMOP.cpp
// Author      : rever
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;

#include "common.h"

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


