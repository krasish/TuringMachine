#include <iostream>
#include <fstream>
//#include <ostream>
//#include <istream>
#include<string>
#include "MachineFactory.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main() {
	MachineFactory mf;
	mf.readMachineFromConsole();
	Machine m = mf.getMachine();

	if (std::ifstream("C:\\Users\\krasi\\Desktop\\nf.txt").good()) {
		std::ofstream ofs("C:\\Users\\krasi\\Desktop\\nf.txt");
		m.save(ofs);
		ofs.close();
		cout << GREEN_TEXT << "Success!" << RESET_COLORING << endl;
	}
}