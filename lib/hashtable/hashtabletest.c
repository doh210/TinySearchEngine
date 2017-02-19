#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "../set/set.h"
#include "jhash.h"

/*
 * Testing the functionality of the hashtable data structures
 */
int main()
{
	hashtable_t *hashtable;

	// Create a new hashtable
	hashtable = hashtable_new(6, free);
	if (hashtable == NULL){
		printf("hashtable_new failed\n");
		exit(1);
	}

	// Insert into the hashtable
	hashtable_insert(hashtable, "key1", "firstdata");

	// Try writing data into a slot where it already exists
	hashtable_insert(hashtable, "key1", "seconddata");

	// Gives the data from the key
	printf("Key of key1 gave %s\n", (char *)hashtable_find(hashtable, "key1"));

	// Gives the index from the hash function
	printf("It was indexed at %d\n", (int) JenkinsHash("key1", 6));

	// Delete the hashtable
	hashtable_delete(hashtable);

	// Create a new hashtable
	hashtable = hashtable_new(3, free);
	if (hashtable == NULL){
		printf("hashtable_new failed\n");
		exit(1);
	}

	// Check find for a non-existent key
	printf("Key of key1 gave %s\n", (char *)hashtable_find(hashtable, "key1"));

	// Check that it works for multiple sets
	hashtable_insert(hashtable, "key1", "hello");
	printf("Key of key1 gave %s\n", (char *)hashtable_find(hashtable, "key1"));
	printf("It was indexed at %d\n", (int) JenkinsHash("key1", 3));

	hashtable_insert(hashtable, "key12", "world");
	printf("Key of key12 gave %s\n", (char *)hashtable_find(hashtable, "key12"));
	printf("It was indexed at %d\n", (int) JenkinsHash("key12", 3));

	hashtable_insert(hashtable, "key123", "this");
	printf("Key of key123 gave %s\n", (char *)hashtable_find(hashtable, "key123"));
	printf("It was indexed at %d\n", (int) JenkinsHash("key123", 3));

	hashtable_insert(hashtable, "key1234", "is");
	printf("Key of key1234 gave %s\n", (char *)hashtable_find(hashtable, "key1234"));
	printf("It was indexed at %d\n", (int) JenkinsHash("key1234", 3));

	hashtable_insert(hashtable, "key12345", "david");
	printf("Key of key12345 gave %s\n", (char *)hashtable_find(hashtable, "key12345"));
	printf("It was indexed at %d\n", (int) JenkinsHash("key12345", 3));

	// Delete the hashtable
	hashtable_delete(hashtable);

	return 0;
}