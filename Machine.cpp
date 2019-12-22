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
	states.printStates(os, false);

	os << "Transitions: " << '\n';
	states.printTransitions(os);

}


string Machine::executeAndGetTape() {
	execute();
	return (string)tape;
}

void Machine::print() {
	cout << endl;
	cout << YELLOW_TEXT << "------------------------------------------------\n";
	cout << YELLOW_TEXT << "\t\t| Machine <" << id << "> |" << RESET_COLORING << endl;
	cout << YELLOW_TEXT << "------------------------------------------------\n";

	cout << YELLOW_TEXT << "Blank symbol: " << tape.getBlank() << endl << RESET_COLORING;

	cout << YELLOW_TEXT << "Sigma: " << RESET_COLORING;
	symbols.printSymbols(cout, symbols.sigma);

	cout << YELLOW_TEXT << "Gamma: " << RESET_COLORING;
	symbols.printSymbols(cout, symbols.gamma);

	cout << YELLOW_TEXT << "Tape: " << RESET_COLORING;
	cout << (string)tape << endl;

	cout << YELLOW_TEXT << "States: " << RESET_COLORING << endl;
	states.printStates(cout, true);
	
	cout << YELLOW_TEXT << "Transitions: " << RESET_COLORING << endl;
	states.printTransitions(cout);
	cout << YELLOW_TEXT << "------------------------------------------------\n" << RESET_COLORING;
	cout << endl;

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

MachineSymbols& MachineSymbols::compose(MachineSymbols& other) {
	if (other.blank_symbol != this->blank_symbol) {
		throw invalid_argument("Attempt to compose machines with non-matching blank symbols");
	}
	symbol_set new_sigma = *(new symbol_set(sigma));
	new_sigma.insert(other.sigma.begin(), other.sigma.end());

	symbol_set new_gamma = *(new symbol_set(gamma));
	new_gamma.insert(other.gamma.begin(), other.gamma.end());

	return *(new MachineSymbols(new_sigma, new_gamma, blank_symbol));
}
