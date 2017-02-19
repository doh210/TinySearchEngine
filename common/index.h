/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * The index is keyed by words (strings) and stores counter sets; 
 * each counter set represents the set of documents where that word 
 * appeared, and the number of occurrences of that word in each document.
 * 
 * David Oh, Summer 2016
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include "../lib/hashtable/hashtable.h"
#include "../lib/counters/counters.h"

/**************** global types ****************/

// The index is actually a hashtable. 
typedef hashtable_t index_t;

/********** add function prototypes here *********/
index_t *index_new(const int num_slots);

counters_t *index_find(index_t *index, char *key);

bool index_insert(index_t *index, char *key, counters_t *data);

void index_delete(index_t *index);

void index_build(char *dir, index_t *index);

void index_save(FILE *fp, index_t *index);

void index_load(FILE *fp, index_t *index);

#endif // __INDEX_H
