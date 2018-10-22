# This Makefile was made following the tutorial on 
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=  # -lm

_DEPS=graph.h ll.h cluster.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=test.o graph.o ll.o cluster.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean  # dont try to make a file named clean

clean:
	rm -f $(ODIR)/*.o *~ core
