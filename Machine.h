#pragma once
#include <memory>
#include <set>
#include "Tape.h"
#include "StatesUtil.h"

using std::set;
class Machine;


typedef set<char> symbol_set;

struct MachineSymbols {
	symbol_set sigma;
	symbol_set gamma;
	char blank_symbol;

	MachineSymbols(symbol_set sigma = symbol_set(), symbol_set gamma = symbol_set(), char blank_symbol = ' ') : sigma(sigma), gamma(gamma), blank_symbol(blank_symbol) {};
};

class Machine {
	friend class MachineFactory;
public:

	Machine(MachineSymbols symbols = MachineSymbols(), StatesUtil states = StatesUtil(), Tape tape = Tape()) : symbols(symbols), states(states), tape(tape) {};

	bool execute();
	string executeAndGetTape();

private:
	Tape tape;
	MachineSymbols symbols;
	StatesUtil states;
};