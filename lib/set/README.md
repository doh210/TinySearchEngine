README for set.c
David Oh, July 2016

Usage:
 * set.c - contains data structures and functions to create, insert, find and delete
 * Contains the extra credit delete function
 * 
 * usage: data structure to store data associated with a key as a linked list
 *	  set_new must be given the destructor function (i.e. free)
 *	  pass NULL for destructor function to disable delete
 *
 * David Oh, July 2016

Assumptions:

- set_find, set_insert, and set_delete cannot be called after set_delete has already been called
- integer passed into hash function must be equal to the number of slots

"Non"-Assumptions:

Limitations:

- difficulty deleting when containing advanced data structures (i.e. set of sets)