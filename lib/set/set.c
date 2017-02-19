#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

// Define struct setnode to have a key, data, and a next setnode
typedef struct setnode{
	char *key;
	void *data;
	struct setnode *next;
} setnode_t;

// Define struct set to have a first setnode and a function to delete sets
typedef struct set{
	struct setnode *first;
	void (*setDelete)(void *data);
} set_t;

static setnode_t *setnode_new(char *key, void *data);

/*
 * Function to create a new empty set data structure
 */
set_t *
set_new(void (*setDelete)(void *data)){
	set_t *set = malloc(sizeof(set_t));

	// Check if error allocating set
	if (set == NULL){
		return NULL;
	}
	else{
		// Initialize set contents
		set->first = NULL;
		set->setDelete = setDelete;
		return set;
	}
}

/*
 * Function to return data for a key and NULL otherwise
 */
void *
set_find(set_t *set, char *key){
	// Check through each node until there is no next node
	for (setnode_t *node = set->first; node != NULL; ){
		if (strcmp(key, node->key) == 0){
			return node->data; // Return the data if it is found
		}
		node = node->next;
	}
	return NULL;
}

/*
 * Function to return true if a new item was inserted and false otherwise
 */
bool set_insert(set_t *set, char *key, void *data)
{
	bool inserted = false; // function result
	bool exist = false; // whether the key exists

	if (set == NULL)
		return false; // bad set

	// check if the key already exists in the set
	for (setnode_t *node = set->first; node != NULL; node = node->next){
		if (strcmp(key, node->key) == 0) {
			exist = true;
			break;
		}
	}

	// insert new node at the head of set if it's a new key
	if (!exist) {
		setnode_t *new = setnode_new(key, data);
		if (new != NULL) {
			new->next = set->first;
			set->first = new;
			inserted = true;
		}
	}

	return inserted;
}

static setnode_t * // not visible outside this file
setnode_new(char *key, void *data)
{
	setnode_t *node = malloc(sizeof(setnode_t));

	if (node == NULL) {
		// error allocating memory for node; return error
		return NULL;
	} else {
		node->key = malloc(strlen(key)+1);
		if (node->key == NULL) {
			// error allocating memory for key;
			// cleanup and return error
			free(node);
			return NULL;
		} else {
			strcpy(node->key, key);
			node->data = data;
			node->next = NULL;
			return node;
		}
	}
}

/*
 * Function to delete the set and its contents
 */
void
set_delete(set_t *set)
{
	// Check if the set is NULL
	if (set == NULL){
		return;
	}
	// Delete the nodes with the specified delete function pointer
	for (setnode_t *node = set->first; node != NULL; ){
		if (set->setDelete != NULL){
			(*set->setDelete)(node->data);
		}
		setnode_t *next = node->next;
		free(node->key);
		free(node);
		node = next;
	}
	// Free the set
	free(set);
	return;
}

/* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void
set_iterate(set_t *set,
		  void (*itemfunc)(void *arg, const char *key, void *data),
		  void *arg)
{
	if (set == NULL || itemfunc == NULL){
		return;
	}
	for (setnode_t *node = set->first; node != NULL; node = node->next){
		(*itemfunc)(arg, node->key, node->data);
	}
}