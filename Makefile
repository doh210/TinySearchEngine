# Makefile for TSE
# Builds and compiles all directories
#
# David Oh, August 2016

#OBJS = memory/memory.o bag/bag.o set/set.o hashtable/hashtable.o counters/counters.o
#LIB = cs50ds.a

all:
	$(MAKE) -C lib
	$(MAKE) -C common
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	#rm -f $(LIB)
	cd common; $(MAKE) $@
	cd crawler; $(MAKE) $@
	cd indexer; $(MAKE) $@
	cd lib; $(MAKE) $@
	cd querier; $(MAKE) $@