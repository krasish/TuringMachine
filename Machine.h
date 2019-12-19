#pragma once
#include <memory>
#include <set>
#include "Tape.h"
#include "StatesUtil.h"
#include <ostream>

using std::set;
using std::ostream;
class Machine;


typedef set<char> symbol_set;

struct MachineSymbols {
	symbol_set sigma;
	symbol_set gamma;
	char blank_symbol;

	MachineSymbols(symbol_set sigma = symbol_set(), symbol_set gamma = symbol_set(), char blank_symbol = ' ') : sigma(sigma), gamma(gamma), blank_symbol(blank_symbol) {};
	void printSymbols(ostream& os, symbol_set printable);

};

class Machine {
	friend class MachineBuilder;
public:

	Machine(MachineSymbols symbols = MachineSymbols(), StatesUtil states = StatesUtil(), Tape tape = Tape()) : symbols(symbols), states(states), tape(tape) {};

	bool execute();
	void save(ostream& os);
	string executeAndGetTape();

private:
	Tape tape;
	MachineSymbols symbols;
	StatesUtil states;
};