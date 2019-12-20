#include <iostream>
#include <fstream>
#include<string>
#include "MachineBuilder.h"
#include "MachineParser.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main() {
	
	string filename;
	std::ifstream ifs(filename);
	MachineBuilder mf;
	MachineParser p(&mf);
	p.parseFromFile(ifs);
	ifs.close();
	Machine m = mf.getMachine();
	
	cout << m.executeAndGetTape();
	
	return 0;
}