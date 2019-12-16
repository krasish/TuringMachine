#include <iostream>
#include<string>
#include "MachineFactory.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main() {
	MachineFactory mf;
	mf.readMachineFromConsole();
	cout << mf.getMachine().executeAndGetTape();
}