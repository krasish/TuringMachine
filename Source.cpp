#include <iostream>
#include <fstream>
#include<string>
#include "MachineFactory.h"
#include "MachineParser.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main() {
	MachineFactory mf;
	MachineParser p(&mf);
	p.parseFromConsole();
	Machine m = mf.getMachine();
	cout << m.executeAndGetTape();
	return 0;
}