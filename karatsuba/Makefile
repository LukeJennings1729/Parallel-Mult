# Makefile for karatsuba.c
# Nov 16, 2021
CC=gcc -g -Wall
CFILES=karatsuba.c timer.c
OFILES=$(CFILES:.c=.o)

karatsuba:	$(OFILES)
	$(CC) -o karatsuba $(OFILES) -lm -lgmp -lpthread

.c.o:
	$(CC) -c $<

clean::
	/bin/rm $(OFILES) karatsuba
