#ifndef __HASHTABLE_H
#define __HASHTABLE_H

typedef struct hashtable hashtable_t;

hashtable_t *hashtable_new(const int num_slots, void (*htDelete)(void *data));

void *hashtable_find(hashtable_t *ht, char *key);

bool hashtable_insert(hashtable_t *ht, char *key, void *data);

void hashtable_delete(hashtable_t *ht);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, 
		       void (*itemfunc)(void *arg, const char *key, void *data),
		       void *arg);

#endif