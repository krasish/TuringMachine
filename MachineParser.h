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
	void readStatesFromConsole(state_map&, string&, string&, unsigned);
	void readTransitionsFromConsole(transitions_map&, symbol_set&, state_map&, unsigned);
	transition parseTransition(symbol_set&, state_map&, size_t number, bool console, istream& is);

	bool symbolSetsAreCorrect(symbol_set& sigma, symbol_set& gamma, char blank_symbol);
	bool tapeIsCorrect(Tape& tape, symbol_set& gamma, char blank_symbol);
	bool isLineForStartingState(string& line) { return (line.substr(0, 9) == "Starting:"); }
	bool isLineForCurrentState(string& line) { return (line.substr(0, 8) == "Current:"); }

	string getAfterColumn(string& s);
	void printFileErrorMessage() { cerr << RED_TEXT << "Loading machine from file was not successful!" << '\n' << RESET_COLORING; }
};

