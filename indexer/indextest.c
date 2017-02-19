/* 
 * indextest - load an index, and save it, to test those functions
 *
 * 
 * David Oh, August 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "file.h"
#include "counters/counters.h"

// Function to create index from file produced by indexer
// Returns integer corresponding to exit code
int main(int argc, char *argv[])
{
	FILE *old;
	FILE *new;
	index_t *index;

	// Check that the number of arguments is correct
	if (argc != 3){
		printf("%s requires two arguments, an oldIndexFilename and newIndexFilename\n", argv[0]);
		return 1;
	}

	// Check that the old file to read from is valid
	old = fopen(argv[1], "r");
	if (old == NULL){
		printf("Please provide a valid/readable old index file\n");
		return 1;
	}

	// Check that the new file to write into is valid
	new = fopen(argv[2], "w");
	if (new == NULL){
		fclose(old);
		printf("Please provide a valid/writable new index file\n");
		return 1;
	}

	// Create a new index data structure
	index = index_new(10000);

	index_load(old, index);
	index_save(new, index);

	// Clean up
	index_delete(index);
	fclose(old);
	fclose(new);
}