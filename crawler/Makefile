# Makefile for 'crawler' module
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016

C = ../common
L = ../lib

PROG = crawler
OBJS = crawler.o $C/web.o $L/hashtable/hashtable.o $L/hashtable/jhash.o $L/bag/bag.o $L/set/set.o
LIBS = -lcurl
LLIBS = $L/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies; add others as needed
crawler.o: $C/web.h $L/hashtable/jhash.h $L/hashtable/hashtable.h $L/bag/bag.h $L/set/set.h
#$C/web.o: $C/web.h
#$L/hashtable/hashtable.o: $L/hashtable/hashtable.h $L/hashtable/jhash.h $L/set/set.h
#$L/hashtable/jhash.o: $L/hashtable/jhash.h
#$L/bag/bag.o: $L/bag/bag.h
#$L/set/set.o: $L/set/set.h

# build the library
$L/cs50ds.a: 
	cd $L; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd $L; $(MAKE) clean
