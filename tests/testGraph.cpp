#include <string>
#include <iostream>
#include <vector>

#include "../graph_binary.h"

int main(int argc, char **argv)
{
	std::vector<int> topology = {4, -2, 2};

	std::cout << "Testing non-recurrent network...\n";

	std::string inputFile("nonrecurrentGraph.bin");
	std::string weightsFile("nonrecurrentGraph.wgt");

	Graph testGraph0(inputFile.c_str(), weightsFile.c_str(), WEIGHTED);
	testGraph0.display_raw();
	testGraph0.display();

	Graph testGraph1("42 1 0 0 1 0 0.5 0.5 0 0 0 0 0 1 0.1 0 1", topology);
	testGraph1.display_raw();
	testGraph1.display();

	std::cout << std::endl << "Testing network with recurrent connections...\n";

	inputFile = "recurrentGraph.bin";
	weightsFile = "recurrentGraph.wgt";

	Graph testGraph2(inputFile.c_str(), weightsFile.c_str(), WEIGHTED);
	testGraph2.display_raw();
	testGraph2.display();

	Graph testGraph3("41 1 0 0 1 0 0.5 0.5 0 0.2 0.5 0 -0.2 1 0.1 0 1", topology);
	testGraph3.display_raw();
	testGraph3.display();

	return 0;
}

