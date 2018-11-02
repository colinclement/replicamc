# This Makefile was made following the tutorial on 
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR) 

PCGLIB=../lib/pcg-c-basic-master
CFLAGS += -I$(PCGLIB)

ODIR=obj

LIBS=$(PCGLIB)/pcg_basic.o -lm

_DEPS=graph.h ll.h cluster.h mc.h util.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=replicamc.o graph.o ll.o cluster.o mc.o util.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

replicamc: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

.PHONY: clean # dont try to make a file named clean

clean:
	rm -f $(ODIR)/*.o *~ core
