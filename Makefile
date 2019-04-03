#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/array-and-pthread.c
	gcc src/array-and-pthread.c -o bin/array-and-pthread.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/array-and-pthread.out
	./bin/array-and-pthread.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
