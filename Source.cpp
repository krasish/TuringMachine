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
	MachineBuilder mf;
	MachineParser p(&mf);
	p.parseFromConsole();
	Machine m = mf.getMachine();
	cout << m.executeAndGetTape();
	return 0;
}