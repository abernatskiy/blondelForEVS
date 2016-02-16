#ifndef PARETO_H
#define PARETO_H

#include <string>
#include <vector>
#include <array>
#include <map>

#define NUM_OBJECTIVES 2
typedef std::array<double,NUM_OBJECTIVES> ObjSpacePoint;
typedef std::map<ObjSpacePoint,std::vector<std::string>> ParetoFront;

bool firstDominatesSecond(const ObjSpacePoint& first, const ObjSpacePoint& second);
void updateParetoFront(ParetoFront& pf, ObjSpacePoint osp, std::string genome);

void printParetoFront(const ParetoFront& pf);
std::string str(const ObjSpacePoint& osp);
std::string str(const ParetoFront& pf);

#endif // PARETO_H
