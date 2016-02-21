#include <iostream>
#include "../numeric_genome.h"

using namespace std;

int main(int argc, char** argv) {
	NumericGenome ng0("39900000 -1.0 0.0 1.0 0.0 0.0 0.0 -1.0 0.0 -1.0 1.0 -1.0 -1.0 -1.0 0.0 1.0 1.0");
	cout << "Constructed " << ng0.str() << endl;
	ng0.debugPrint();

	cout << endl;

	NumericGenome ng1("39900001 1.0 0.0 -1.0 0.0 0.0 0.0 1.0 0.0 -1.0 1.0 -1.0 -1.0 -1.0 0.0 1.0 1.0"); // manually written to be at distance 6
	cout << "Constructed " << ng1.str() << endl;
	ng1.debugPrint();

	cout << endl;

	NumericGenome ng2("28900000 0.0 -1.0 0.0 -1.0 -1.0 0.0 0.0 1.0 -1.0 0.0 -1.0 0.0 -1.0 -1.0 -1.0 1.0");
	cout << "Constructed " << ng2.str() << endl;
	ng2.debugPrint();

	cout << endl << "Pairwise distances:" << endl
				<< "d(ng0,ng1)=" << ng0.distance(ng1) << endl
				<< "d(ng1,ng0)=" << ng1.distance(ng0) << endl
				<< "d(ng1,ng2)=" << ng1.distance(ng2) << endl
				<< "d(ng2,ng1)=" << ng2.distance(ng1) << endl
				<< "d(ng0,ng2)=" << ng0.distance(ng2) << endl
				<< "d(ng2,ng0)=" << ng2.distance(ng0) << endl;

	return 0;
}
