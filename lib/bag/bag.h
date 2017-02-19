#ifndef __BAG_H
#define __BAG_H

typedef struct bag bag_t;

bag_t *bag_new(void (*bagnodeDelete)(void *data));

void bag_insert(bag_t *bag, void *data);

void *bag_extract(bag_t *bag);

void bag_delete(bag_t *bag);

/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, data).
 */
void bag_iterate(bag_t *bag,
		 void (*itemfunc)(void *arg, void *data),
		 void *arg);

#endif