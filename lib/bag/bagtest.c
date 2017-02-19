#include <stdio.h>
#include <stdlib.h>
#include "bag.h"

/*
 * Testing the functionality of the bag data structures
 */
int main()
{
	bag_t *bag;

	bag = bag_new(free);
	if (bag == NULL){
		printf("bag_new failed\n");
		exit(1);
	}

	// Try extracting before any data is added
	printf("initial extract returns %s\n", (char*) bag_extract(bag));

	// Insert into the bag
	bag_insert(bag, "1");
	bag_insert(bag, "2");
	bag_insert(bag, "3");
	bag_insert(bag, "4");

	// Should retrieve the most recent item in the bag
	printf("first extract returns %s\n", (char*) bag_extract(bag));

	// Delete the bag
	bag_delete(bag);

	// Make a new bag
	bag = bag_new(free);
	if (bag == NULL){
		printf("bag_new failed\n");
		exit(1);
	}

	// Insert again into the bag
	bag_insert(bag, "1");
	bag_insert(bag, "2");
	bag_insert(bag, "3");
	bag_insert(bag, "4");

	// Extract until every item is extracted
	printf("second extract returns %s\n", (char*) bag_extract(bag));
	printf("third extract returns %s\n", (char*) bag_extract(bag));
	printf("fourth extract returns %s\n", (char*) bag_extract(bag));
	printf("fifth extract returns %s\n", (char*) bag_extract(bag));
	printf("sixth extract returns %s\n", (char*) bag_extract(bag));

	bag_delete(bag);
}