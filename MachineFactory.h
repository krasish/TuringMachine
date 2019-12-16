#pragma once
#include <iostream>
#include <algorithm>
#include "Machine.h"
#include "StatesUtil.h"

using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::set_difference;
using std::invalid_argument;

typedef pair<state_pair, state_tuple> transition;

class MachineFactory {
private:
	char blank_symbol;
	symbol_set sigma;
	symbol_set gamma;
	Tape tape;
	state_map states;
	transitions_map transitions_map;
	string starting;

	bool machine_loaded = false;

	symbol_set& parseSymbolsLine(string line);
	bool symbolSetsAreCorrect();
	bool tapeIsCorrect();
	
	void readSymbolsFromConsole();
	void readStatesFromConsole(unsigned count);
	void readTransitionsFromConsole(unsigned count);
	transition parseTransition(size_t number);


public:
	void readMachineFromConsole();

	Machine& getMachine();
};

const string BLACK_TEXT = "\x1B[30m";
const string RED_TEXT = "\x1B[31m";
const string GREEN_TEXT = "\x1B[32m";
const string YELLOW_TEXT = "\x1B[33m";
const string BLUE_TEXT = "\x1B[34m";
const string MAGENTA_TEXT = "\x1B[35m";
const string CYAN_TEXT = "\x1B[36m";
const string WHITE_TEXT = "\x1B[37m";
const string BRIGHT_BLACK_TEXT = "\x1B[90m";
const string BRIGHT_RED_TEXT = "\x1B[91m";
const string BRIGHT_GREEN_TEXT = "\x1B[92m";
const string BRIGHT_YELLOW_TEXT = "\x1B[93m";
const string BRIGHT_BLUE_TEXT = "\x1B[94m";
const string BRIGHT_MAGENTA_TEXT = "\x1B[95m";
const string BRIGHT_CYAN_TEXT = "\x1B[96m";
const string BRIGHT_WHITE_TEXT = "\x1B[97m";
const string RESET_COLORING = "\033[0m";
