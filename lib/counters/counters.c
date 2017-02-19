#include <stdio.h>
#include <stdlib.h>
#include "counters.h"

// Define struct counters_node to have key, counter, and a next counters_node
typedef struct counters_node{
	int key;
	int counter;
	struct counters_node *next;
} counters_node_t;

// Define struct counters to have a first counters_node
typedef struct counters{
	struct counters_node *first;
} counters_t;

static counters_node_t *countersnode_new(int key);

/*
 * Function to create a new counters data structure
 */
counters_t *
counters_new(void)
{
	counters_t *ctrs = malloc(sizeof(counters_t));

	if (ctrs == NULL){
		return NULL;
	}
	else{
		ctrs->first = NULL;
		return ctrs;
	}
}

/*
 * Function to increment the counter for a given key
 * Will intialize the counter at 1 if the key does not yet exist
 */
void
counters_add(counters_t *ctrs, int key)
{
	if (ctrs == NULL){
		return;
	}
	if (ctrs->first == NULL){
		ctrs->first = countersnode_new(key);
	}
	counters_node_t *prev = ctrs->first;
	for (counters_node_t *node = prev; node != NULL; node = prev->next){
		// Increment the counter if the key exists
		if (node->key == key){
			node->counter++;
			return;
		}
		prev = node;
	}
	// Create the counters_node for the new counter
	prev->next = countersnode_new(key);
	return;
}

/* Allocate and initialize a countersnode */
static counters_node_t * // not visible outside this file
countersnode_new(int key)
{
	counters_node_t *node = malloc(sizeof(counters_node_t));

	if (node == NULL) {
		// error allocating memory for node; return error
		return NULL;
	}
	else {
		node->key = key;
		node->counter = 1;
		node->next = NULL;
		return node;
	}
}

/*
 * Function to return the count of the counter for a given key
 * Will give 0 if the key does not exist
 */
int
counters_get(counters_t *ctrs, int key)
{
	if (ctrs == NULL){
		return 0;
	}
	for (counters_node_t *node = ctrs->first; node != NULL; ){
		// Check if the key exists
		if (node->key == key){
			return node->counter;
		}
		node = node->next;
	}
	return 0;
}

/*
 * Function to delete counters
 */
void
counters_delete(counters_t *ctrs)
{
	if (ctrs == NULL){
		return;
	}
	// Free each counters_node
	for (counters_node_t *node = ctrs->first; node != NULL; ){
		counters_node_t *next = node->next;
		free(node);
		node = next;
	}
	free(ctrs);
}

/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 */
void
counters_set(counters_t *ctrs, int key, int count)
{
	if (ctrs == NULL){
		return;
	}
	if (ctrs->first == NULL){
		ctrs->first = countersnode_new(key);
		ctrs->first->counter = count;
	}
	counters_node_t *prev = ctrs->first;
	for (counters_node_t *node = prev; node != NULL; node = prev->next){
		// Increment the counter if the key exists
		if (node->key == key){
			node->counter = count;
			return;
		}
		prev = node;
	}
	// Create the counters_node for the new counter
	prev->next = countersnode_new(key);
	prev->next->counter = count;
	return;
}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void
counters_iterate(counters_t *ctrs,
		      void (*itemfunc)(void *arg, const int key, int count),
		      void *arg)
{
	if (ctrs == NULL || itemfunc == NULL){
		return;
	}
	else {
		for (counters_node_t *ctr = ctrs->first; ctr != NULL; ctr = ctr->next){
			(*itemfunc)(arg, ctr->key, ctr->counter);
		}
	}
}