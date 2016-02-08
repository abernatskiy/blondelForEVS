#include <string>
#include <iostream>

#include "graph_binary.h"
#include "community.h"

#ifndef RANDOM_SEED
#define RANDOM_SEED 10000
#endif // RANDOM_SEED

char *filename = NULL;
char *filename_w = NULL;
char *filename_part = NULL;
int type       = UNWEIGHTED;
int nb_pass    = 0;
double precision = 0.000001;
int display_level = -2;
int k1 = 16;

bool verbose = false;

int main(int argc, char **argv)
{
	srand(RANDOM_SEED);
	std::string inputFile("graph.bin");
	std::string weightsFile("graph.wgt");
	Community c(inputFile.c_str(), NULL, type, -1, precision);
//	type = WEIGHTED;
//	Community c(inputFile.c_str(), weightsFile.c_str(), type, -1, precision);
	Graph g;
	bool improvement = true;
	double new_mod;
//	int level = 0;

	do {
		improvement = c.one_level();
		new_mod = c.modularity();
//		if (++level==display_level)
//			g.display();
//		if (display_level==-1)
//			c.display_partition();
		g = c.partition2graph_binary();
		c = Community(g, -1, precision);

	} while(improvement);

  std::cout << new_mod << endl;
}

