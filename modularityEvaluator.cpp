#include <string>
#include <iostream>
#include <vector>

#include "graph_binary.h"
#include "community.h"

#ifndef RANDOM_SEED
#define RANDOM_SEED 10000
#endif // RANDOM_SEED

#ifndef PRECISION
#define PRECISION 1e-6
#endif // PRECISION

void checkConnection(const Graph& g, int i, int j) {
	if(g.has_directed_connection(i, j))
		std::cout << i << "->" << j << std::endl;
}

double modularity(const std::string& annGenotype, const std::vector<int>& annTopology) {
	Community c(annGenotype, annTopology, -1, PRECISION);

	cout << endl;
//	std::cout << "Was:\n";
//	c.g.display_raw();
	c.g.sanitize();
//	std::cout << "Now:\n";
//	c.g.display_raw();


	checkConnection(c.g, 0, 4);
	checkConnection(c.g, 4, 0);
	checkConnection(c.g, 4, 4);
	checkConnection(c.g, 4, 5);
	checkConnection(c.g, 5, 4);


	Graph g;
	bool improvement = true;
	double new_mod;

	do {
		improvement = c.one_level();
		new_mod = c.modularity();
		g = c.partition2graph_binary();
		c = Community(g, -1, PRECISION);

	} while(improvement);

	return new_mod;
}

int main(int argc, char **argv) {
	srand(RANDOM_SEED);
	std::vector<int> annTopology = {4, -2, 2};
	std::vector<std::string> annGenomes = {
		"43 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.1 0 1",
		"42 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.9 0 1",
		"41 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.5 0.2 1 0.1 0 1",
		"40 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.4 0.2 1 0.1 0 1",
		"39 1 0 0 1 0 0.5 0.5 0 0.2 0 0.5 0.2 1 0.1 0 1"
	};

	for(auto gen : annGenomes)
	  std::cout << gen << " Q:" << modularity(gen, annTopology) << std::endl;

	return 0;
}

