#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <cstdlib> // debug exit()
#include <sstream>
#include <algorithm>

#ifndef RANDOM_SEED
#define RANDOM_SEED 9595
#endif // RANDOM_SEED

#ifndef PRECISION
#define PRECISION 1e-6
#endif // PRECISION

#ifndef FS_CHUNK
#define FS_CHUNK 10000
#endif // FS_CHUNK

#ifndef Q_OPTIMIZATION_ATTEMPTS
#define Q_OPTIMIZATION_ATTEMPTS 50
#endif // Q_OPTIMIZATION_ATTEMPTS

#define DM if(DEBUG)
#define DEBUG false

#include "graph_binary.h"
#include "community.h"

#include "pareto.h"

double modularity(const std::string& annGenotype, const std::vector<int>& annTopology) {
	DM std::cout << "mcall" << std::flush;
	Community c(annGenotype, annTopology, -1, PRECISION, true);
	DM std::cout << "Comm" << std::flush;

	Graph g;
	bool improvement = true;
	double new_mod;
	do {
		DM std::cout << "." << std::flush;
		improvement = c.one_level();
		DM std::cout << "o" << std::flush;
		new_mod = c.modularity();
		DM std::cout << "O" << std::flush;
		g = c.partition2graph_binary();
		DM std::cout << "0" << std::flush;
		c = Community(g, -1, PRECISION, false);
	} while(improvement);
	return new_mod;
/*
	Test:
	std::vector<std::string> annGenomes = {
		"43 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.1 0 1",
		"42 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.9 0 1",
		"41 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.5 0.2 1 0.1 0 1",
		"40 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.4 0.2 1 0.1 0 1",
		"39 1 0 0 1 0 0.5 0.5 0 0.2 0 0.5 0.2 1 0.1 0 1",
		"38 1 0 0 1 0 0.5 0.5 0 0.2 0 0.5 0.2 1 0.1 0 1",
		"37 1 0 0 -1 0 0.5 -0.5 0 -0.2 0 -0.5 0.2 -1 0.1 0 -1",
		"36 -1 0 0 -1 0 -0.5 -0.5 0 -0.2 0 -0.5 -0.2 -1 -0.1 0 -1"
	};
	for(auto gen : annGenomes)
	  std::cout << gen << " Q:" << modularity(gen, annTopology) << std::endl;
*/
}

/*
void removeSubstrs(std::string& s, const std::string& substr) {
	auto n = substr.length();
	for(auto i=s.find(substr); i!=std::string::npos; i=s.find(substr))
		s.erase(i, n);
}
*/

int main(int argc, char **argv) {
//	unsigned int skipColumns = std::stoi(argv[1]);

	// Set your network topology here and recompile.
	// For info on the network topology description format,
	// see the comment for the Graph class that takes network
	// topology as an argument (currently lines 78-83 of graph_binary.cpp)
	std::vector<int> annTopology = {}; // fully connected network, complete adjacency matrix expected in form of concatenated space-separated rows
//	std::vector<int> annTopology = {4, -2, 2}; // network with 4 inputs, 2 hidden neurons with recurrent connections and 2 outputs
                                               // Expected network format is a concatenation of inter-layer connections weight matrices represented as
                                               // concatenated space-separated rows. For recurrent layers include the recurrent connections matrix
                                               // after the matrix of the non-recurrent layer directly above it (inputs are up).

	std::vector<std::string> annGenomes;
	for(std::string line; std::getline(std::cin, line);) {
		annGenomes.push_back(std::string(line));
	}

/*	std::vector<std::string> annGenomes = {
		"27845944 0.0 1.0 -1.0 -1.0 0.0 0.0 0.0 -1.0 1.0 0.0 -1.0 0.0 0.0 1.0 1.0 0.0",
		"34384856 1.0 0.0 -1.0 0.0 0.0 -1.0 0.0 1.0 1.0 -1.0 -1.0 1.0 0.0 -1.0 0.0 1.0",
		"42 1 0 -1 0 0 1 0 -1 1 -1 0 1 1 0 0 1"
	};
*/
	for(auto gen : annGenomes)
	  std::cout << gen << " Q: " << modularity(gen, annTopology) << std::endl;

	return 0;
}

