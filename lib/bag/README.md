README for bag.c
David Oh, July 2016

Usage:
 * bag.c - contains data structures and functions to create, insert, extract and delete
 * Contains the extra credit delete function
 * 
 * usage: data structure to store data as a linked list
 *	  bag_new must be given the destructor function (i.e. free)
 *	  pass NULL for destructor function to disable delete
 *
 * David Oh, July 2016

Assumptions:

- bag_insert, bag_extract, and bag_delete cannot be called after bag_delete has already been called

"Non"-Assumptions:

Limitations:

- difficulty deleting when containing advanced data structures (i.e. bag of sets)