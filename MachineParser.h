#pragma once
#include <iostream>
#include <algorithm>
#include "MachineBuilder.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::getline;
using std::set_difference;
using std::istream;

typedef pair<state_pair, state_tuple> transition;

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


class MachineParser {
public:

	MachineParser(MachineBuilder* factory = nullptr) : builder(factory) {}

	void parseFromConsole();
	void parseFromFile(istream&);

private:
	MachineBuilder* builder;

	symbol_set& parseSymbolsLine(string line);
	symbol_set& parseFileSymbolsLine(string line);

	void readSymbolsFromConsole(symbol_set& sigma, symbol_set& gamma, char blank_symbol);
	void readStatesFromConsole(state_map&, string&, unsigned);
	void readTransitionsFromConsole(transitions_map&, symbol_set&, state_map&, unsigned);
	transition parseTransition(symbol_set&, state_map&, size_t number, bool console, istream& is);

	bool symbolSetsAreCorrect(symbol_set& sigma, symbol_set& gamma, char blank_symbol);
	bool tapeIsCorrect(Tape& tape, symbol_set& gamma, char blank_symbol);
	bool isLineForCurrentState(string& line) { return (line.substr(0, 8) == "Current:"); }

	string getAfterColumn(string& s);
	void printFileErrorMessage() { cerr << RED_TEXT << "Loading machine from file was not successful!" << '\n' << RESET_COLORING; }
};

