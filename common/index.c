/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * See index.h for interface descriptions.
 * 
 * David Oh, August 2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/hashtable/hashtable.h"
#include "../lib/counters/counters.h"
#include "index.h"
#include "web.h"
#include "webpage.h"
#include "word.h"

typedef hashtable_t index_t;

// Create a new index structure
index_t *
index_new(const int num_slots)
{
	return hashtable_new(num_slots, (void (*)(void *))counters_delete);
}

// Find the corresponding data from the index structure given a key
counters_t *
index_find(index_t *index, char *key)
{
	return hashtable_find(index, key);
}

// Try to insert key and data into index structure, return true if successful
bool
index_insert(index_t *index, char *key, counters_t *data)
{
	return hashtable_insert(index, key, data);
}

// Delete the index structure
void
index_delete(index_t *index)
{
	hashtable_delete(index);
}

// Build an index structure from a directory of webpage files
void index_build(char *dir, index_t *index)
{
	char *word;
	int pos;
	int docID = 1;
	WebPage *page;
	counters_t *word_count;

	// Load while there are still pages
	while ((page = webpage_load(dir, docID)) != NULL){
		pos = 0;
		// Get words one by one until there are no more
		while ((pos = GetNextWord(page->html, pos, &word)) > 0){
			// Validate the word
			if (strlen(word) < 3){
				free(word);
				continue;
			}
			word = NormalizeWord(word);

			// Check if the word exists in the hashtable
			word_count = index_find(index, word);
			if (word_count == NULL){
				// Create a new counter if the word was not already added
				word_count = counters_new();

				// Insert the new word/counter pair into the hashtable
				index_insert(index, word, word_count);

			}

			// Increment the counter
			counters_add(word_count, docID);
			free(word);
		}

		docID++;
		webpage_delete(page);
	}
}

// Print the contents of the counter
void print_count(void *fp, int key, int count)
{
	fprintf((FILE*) fp, "%d %d ", key, count);
}

// Print the contents of the set in the index
void print_index(void *fp, const char *key, void *counter)
{
	fprintf((FILE*) fp, "%s ", key);
	counters_iterate((counters_t*)counter, print_count, (FILE*) fp);
	fprintf((FILE*) fp, "\n");
}

// Write the contents of the index to a file
void index_save(FILE *fp, index_t *index)
{
	hashtable_iterate(index, print_index, fp);
}

// Load an index structure from a file
void index_load(FILE *fp, index_t *index)
{
	char line[1000000];
	char key[1000];
	char docID_count_str[30];
	int docID = 0;
	int count = 0;
	int length;
	counters_t *word_count;

	// Load the file line by line
	while (fgets(line, 1000000, fp) != NULL){
		if (sscanf(line, "%s ", key) != 1){
			continue;
		}
		length = strlen(key);
		word_count = index_find(index, key);

		// Check if the corresponding counter already exists
		if (word_count == NULL){

			// Create a new counter if the word was not already added
			word_count = counters_new();

			// Insert the new word/counter pair into the hashtable
			index_insert(index, key, word_count);

		}
		char* temp = malloc(strlen(line)+1);
		strcpy(temp, line + length);

		// Extract the document ID and frequency
		while (sscanf(temp, "%d %d ", &docID, &count) == 2){
			sprintf(docID_count_str, "%d %d ", docID, count);
			length = length + strlen(docID_count_str);
			strcpy(temp, line + length);
			counters_set(word_count, docID, count);
		}

		// Clean up
		free(temp);
		memset(line,'\0',1000000);
	}
}
