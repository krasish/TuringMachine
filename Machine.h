#pragma once
#include <memory>
#include <set>
#include "Tape.h"
#include "StatesUtil.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::set;
using std::ostream;
using std::invalid_argument;
using std::set_union;

class Machine;

typedef set<char> symbol_set;

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

static unsigned produced_machines = 0;

struct MachineSymbols {
	symbol_set sigma;
	symbol_set gamma;
	char blank_symbol;

	MachineSymbols(symbol_set sigma = symbol_set(), symbol_set gamma = symbol_set(), char blank_symbol = ' ') : sigma(sigma), gamma(gamma), blank_symbol(blank_symbol) {};
	void printSymbols(ostream& os, symbol_set printable);
	MachineSymbols& compose(const MachineSymbols& other);
};

class Machine {
	friend class MachineBuilder;
public:

	Machine(MachineSymbols symbols = MachineSymbols(), StatesUtil states = StatesUtil(), Tape tape = Tape()) : id(produced_machines++), symbols(symbols), states(states), tape(tape) {};

	bool execute();
	void save(ostream& os);
	string executeAndGetTape();
	void print();

	Machine& compose(const Machine& other);

private:
	unsigned id;
	Tape tape;
	MachineSymbols symbols;
	StatesUtil states;
};
