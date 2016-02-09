#include <string>
#include <iostream>
#include <vector>

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

void printGraph(const Graph& g)
{
	std::cout << "Nodes: " << g.nb_nodes << " links: " << g.nb_links << " totalWeight: " << g.total_weight << "\nDegrees:";
	for(auto d : g.degrees)
		std::cout << " " << d;
	std::cout << "\nLinks:";
	for(auto l : g.links)
		std::cout << " " << l;
	std::cout << "\nWeights:";
	for(auto w : g.weights)
		std::cout << " " << w;
	std::cout << std::endl;
};

int main(int argc, char **argv)
{
	srand(RANDOM_SEED);
	std::vector<int> topology = {4, -2, 2};
	Graph testGraph1("42 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.1 0 1", topology);
	printGraph(testGraph1);
	testGraph1.display();

	std::string inputFile("graph.bin");
	std::string weightsFile("graph.wgt");
	Graph testGraph0(inputFile.c_str(), weightsFile.c_str(), WEIGHTED);
	printGraph(testGraph0);
	testGraph0.display();

//	Community c(inputFile.c_str(), NULL, type, -1, precision);
//	type = WEIGHTED;
//	Community c(inputFile.c_str(), weightsFile.c_str(), type, -1, precision);
/*	Graph g;
	bool improvement = true;
	double new_mod;

	do {
		improvement = c.one_level();
		new_mod = c.modularity();
		g = c.partition2graph_binary();
		c = Community(g, -1, precision);

	} while(improvement);

  std::cout << new_mod << endl;*/

	return 0;
}

