#ifndef NUMERIC_GENOME_H
#define NUMERIC_GENOME_H

#include <array>
#include <string>
#include <cmath> // for fabs(), replace with misc.h for iabs()

#define GENOME_SIZE 16
#define GENOME_SIZE_WITH_ID 17 // ==GENOME_SIZE+1, used in class definition
#define GENE_TYPE double
#define GENE_FROM_STR(X) std::stod(X)
#define ABSFUN(X) fabs(X)

class NumericGenome {
private:
	std::array<GENE_TYPE,GENOME_SIZE> m_nums;
	std::string m_origGen;

public:
	NumericGenome(std::string genome);
	std::string str() const {return m_origGen;};
	void debugPrint() const;
	GENE_TYPE distance(const NumericGenome& other) const; // currently a Manhattan distance
};

#endif // NUMERIC_GENOME_H
