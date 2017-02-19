# Makefile for 'indexer' module
#
# David Oh, August 2016

C = ../common
L = ../lib

PROG1 = indexer
PROG2 = indextest
OBJS1 = indexer.o
OBJS2 = indextest.o
LIBS = -lcurl
LLIBS = $C/common.a $L/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
CC = gcc
MAKE = make

# build the index

all:
	$(MAKE) $(PROG1)
	$(MAKE) $(PROG2)

$(PROG1): $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies;
indexer.o: $L/set/set.h $L/hashtable/hashtable.h $C/word.h $C/webpage.h $L/hashtable/jhash.h $C/index.h
indextest.o: $L/set/set.h $L/hashtable/hashtable.h $C/word.h $C/webpage.h $L/hashtable/jhash.h $C/index.h

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG1)
	rm -f $(PROG2)
	cd $C; $(MAKE) clean
