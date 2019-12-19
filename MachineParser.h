#pragma once
#include <iostream>
#include <algorithm>
#include "MachineBuilder.h"

using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::set_difference;

typedef pair<state_pair, state_tuple> transition;

class MachineParser {
public:

	MachineParser(MachineBuilder* factory = nullptr) : factory(factory) {}

	void parseFromConsole();
	void parseFromFile();

private:
	MachineBuilder* factory;

	symbol_set& parseSymbolsLine(string line);
	void readSymbolsFromConsole(symbol_set& sigma, symbol_set& gamma, char blank_symbol);
	void readStatesFromConsole(state_map&, string&, unsigned);
	void readTransitionsFromConsole(transitions_map&, symbol_set&, state_map&, unsigned);
	transition parseTransition(symbol_set&, state_map&, size_t number);
	bool symbolSetsAreCorrect(symbol_set& sigma, symbol_set& gamma, char blank_symbol);
	bool tapeIsCorrect(Tape& tape, symbol_set& gamma, char blank_symbol);
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
