#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../set/set.h"
#include "hashtable.h"
#include "jhash.h"

// Define struct set to have set number of slots, a table of sets,
// and a function to delete the data in the hashtable
typedef struct hashtable{
	int slotNum;
	set_t **table;
	void (*htDelete)(void *data);
} hashtable_t;

/*
 * Function to create a new empty hashtable data structure
 */
hashtable_t *
hashtable_new(const int num_slots, void (*htDelete)(void *data)){
	hashtable_t *ht;
	ht = malloc(sizeof(hashtable_t));

	// Check if error allocating set
	if (ht == NULL){
		return NULL;
	}
	// Initialize 
	ht->slotNum = num_slots;

	ht->table = malloc(num_slots * sizeof(set_t *));

	// Check if the table was allocated properly
	if (ht->table == NULL){
		free(ht);
		return NULL;
	}

	// Check if all the sets were allocated properly
	for (int i = 0; i < num_slots; i++){
		set_t *s = set_new(htDelete);
		if (s == NULL){
			while (--i >= 0){
				set_delete(ht->table[i]);
			}
			free(ht->table);
			free(ht);
			return NULL;
		}

		ht->table[i] = s;
	}
	// Set the delete function
	ht->htDelete = htDelete;

	return ht;
}

/*
 * Function to return data for a key and NULL otherwise
 */
void *
hashtable_find(hashtable_t *ht, char *key)
{
	// Check if the hashtable is NULL
	if (ht == NULL){
		return NULL;
	}
	// Check which set to look in with hash functionn
	else{
		long index = JenkinsHash(key, ht->slotNum);
		return set_find(ht->table[index], key);
	}
	return NULL;
}

/*
 * Function to return true if a new item was inserted and false otherwise
 */
bool
hashtable_insert(hashtable_t *ht, char *key, void *data)
{
	// Check if the hashtable is NULL
	if (ht == NULL){
		return false;
	}
	// Find which set to insert into with the hash function
	long index = JenkinsHash(key, ht->slotNum);
	// Return whether it inserted or not
	bool inserted = set_insert(ht->table[index], key, data);

	return inserted;
}

/*
 * Function to delete the set and its contents
 */
void
hashtable_delete(hashtable_t *ht)
{
	// Check if the hashtable is NULL
	if (ht == NULL){
		return;
	}
	// Delete the set and its contents
	for (int i = 0; i < ht->slotNum; i++){
		set_delete(ht->table[i]);
	}
	free(ht->table);
	free(ht);
	return;
}

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void
hashtable_iterate(hashtable_t *ht, 
		       void (*itemfunc)(void *arg, const char *key, void *data),
		       void *arg)
{
	if (ht == NULL || itemfunc == NULL){
		return;
	}

	for (int i = 0; i < ht->slotNum; i++){
		set_iterate(ht->table[i], itemfunc, arg);
	}
}