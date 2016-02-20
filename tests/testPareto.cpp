#include <iostream>

#include "../pareto.h"

int main(int argc, char **argv) {

	std::cout << "Filling the first Pareto front:\n";

	ParetoFront pf;

	updateParetoFront(pf, {-1,-1}, "A");
	printParetoFront(pf);

	updateParetoFront(pf, {-1,-1}, "B");
	printParetoFront(pf);

	updateParetoFront(pf, {-2,-2}, "C");
	printParetoFront(pf);

	updateParetoFront(pf, {-0.5,-1}, "D");
	printParetoFront(pf);

	updateParetoFront(pf, {-1,-0.2}, "E");
	printParetoFront(pf);

	updateParetoFront(pf, {-1.5,-1.5}, "F");
	printParetoFront(pf);

	updateParetoFront(pf, {-0.3,-0.3}, "G");
	printParetoFront(pf);

	updateParetoFront(pf, {-1,-0.2}, "H");
	printParetoFront(pf);

	std::cout << "\nFilling the second Pareto front:\n";

	ParetoFront opf;

	updateParetoFront(opf, {-4,-0.1}, std::vector<std::string>({"a", "b", "c"}));
	printParetoFront(opf);

	updateParetoFront(opf, {-0.3,-0.4}, "d");
	printParetoFront(opf);

	updateParetoFront(opf, {-0.4,-0.5}, "e");
	printParetoFront(opf);

	updateParetoFront(opf, {-2.,-0.2}, std::vector<std::string>({"f", "g"}));
	printParetoFront(opf);

	updateParetoFront(opf, {-0.25,-0.35}, std::vector<std::string>({"h"}));
	printParetoFront(opf);

	std::cout << "\nMerging the two fronts:\n";

	mergeParetoFronts(pf, opf);
	printParetoFront(pf);

//	updateParetoFront(opf, {}

	return 0;
}

