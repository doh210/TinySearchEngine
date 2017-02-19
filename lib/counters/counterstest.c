#include <stdio.h>
#include <stdlib.h>
#include "counters.h"

/*
 * Testing the functionality of the counters data structures
 */
int main()
{
	counters_t *counter;

	// Create a new counter
	counter = counters_new();
	if (counter == NULL){
		printf("counters_new failed\n");
		exit(1);
	}

	// Give the initial count of the counter
	printf("key of 13 gave %d\n", counters_get(counter, 13));

	// Increment once
	counters_add(counter, 13);

	// Give the value of the counter
	printf("key of 13 gave %d\n", counters_get(counter, 13));

	// Increment the counter with a key of 13
	counters_add(counter, 13);
	counters_add(counter, 13);
	counters_add(counter, 13);
	counters_add(counter, 13);
	counters_add(counter, 13);

	// Give the value of the counter
	printf("key of 13 gave %d\n", counters_get(counter, 13));

	// Try adding a new counter
	counters_add(counter, 6);

	// Give the value of the new counter
	printf("key of 6 gave %d\n", counters_get(counter, 6));

	counters_delete(counter);
}
