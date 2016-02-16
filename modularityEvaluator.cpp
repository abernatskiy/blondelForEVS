#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <map>

#include "graph_binary.h"
#include "community.h"

#ifndef RANDOM_SEED
#define RANDOM_SEED 10000
#endif // RANDOM_SEED

#ifndef PRECISION
#define PRECISION 1e-6
#endif // PRECISION

#define NUM_OBJECTIVES 2

typedef std::array<double,NUM_OBJECTIVES> ObjSpacePoint;
typedef std::map<ObjSpacePoint,std::vector<std::string>> ParetoFront;

void checkConnection(const Graph& g, int i, int j) {
	if(g.has_directed_connection(i, j))
		std::cout << i << "->" << j << std::endl;
}

double modularity(const std::string& annGenotype, const std::vector<int>& annTopology) {
	Community c(annGenotype, annTopology, -1, PRECISION);
	c.g.sanitize();
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

bool firstDominatesSecond(const ObjSpacePoint& first, const ObjSpacePoint& second) {
	for(int i=0; i<NUM_OBJECTIVES; i++)
		if(second.m_evals[i] < first.m_evals[i])
			return true;
	return false;
};

void updateParetoFront(ParetoFront& pf, ObjSpacePoint osp, std::string genome) {
	// checking whether this particular pair of objective values is present in the Pareto front or not
	auto pfRecord = pf.find(osp);

	if(pfRecord != pf.end()) {
		// if yes, just append the genome to the vector of the previosly encountered genomes with the same pair
		(pfRecord->second).push_back(genome);
	}
	else {
		// if no, then consider the new pair for the Pareto front
		// first, check if the new pair is nondominated
		for(auto curPfRecord : pf) {
			if(firstDominatesSecond(curPfRecord.first, osp))
				return;
		}
		// next, check whether it dominates any previous records and if it does, delete any
		for(auto pfit=pf.cbegin(); pfit!=pf.cend();) {
			if(firstDominatesSecond(osp, pfit->first))
				pf.erase(pfit++);
			else
				++pfit;
		}
		// last, append the point to the Pareto front and initialize its dictionary with {genome}
		pf[osp] = {genome};
	}
}

int main(int argc, char **argv) {
	srand(RANDOM_SEED);
	std::vector<int> annTopology = {4, -2, 2};

/*
	std::vector<std::string> annGenomes = {
		"43 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.1 0 1",
		"42 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.9 0 1",
		"41 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.5 0.2 1 0.1 0 1",
		"40 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0.4 0.2 1 0.1 0 1",
		"39 1 0 0 1 0 0.5 0.5 0 0.2 0 0.5 0.2 1 0.1 0 1"
	};
	for(auto gen : annGenomes)
	  std::cout << gen << " Q:" << modularity(gen, annTopology) << std::endl;
*/
	return 0;
}

