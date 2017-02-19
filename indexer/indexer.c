/*
 * indexer.c - Indexes the words from files created by crawler
 *           - Stores the indexes of the words in a new file
 *
 * David Oh
 *
 * COSC 50, August 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable/hashtable.h"
#include "counters/counters.h"
#include "word.h"
#include "webpage.h"
#include "index.h"

// Function to check if the directory to read from is valid
// Returns true if the directory contains the output from crawler
bool IsCrawlerDirectory(char *dir)
{
	FILE *fp = NULL;
	char *file = malloc(strlen(dir) + strlen("/.crawler") + 1);

	// Check if the file name was properly allocated
	if (file == NULL){
		printf("Error: the name for the pageDir was not properly allocated\n");
		return false;
	}

	strcpy(file, dir);
	strncat(file, "/.crawler", 10);

	fp = fopen(file, "r");

	// Check if the file ".crawler" was found
	if (fp == NULL){
		printf("Please provide a valid existing and/or readable pageDir\n");
		free(file);
		return false;
	}
	fclose(fp);
	free(file);
	return true;
}

// Function to index the words from the files created by the crawler
// Returns integer corresponding to exit code
int main(int argc, char *argv[])
{
	FILE *fp;
	char *dir;
	index_t *wordTable;

	// Check that the number of arguments is correct
	if (argc != 3){
		printf("%s requires two arguments, a pageDirectory and indexFilename\n", argv[0]);
		return 1;
	}

	// Check if the provided file is valid
	fp = fopen(argv[2], "w");
	if (fp == NULL){
		printf("Please provide a valid/writable index file\n");
		return 1;
	}

	// Check if the provided directory contains the output from crawler
	if (!IsCrawlerDirectory(argv[1])){
		fclose(fp);
		return 1;
	}
	dir = malloc(strlen(argv[1]) + 1);
	if (dir == NULL){
		printf("Error: the name for the testDir was not properly allocated\n");
		fclose(fp);
		return 1;
	}
	strcpy(dir, argv[1]);

	// Create a new index data structure
	wordTable = index_new(10000);
	if (wordTable == NULL){
		printf("Error: the index was not properly allocated\n");
		free(dir);
		fclose(fp);
		return 2;
	}
	index_build(dir, wordTable);
	index_save(fp, wordTable);

	// Clean up
	free(dir);
	fclose(fp);
	index_delete(wordTable);
	return 0;
}