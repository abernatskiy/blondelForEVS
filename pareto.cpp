#include <iostream>
#include <sstream>
#include <cassert>

#include "pareto.h"

bool firstDominatesSecond(const ObjSpacePoint& first, const ObjSpacePoint& second) {
	assert(first.size() == second.size());
	// the objectives are minimized
	bool dominates = true;
	for(unsigned int i=0; i<first.size(); i++)
		if(second[i] <= first[i])
			dominates = false;
	return dominates;
}

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
		std::cout << "Point " << str(osp) << " is nondominated!\n";
		// next, check whether it dominates any previous records and if it does, delete any
		for(auto pfit=pf.cbegin(); pfit!=pf.cend();) {
			if(firstDominatesSecond(osp, pfit->first)) {
				std::cout << str(osp) << " dominates " << str(pfit->first) << ", erasing the latter's record\n";
				pf.erase(pfit++);
			}
			else
				++pfit;
		}
		// last, append the point to the Pareto front and initialize its dictionary with {genome}
		pf[osp] = {genome};
	}
}

void printParetoFront(const ParetoFront& pf) {
	for(auto pfRecord : pf) {
		std::cout << "Objective values:";
		for(auto objVal : pfRecord.first)
			std::cout << " " << objVal;
		std::cout << " Genotypes:\n";
		for(auto genotype : pfRecord.second)
			std::cout << genotype << "\n";
	}
	std::cout << "\n";
}

std::string str(const ObjSpacePoint& osp) {
	std::ostringstream os;
	auto objIt = osp.begin();
	os << *(objIt++);
	for(;objIt!=osp.end(); objIt++)
		os << " " << *objIt;
	return os.str();
}

std::string str(const ParetoFront& pf) {
	std::ostringstream os;
	for(auto pfRecord : pf) {
		os << str(pfRecord.first);
		for(auto genome : pfRecord.second)
			os << " " << genome;
	}
	return os.str();
}
