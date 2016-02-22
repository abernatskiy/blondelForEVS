#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <sstream>

#ifndef FS_CHUNK
#define FS_CHUNK 10000
#endif // FS_CHUNK

#ifndef DISTANCE_CAP
#define DISTANCE_CAP 3
#endif // DISTANCE_CAP

#define GENOME_SIZE 16
#define GENOME_SIZE_WITH_ID 17
#define GENE_FORMAT double
#define GENE_FROM_STR(X) std::stod(X)

#define DM if(DEBUG)
#define DEBUG true

#include "pareto.h"
#include "numeric_genome.h"

int main(int argc, char **argv) {
	if(argc != 3) {
		std::cout << "Usage: paretoFitnessDistance <baseGenomesFile> <columnsToIgnore>\n";
		return 1;
	}
	unsigned int ignoreColumns = std::stoul(argv[2]);
	std::string baseGenomesFileName(argv[1]);
	unsigned int distanceCap = DISTANCE_CAP;

	// Read the set of genomes to consider from the input file
	std::ifstream baseGenomesFileStream;
	baseGenomesFileStream.open(baseGenomesFileName);
	assert(baseGenomesFileStream.is_open());

	std::vector<NumericGenome> baseGenomes;
	{
		std::string curline;
		unsigned int pos;
		while(std::getline(baseGenomesFileStream, curline)) {
			if(curline[0]!='#' && curline!="") {
				pos = 0;
				for(unsigned int i=0; i<ignoreColumns; i++) {
					pos = curline.find(' ', pos+1);
				}
//				DM std::cout << "Read a genome: " << curline.substr(pos+1) << "\n" << std::flush;
				baseGenomes.push_back(NumericGenome(curline.substr(pos+1)));
			}
		}
	}

	DM {
		std::cout << "Base genomes:\n\n";
		for(auto bg : baseGenomes)
			bg.debugPrint();
		std::cout << "\n";
	}

	// Create two fitness vs distance Pareto front for each base genome - one permanent, one for the current file chunk
	std::vector<ParetoFront> pfs, curPfs;
	pfs.resize(baseGenomes.size());
	curPfs.resize(baseGenomes.size());

	// Read the fitness "database" population0.log and compute the fitness vs distance Pareto fronts
	std::ifstream ifs;
	ifs.open("population0.log");
	assert(ifs.is_open());

	std::array<double,FS_CHUNK> fitness;
	std::array<std::string,FS_CHUNK> genomes;
	bool lastChunk = false;
	unsigned long curChunkSize = 0;

	long long counter = 0;
	std::string curline;
	GENE_TYPE curDist;

	// while population0.log is not depleted
	while(!lastChunk) {
		// read genomes and fitnesses from it
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

		// and use the results to update the Pareto fronts
		for(unsigned int i=0; i<curChunkSize; i++) {
			for(unsigned int curpfi=0; curpfi<baseGenomes.size(); curpfi++) {
				curDist = baseGenomes[curpfi].distance(NumericGenome(genomes[i]));
				if(curDist < distanceCap)
					updateParetoFront(curPfs[curpfi], {fitness[i], curDist}, genomes[i]);
			}
			if(counter%100000 == 0 && counter != 0)
				std::cout << "Considered 10^5 evaluations recently. Current genome is " << genomes[i] << "\n";
			counter++;
		}

		for(unsigned int curpfi=0; curpfi<baseGenomes.size(); curpfi++) {
			mergeParetoFronts(pfs[curpfi], curPfs[curpfi]);
			curPfs[curpfi].clear();
		}

		DM {
			for(unsigned int curpfi=0; curpfi<baseGenomes.size(); curpfi++) {
				std::cout << "Pareto front for " << baseGenomes[curpfi].str() << ":\n";
				std::cout << str(pfs[curpfi]) << "\n";
			}
			std::cout << "\n" << std::flush;
		}
	}

	ifs.close();

/*	std::ofstream ofs;
	ofs.open("paretoFrontOfFitnessVsModularity.log", std::ofstream::out | std::ofstream::trunc);
	assert(ofs.is_open());
	ofs << "# Columns: score modularity id indivDesc0 indivDesc2 ...\n";
	ofs << str(pf);
	ofs.close();
*/
	return 0;
}

