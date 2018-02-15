#!/bin/bash

CC=g++
CFLAGS=-std=c++11 -O2 -msse2 -ffast-math -m64 -fno-rtti -fno-exceptions -fno-stack-protector ${DISTFLAGS} ${MORECFLAGS} -g -ggdb -Wall
LDFLAGS=-m64 -g -ggdb -Wall
TESTS=tests/testGraph tests/testPareto tests/testNumericGenome
OBJPARETO=pareto.o
OBJMOD=graph_binary.o community.o paretoFitnessModularity.o ${OBJPARETO}
OBJDIST=numeric_genome.o paretoFitnessDistance.o ${OBJPARETO}
EXEC=gpModularity paretoFitnessModularity paretoFitnessDistanceForModularity paretoFitnessDistanceForSparsity

paretoFitnessDistanceForModularity: DISTFLAGS = -DOUTPUT_DIR="\"fitnessDistanceParetoFrontsForModularity\"" -DCOLUMNS_TO_IGNORE=2
paretoFitnessDistanceForSparsity: DISTFLAGS = -DOUTPUT_DIR="\"fitnessDistanceParetoFrontsForSparsity\"" -DCOLUMNS_TO_IGNORE=1

all: paretoFitnessModularity
	make paretoFitnessDistanceForModularity
	make paretoFitnessDistanceForSparsity

tests: ${TESTS}

paretoFitnessModularity : $(OBJMOD)
	$(CC) -o $@ $^ $(LDFLAGS)

gpModularity : clean graph_binary.o community.o gpModularity.o
	$(CC) -o $@ graph_binary.o community.o gpModularity.o $(LDFLAGS)

paretoFitnessDistanceForModularity : clean ${OBJDIST}
	$(CC) -o $@ ${OBJDIST} $(LDFLAGS)

paretoFitnessDistanceForSparsity : clean ${OBJDIST}
	$(CC) -o $@ ${OBJDIST} $(LDFLAGS)

tests/testGraph : graph_binary.o tests/testGraph.o
	$(CC) -o $@ $^ $(LDFLAGS)

tests/testPareto : pareto.o tests/testPareto.o
	$(CC) -o $@ $^ $(LDFLAGS)

tests/testNumericGenome : numeric_genome.o tests/testNumericGenome.o
	$(CC) -o $@ $^ $(LDFLAGS)

##########################################
# Generic rules
##########################################

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~ $(EXEC) ${TESTS}
