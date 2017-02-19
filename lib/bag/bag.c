#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

// Define struct bagnode to have data and a next bagnode
typedef struct bagnode{
	void *data;
	struct bagnode *next;
} bagnode_t;

// Define struct bag to have a first bagnode and a function to delete sets
typedef struct bag{
	struct bagnode *head;
	void (*bagnodeDelete)(void *data);
} bag_t;

static bagnode_t *bagnode_new(void *data);

/*
 * Function to create a new empty bag data structure
 */
bag_t *
bag_new(void (*bagnodeDelete)(void *data))
{
	bag_t *bag = malloc(sizeof(bag_t));

	// Check if error allocating bag
	if (bag == NULL){
		return NULL;
	}
	else {
		// Initialize bag contents
		bag->head = NULL;
		bag->bagnodeDelete = bagnodeDelete;
		return bag;
	}
}

/*
 * Function to insert data if the bag is not NULL
 */
void
bag_insert(bag_t *bag, void *data)
{
	if (bag != NULL){
		bagnode_t *new_node = bagnode_new(data);
		new_node->next = bag->head;
		bag->head = new_node;
	}
}

/*
 * Function to return data from the bag if the bag and item in bag are not NULL
 */
void *
bag_extract(bag_t *bag)
{
	if (bag == NULL){
		return NULL;
	}
	else {
		// Check if item in bag is NULL
		if (bag->head == NULL){
			return NULL;
		}
		else{
			// Shift items in bag
			bagnode_t *extract = bag->head;
			void *data = extract->data;
			bag->head = extract->next;
			free(extract);			// free the memory for the bag
			return data;			// return the data of the item
		}
	}
}

/*
 * Function to delete the bag and its contents
 */
void
bag_delete(bag_t *bag)
{
	if (bag == NULL){
		return;
	}
	if (bag->bagnodeDelete != NULL){
		// Shift through bag and delete sequentially
		for (bagnode_t *node = bag->head; node != NULL; ){
			bagnode_t *next = node->next;
			(*bag->bagnodeDelete)(node->data);
			free(node);
			node = next;
		}
		free(bag);
	}
	return;
}

/*
 * Function to create a new bagnode
 */
static bagnode_t *
bagnode_new(void *data)
{
	bagnode_t *node = malloc(sizeof(bagnode_t));

	// Check if allocation was successful
	if (node == NULL){
		return NULL;
	}
	else{
		node->data = data;
		return node;
	}
}

/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, data).
 */
void
bag_iterate(bag_t *bag,
            void (*itemfunc)(void *arg, void *data),
            void *arg)
{
  if (bag == NULL || itemfunc == NULL) {
    return; // bad bag or null function
  } else {
    // scan the bag
    for (bagnode_t *node = bag->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->data); 
    }
  }
  return;
}