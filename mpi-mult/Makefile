# Makefile for mpi-mult.c
# Nov 16, 2021
CC=gcc -g -Wall
CFILES=thread_mult.c timer.c
OFILES=$(CFILES:.c=.o)

thread_mult:	$(OFILES)
	$(CC) -o thread_mult $(OFILES) -lm -lgmp -lpthread

.c.o:
	$(CC) -c $<

clean::
	/bin/rm $(OFILES) thread_mult
