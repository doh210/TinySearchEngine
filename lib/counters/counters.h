#ifndef __COUNTERS_H
#define __COUNTERS_H

typedef struct counters counters_t;

counters_t *counters_new();

void counters_add(counters_t *ctrs, int key);

int counters_get(counters_t *ctrs, int key);

void counters_delete(counters_t *ctrs);

/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 */
void counters_set(counters_t *ctrs, int key, int count);

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs,
		      void (*itemfunc)(void *arg, const int key, int count),
		      void *arg);

#endif