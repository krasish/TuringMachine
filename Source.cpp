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
	string filename2 = "C:\\Users\\krasi\\Desktop\\s2.txt";
	string filename3 = "C:\\Users\\krasi\\Desktop\\composed.txt";
	std::ifstream ifs(filename);
	std::ifstream ifs2(filename2);
	std::ofstream ofs(filename3);
	MachineBuilder mb;
	MachineParser p(&mb);
	p.parseFromFile(ifs);
	Machine m = mb.getMachine();
	p.parseFromFile(ifs2);
	//p.parseFromConsole();
	Machine m2 = mb.getMachine();
	Machine m3 = m.whileMachine(m2);
	m3.save(ofs);
	ifs.close();
	ifs2.close();
	ofs.close();
	m3.print();
	m2.print();
	cout << m2.executeAndGetTape("aaba") << endl;
	cout << m3.executeAndGetTape("ABAaa");
	
	return 0;
}