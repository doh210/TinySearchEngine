README for hashtable.c
David Oh, July 2016

Usage:
 * hashtable.c - contains data structures and functions to create, insert, find and delete
 * 
 * usage: data structure to store data associated with a key in a set in an array of sets
 *	  hashtable_new must be given the destructor function (i.e. free)
 *	  pass NULL for destructor function to disable delete
 *
 * David Oh, July 2016

Assumptions:

- hashtable_find, hashtable_insert, and hashtable_delete cannot be called after hashtable_delete has already been called
- integer passed into hash function must be equal to the number of slots

"Non"-Assumptions:

Limitations:

- difficulty deleting when containing advanced data structures (i.e. hashtable of sets)