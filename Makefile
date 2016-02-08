#!/bin/bash

CC=g++
CFLAGS=-std=c++11 -O2 -msse2 -ffast-math -m64 -fno-rtti -fno-exceptions -fno-stack-protector ${MORECFLAGS} -g -ggdb -Wall
LDFLAGS=-m64 -g -ggdb -Wall
EXEC=modularityEvaluator
OBJ1= graph_binary.o community.o

all: $(EXEC)

modularityEvaluator : $(OBJ1) modularityEvaluator.o
	$(CC) -o $@ $^ $(LDFLAGS)

##########################################
# Generic rules
##########################################

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~ $(EXEC)
