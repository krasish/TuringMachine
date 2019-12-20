#pragma once
#include "Machine.h"
#include "StatesUtil.h"

using std::invalid_argument;

class MachineBuilder {
private:
	char blank_symbol;
	symbol_set sigma;
	symbol_set gamma;
	Tape tape;
	state_map states;
	transitions_map transitions;
	string starting;

	bool machine_loaded = false;

public:

	Machine& getMachine();

	void setBlankSymbol(char blank_Symbol) { this->blank_symbol = blank_symbol; }
	void setSigma(symbol_set& sigma) { this->sigma = sigma; }
	void setGamma(symbol_set& gamma) { this->gamma = gamma; }
	void setTape(Tape& tape) { this->tape = tape; }
	void setStateMap(state_map& states) { this->states = states; }
	void setTransitionsMap(transitions_map& transitions) { this->transitions = transitions; }
	void setStartingState(string starting) { this->starting = starting; }

	void setLoaded() { machine_loaded = true; }
};

