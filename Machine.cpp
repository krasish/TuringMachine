#include "Machine.h"

bool Machine::execute() {
	while (true) {
		StateType current_state_type = states.transition(tape);	
		if (current_state_type == StateType::ACCEPTING) {
			return true;
		}
		if (current_state_type == StateType::REJECTING) {
			return false;
		}
	}
}

void Machine::save(ostream& os) {
	os << "Blank: " << symbols.blank_symbol << '\n';

	os << "Gamma: ";
	symbols.printSymbols(os, symbols.gamma);

	os << "Sigma: ";
	symbols.printSymbols(os, symbols.sigma);

	os << "Tape: " << (string)tape << '\n';

	os << "States: " << '\n';
	states.printStates(os);

	os << "Transitions: " << '\n';
	states.printTransitions(os);

}


string Machine::executeAndGetTape() {
	execute();
	return (string)tape;
}

void MachineSymbols::printSymbols(ostream& os, symbol_set printable) {
	for (auto it = printable.begin(); it != printable.end(); it++) {
		os << *it;
		if (std::distance(it, printable.end()) != 1) {
			os << ", ";
		}
	}
	os << '\n';
}
