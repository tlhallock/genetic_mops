/*
 * memory_manager.c
 *
 *  Created on: Apr 17, 2014
 *      Author: thallock
 */

#include <stdlib.h>

void *allocate(size_t size)
{
	void *ptr = malloc (size);
//	printf("allocated %p\n", ptr);
	return ptr;
}

void destroy(void *ptr)
{
//	printf("freeing %p\n", ptr);
	free(ptr);
}
