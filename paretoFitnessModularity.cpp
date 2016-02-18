#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef RANDOM_SEED
#define RANDOM_SEED 10000
#endif // RANDOM_SEED

#ifndef PRECISION
#define PRECISION 1e-6
#endif // PRECISION

#ifndef FS_CHUNK
#define FS_CHUNK 50
#endif // FS_CHUNK

#include "graph_binary.h"
#include "community.h"

#include "pareto.h"

double modularity(const std::string& annGenotype, const std::vector<int>& annTopology) {
	Community c(annGenotype, annTopology, -1, PRECISION);
//	c.g.display();
	c.g.sanitize();
//	c.g.display();
//	std::cout << "\n";
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

int main(int argc, char **argv) {
	srand(RANDOM_SEED);
	std::vector<int> annTopology = {4, -2, 2};

	std::ifstream ifs;
	ifs.open("population0.log");
	assert(ifs.is_open());

	std::vector<double> fitness;
	std::vector<std::string> genomes;
	std::string curline;
	bool lastChunk = false;
	ParetoFront pf;

	while(!lastChunk) {
		for(unsigned int i=0; i<FS_CHUNK; i++) {
			if(!std::getline(ifs, curline)) {
				lastChunk = true;
				break;
			}
			if(curline[0]=='#' || curline=="")
				continue;
			auto spacepos = curline.find(' ');
			fitness.push_back(std::stod(curline.substr(0, spacepos)));
			genomes.push_back(curline.substr(spacepos+1));
		}

		for(unsigned int i=0; i<fitness.size(); i++)
			updateParetoFront(pf, {fitness[i], modularity(genomes[i], annTopology)}, genomes[i]);

		fitness.clear();
		genomes.clear();

//		std::cout << "Pareto front:\n";
//		printParetoFront(pf);
//		std::cout << "Alternative representation:\n";
//		std::cout << str(pf) << "\n\n";
	}

	ifs.close();

	std::ofstream ofs;
	ofs.open("paretoFrontOfFitnessVsModularity.log", std::ofstream::out | std::ofstream::trunc);
	assert(ofs.is_open());
	ofs << "# Columns: score modularity id indivDesc0 indivDesc2 ...\n";
	ofs << str(pf);
	ofs.close();

	return 0;
}

