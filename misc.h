#ifndef MISC_H
#define MISC_H

#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

inline std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

template<int arrSize> inline void split(const std::string& s, char delim, std::array<std::string,arrSize>& elems) {
	std::stringstream ss(s);
	for(unsigned int i=0; i<arrSize; i++)
		std::getline(ss, elems[i], delim);
}

inline std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

inline int iabs(int i) {
	return i>0 ? i : -1*i;
}

inline bool isDir(std::string path) {
	struct stat info;
	if(stat(path.c_str(), &info) != 0)
		return false;
	if(info.st_mode & S_IFDIR)
		return true;
	return false;
}

#endif // MISC_H
