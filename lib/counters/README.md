README for counters.c
David Oh, July 2016

Usage:
 * counters.c - contains data structures and functions to increment, retrieve count, delete
 * 
 * usage: data structures to store counters with keys as a linked list 
 *	  counters_new must be given the destructor function (i.e. free)
 *	  pass NULL for destructor function to disable delete
 *
 * David Oh, July 2016

Assumptions:

- counters_add, counters_get, and counters_delete cannot be called after counters_delete has already been called

"Non"-Assumptions:

Limitations: