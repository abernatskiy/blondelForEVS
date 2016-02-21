#include <iostream>
#include <algorithm>
#include <numeric>
#include "numeric_genome.h"
#include "misc.h" // for split()

using namespace std;

NumericGenome::NumericGenome(string genome) : m_origGen(genome) {
	static array<string,GENOME_SIZE_WITH_ID> geneStrs;
	split<GENOME_SIZE_WITH_ID>(genome, ' ', geneStrs);
	transform(geneStrs.begin()+1, geneStrs.end(), m_nums.begin(), [](string s){return GENE_FROM_STR(s);});
}

void NumericGenome::debugPrint() const {
	cout << "Original genome: " << m_origGen << "\nNumerical representation: ";
	for(auto gene : m_nums)
		cout << " " << gene;
	cout << "\n";
}

GENE_TYPE NumericGenome::distance(const NumericGenome& other) const {
	static array<GENE_TYPE,GENOME_SIZE> diffs;
	for(unsigned int i=0; i<GENOME_SIZE; i++)
		diffs[i] = ABSFUN(m_nums[i] - other.m_nums[i]);
	return accumulate(diffs.begin(), diffs.end(), 0);
}
