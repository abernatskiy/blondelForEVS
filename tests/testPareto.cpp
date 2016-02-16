#include "../pareto.h"

int main(int argc, char **argv) {
	std::vector<int> annTopology = {4, -2, 2};

	ParetoFront pf;

	updateParetoFront(pf, {1,1}, "A");
	printParetoFront(pf);

	updateParetoFront(pf, {1,1}, "B");
	printParetoFront(pf);

	updateParetoFront(pf, {2,2}, "C");
	printParetoFront(pf);

	updateParetoFront(pf, {0.5,1}, "D");
	printParetoFront(pf);

	updateParetoFront(pf, {1,0.2}, "E");
	printParetoFront(pf);

	updateParetoFront(pf, {1.5,1.5}, "F");
	printParetoFront(pf);

	updateParetoFront(pf, {0.3,0.3}, "G");
	printParetoFront(pf);

	updateParetoFront(pf, {1,0.2}, "H");
	printParetoFront(pf);

	return 0;
}

