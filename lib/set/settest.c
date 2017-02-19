#include <stdio.h>
#include <stdlib.h>
#include "set.h"

/*
 * Testing the functionality of the set data structures
 */
int main()
{
	set_t *set;

	set = set_new(free);

	// Check if set was properly allocated
	if (set == NULL){
		printf("set_new failed.\n");
		exit(1);
	}

	// Insert data for a key "firstkey"
	set_insert(set, "firstkey", "firstdata");

	// Insert should give data of "firstdata" or NULL if improperly allocated
	printf("Key of firstkey gave %s\n", (char *)set_find(set, "firstkey"));

	// Try inserting new data for a key "firstkey"
	set_insert(set, "firstkey", "firstdata_2");

	// Second insert should have failed
	printf("Key of firstkey gave %s\n", (char *)set_find(set, "firstkey"));

	set_delete(set);

	// Create a new set
	set = set_new(free);

	// Check if set was properly allocated
	if (set == NULL){
		printf("set_new failed.\n");
		exit(1);
	}

	// Insert data for a key "firstkey"
	set_insert(set, "firstkey", "i am from first key");

	// Insert should give data of "i am from first key" or NULL if improperly allocated
	printf("Key of firstkey gave %s\n", (char *)set_find(set, "firstkey"));

	// Insert data for a key "secondkey"
	set_insert(set, "secondkey", "i am from second key");

	// Insert should give data of "i am from second key" or NULL if improperly allocated
	printf("Key of secondkey gave %s\n", (char *)set_find(set, "secondkey"));

	// Insert data for a key "thirdkey"
	set_insert(set, "thirdkey", "i am from third key");

	// Insert should give data of "i am from third key" or NULL if improperly allocated
	printf("Key of thirdkey gave %s\n", (char *)set_find(set, "thirdkey"));

	// Delete the set
	set_delete(set);
}