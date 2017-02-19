#ifndef __SET_H
#define __SET_H
#include <stdbool.h>

typedef struct set set_t;

set_t *set_new();

void *set_find(set_t *set, char *key);

bool set_insert(set_t *set, char *key, void *data);

void set_delete(set_t *set);

/* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void set_iterate(set_t *set,
		  void (*itemfunc)(void *arg, const char *key, void *data),
		  void *arg);

#endif