# Makefile for 'querier' module
#
# David Oh, August 2016

C = ../common
L = ../lib

PROG = querier
OBJS = querier.o 
LIBS = -lcurl
LLIBS = $C/common.a $L/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies;
querier.o: $C/file.h $C/index.h $C/word.h $L/counters/counters.h

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)