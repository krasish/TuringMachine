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
	
	string filename = "C:\\Users\\krasi\\Desktop\\s.txt";
	std::ifstream ifs(filename);
	MachineBuilder mf;
	MachineParser p(&mf);
	//p.parseFromConsole();
	p.parseFromFile(ifs);
	Machine m = mf.getMachine();
	m.print();
	//m.save(ifs);
	ifs.close();
	cout << m.executeAndGetTape();
	
	return 0;
}