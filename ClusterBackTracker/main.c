/*
 * main.c
 *
 *  Created on: Apr 16, 2014
 *      Author: rever
 */

#include "pareto_header.h"

#include <float.h>



int main(int argc, char **argv)
{
	FILE *in = fopen("input.txt", "r");

	pareto_set *set = pareto_set_read(in);

	pareto_set_backtrack(set);

	pareto_set_del(set);
}

