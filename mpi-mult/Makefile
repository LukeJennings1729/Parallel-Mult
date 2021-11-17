# Makefile for mpi-mult.c
# Nov 16, 2021
CC=gcc -g -Wall
CFILES=mult.c
OFILES=$(CFILES:.c=.o)

mult:	$(OFILES)
	$(CC) -o mult $(OFILES) -lm -lgmp

.c.o:
	$(CC) -c $<

clean::
	/bin/rm $(OFILES) mult
