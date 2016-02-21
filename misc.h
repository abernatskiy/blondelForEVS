#ifndef MISC_H
#define MISC_H

#include <string>
#include <vector>
#include <array>
#include <sstream>

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

template<int arrSize> void split(const std::string& s, char delim, std::array<std::string,arrSize>& elems) {
	std::stringstream ss(s);
	for(unsigned int i=0; i<arrSize; i++)
		std::getline(ss, elems[i], delim);
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

inline int iabs(int i) {
	return i>0 ? i : -1*i;
}

#endif // MISC_H
