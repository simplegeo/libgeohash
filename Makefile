# Simple Makefile for static library construction and testing

all: library test

library:
	gcc -c SGGeohash.c -o geohash.o
	ar rcs libgeohash.a geohash.o
	rm geohash.o
test:
	gcc SGGeohashTest.c libgeohash.a 	
	./a.out
	rm a.out
clean: 
	rm -rf *.a *.o
