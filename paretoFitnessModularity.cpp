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


void removeSubstrs(std::string& s, const std::string& substr) {
	auto n = substr.length();
	for(auto i=s.find(substr); i!=std::string::npos; i=s.find(substr))
		s.erase(i, n);
}

std::string genomeHash(const std::string& genome)
{
	std::string genomeNoID = genome.substr(genome.find(' ')+1);
	genomeNoID.erase(std::remove(genomeNoID.begin(), genomeNoID.end(), '-'), genomeNoID.end());
	genomeNoID.erase(std::remove(genomeNoID.begin(), genomeNoID.end(), ' '), genomeNoID.end());
	removeSubstrs(genomeNoID, ".0");
	return genomeNoID;
}

double processedModularity(const std::string& annGenotype, const std::vector<int>& annTopology) {
	static std::map<std::string,double> cache;
	static std::array<double,Q_OPTIMIZATION_ATTEMPTS> qDataPoints;
	std::string annHash = genomeHash(annGenotype);

	DM std::cout << "Computing modularity of " << annGenotype <<  ", hash " << annHash << "\n";
	auto itCacheRec = cache.find(annHash);
	if(itCacheRec == cache.end()) {
		DM std::cout << "Building a new cache record. Resetting random seed to " << RANDOM_SEED << "\n";
		srand(RANDOM_SEED);
		DM std::cout << "Data points:\n";
		for(unsigned int i=0; i<qDataPoints.size(); i++) {
			DM std::cout << "m" << std::flush;
			qDataPoints[i] = modularity(annGenotype, annTopology);
			DM std::cout << " " << qDataPoints[i];
		}
		DM std::cout << "\n";

		double maxQ = -10;
		for(auto qPoint : qDataPoints) {
			if(maxQ < qPoint)
				maxQ = qPoint;
		}

		cache[annHash] = maxQ;

		DM std::cout << "Assigned maximum: " << cache[annHash] << "\n\n";

		return maxQ;
	}
	else {
		DM {
			std::cout << "Retrieving an existing record: fitness is " << itCacheRec->second << " for hash " << annHash << "\n";
			static int retrievals = 0;
			double testQ = modularity(annGenotype, annTopology);
			if(testQ > (itCacheRec->second)) {
				std::cout << "Found a defective cache record.  Genome: " << annGenotype << " hash: " << annHash << " new_mod: " << testQ << " cached: " << (itCacheRec->second) << "\n";
				std::cout << "Prior to that, made " << cache.size() << " records and did " << retrievals << " retrievals\n";
			}
			std::cout << "\n";
			retrievals += 1;
		}
		return itCacheRec->second;
	}
}

int main(int argc, char **argv) {
	std::vector<int> annTopology = {4, -2, 2};

	std::ifstream ifs;
	ifs.open("population0.log");
	assert(ifs.is_open());

	std::array<double,FS_CHUNK> fitness;
	std::array<std::string,FS_CHUNK> genomes;
	std::string curline;
	bool lastChunk = false;
	ParetoFront pf, curPf;

	long long counter = 0;
	unsigned long curChunkSize = 0;

	while(!lastChunk) {
		for(unsigned long i=0; i<FS_CHUNK; i++) {
			if(!std::getline(ifs, curline)) {
				lastChunk = true;
				curChunkSize = i;
				break;
			}
			if(curline[0]=='#' || curline=="")
				continue;
			auto spacepos = curline.find(' ');
			fitness[i] = std::stod(curline.substr(0, spacepos));
			genomes[i] = curline.substr(spacepos+1);
		}

		if(!lastChunk)
			curChunkSize = FS_CHUNK;

		for(unsigned int i=0; i<curChunkSize; i++) {
			updateParetoFront(curPf, {fitness[i], processedModularity(genomes[i], annTopology)}, genomes[i]);
			if(counter%100000 == 0 && counter != 0)
				std::cout << "Considered 10^5 evaluations recently. Current genome is " << genomes[i] << "\n";
			counter++;
		}

		mergeParetoFronts(pf, curPf);
		curPf.clear();

		DM std::cout << "Pareto front:\n";
		DM printParetoFront(pf);
		DM std::cout << "Alternative representation:\n";
		DM std::cout << str(pf) << "\n\n";
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

