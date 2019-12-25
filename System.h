#pragma once
#include <iostream>
#include <fstream>
#include<string>
#include <regex>
#include <conio.h>
#include <cstdio>
#include <vector>
#include "MachineBuilder.h"
#include "MachineParser.h"

#define INDENT "\t\t\t\t\t "

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::sregex_iterator;
using std::vector;



class System {
private:
	vector<Machine> machines;
	MachineBuilder builder;
	MachineParser parser;


public:

	System() {
		builder = *(new MachineBuilder());
		parser = *(new MachineParser(&builder));
	}
	void operate();
	Machine& getMachine(unsigned id);
	
};