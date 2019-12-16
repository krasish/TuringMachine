#include "MachineFactory.h"

symbol_set& MachineFactory::parseSymbolsLine(string line) {
	symbol_set* res_set = new symbol_set();
	for (const auto& ch : line) {
		if (ch == ' ') {
			continue;
		}
		else {
			res_set->insert(ch);
		}
	}
	return *res_set;
}

bool MachineFactory::symbolSetsAreCorrect() {
	if (gamma.find(blank_symbol) == gamma.end()) {
		return false;
	}
	if (sigma.find(blank_symbol) != sigma.end()) {
		return false;
	}
	for (auto it = sigma.cbegin(); it != sigma.cend(); it++) {
		if (gamma.find(*it) == gamma.end()) {
			return false;
		}
	}
	return true;
}

bool MachineFactory::tapeIsCorrect() {
	string tape_line = (string)tape;
	if (tape_line.size() == 1 && tape_line[0] == blank_symbol) {
		return true;
	}
	for (char ch : tape_line) {
		if (gamma.find(ch) == gamma.end())
			return false;
	}
	return true;
}

void MachineFactory::readSymbolsFromConsole() {
	cout << BRIGHT_CYAN_TEXT << "Blank symbol: " << RESET_COLORING;
	string first_line;
	getline(cin, first_line);
	blank_symbol = first_line[0];

	cout << BRIGHT_CYAN_TEXT << "Sigma: " << RESET_COLORING;
	string sigma_line;
	getline(cin, sigma_line);
	sigma = parseSymbolsLine(sigma_line);

	cout << BRIGHT_CYAN_TEXT << "Gamma: " << RESET_COLORING;

	string gamma_line;
	getline(cin, gamma_line);
	gamma = parseSymbolsLine(gamma_line);
	gamma.insert(blank_symbol);
}

void MachineFactory::readStatesFromConsole(unsigned count) {
	cout << endl;
	cout << CYAN_TEXT << "Input states in format: \"<T> <statename>\"" << endl;
	cout << '\t' << "<T> <- {A, R, N} where A = ACCEPTING; R = REJECTING; N = NORMAL" << endl;
	cout << '\t' << "<statename> = A regular string" << endl << RESET_COLORING;
	cout << endl;


	for (size_t i = 0; i < count; i++) {
		string state_line;
		while (state_line.size() < 2) {
			cout << BRIGHT_CYAN_TEXT << "State #" << i << " : " << RESET_COLORING;
			getline(cin, state_line);
		}
		string state_name = state_line.substr(2);

		while ((state_line[0] != 'A' && state_line[0] != 'R' && state_line[0] != 'N') || (states.find(state_name) != states.end())) {
			cout << RED_TEXT << "Incorrect state format or states with duplicate names!" << endl << RESET_COLORING;
			cout << BRIGHT_CYAN_TEXT << "State #" << i << " : " << RESET_COLORING;
			getline(cin, state_line);
			state_name = state_line.substr(2);
		}

		StateType type;
		switch (state_line[0]) {
		case 'A': type = StateType::ACCEPTING; break;
		case 'R': type = StateType::REJECTING; break;
		case 'N': type = StateType::NORMAL; break;
		default: throw invalid_argument("Cannot define type of state");
		}

		states.emplace(state_name, type);
	}

	cout << BRIGHT_CYAN_TEXT << "Initial state: " << RESET_COLORING;
	getline(cin, starting);
	while (states.find(starting) == states.end()) {
		cout << RED_TEXT << "Inexisting state!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Initial state: " << RESET_COLORING;
		getline(cin, starting);
	}

}

void MachineFactory::readTransitionsFromConsole(unsigned count) {
	cout << endl;
	cout << CYAN_TEXT << "Input transitions in format: \"<statename> <symbol> => <statename> <symbol> <TM>\"" << endl;
	cout << '\t' << "<TM> <- {L, R, S} where A = LEFT; R = RIGHT; S = STAY" << endl;
	cout << '\t' << "<statename> = Existing state" << endl << RESET_COLORING;
	cout << endl;

	for (size_t i = 0; i < count; i++) {
		transition current_transition = parseTransition(i);
		transitions_map.insert(current_transition);
	}
}

transition MachineFactory::parseTransition(size_t number) {

	cout << BRIGHT_CYAN_TEXT << "Transition #" << number << " : " << RESET_COLORING;
	string transition_line;
	getline(cin, transition_line);
	size_t delimiter = transition_line.find("=>");

	while (delimiter == string::npos 
		||  (  transition_line[transition_line.length() - 1] != 'L' 
			&& transition_line[transition_line.length() - 1] != 'R'
			&& transition_line[transition_line.length() - 1] != 'S'
		)) {
		cout << RED_TEXT << "Incorrect transition format!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Transition #" << number << " : " << RESET_COLORING;
		getline(cin, transition_line);
		delimiter = transition_line.find("=>");
	}

	string first_part = transition_line.substr(0, delimiter - 1);
	string second_part = transition_line.substr(delimiter + 3);
	size_t first_space = first_part.find(" ");
	size_t second_space = second_part.find(" ");

	while (first_space == string::npos
		|| second_space == string::npos
		|| states.find(first_part.substr(0, first_space)) == states.end()
		|| states.find(second_part.substr(0, second_space)) == states.end()
		|| gamma.find(first_part[first_space + 1]) == gamma.end() 
		|| gamma.find(second_part[second_space + 1]) == gamma.end()
		) {
		cout << RED_TEXT << "Incorrect transition format!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Transition #" << number << " : " << RESET_COLORING;
		getline(cin, transition_line);
		delimiter = transition_line.find("=>");
		if (delimiter == string::npos) {
			continue;
		}
		string first_part = transition_line.substr(0, delimiter - 1);
		string second_part = transition_line.substr(delimiter + 3);
		size_t first_space = first_part.find(" ");
		size_t second_space = second_part.find(" ");
	}

	string first_state = first_part.substr(0, first_space);
	char first_char = first_part[first_space + 1];
	string second_state = second_part.substr(0, second_space);
	char second_char = second_part[second_space + 1];
	TapeMovement movement;

	switch (transition_line[transition_line.length() - 1]) {
	case 'L':
		movement = TapeMovement::LEFT; break;
	case 'R':
		movement = TapeMovement::RIGHT; break;
	case 'S':
		movement = TapeMovement::STAY; break;
	default:
		throw invalid_argument("Illegal tape movement symbol for transition " + number);
	}

	return transition{ state_pair{first_state, first_char}, state_tuple{second_state, second_char, movement} };
}

void MachineFactory::readMachineFromConsole() {
	readSymbolsFromConsole();

	if (!symbolSetsAreCorrect()) {
		cout << RED_TEXT << "Incorrect sets! Sigma MUST be subset of gamma. Sigma MUST NOT contain the blank symbol." << RESET_COLORING;
		return;
	}

	cout << BRIGHT_CYAN_TEXT << "Initialize tape with string [y/n]? " << RESET_COLORING;
	char initialize_tape = 'n';
	cin >> initialize_tape;
	cin.get();

	if (initialize_tape == 'y') {
		cout << BRIGHT_CYAN_TEXT << "Tape: " << RESET_COLORING;
		string tape_line;
		getline(cin, tape_line);
		tape = Tape(tape_line, blank_symbol);
	}
	else {
		tape = Tape(blank_symbol);
	}
	if (!tapeIsCorrect()) {
		cout << RED_TEXT << "Tape contains symbols that are not present in gamma!" << RESET_COLORING;
	}

	unsigned states_count;
	cout << BRIGHT_CYAN_TEXT << "Number of states: " << RESET_COLORING;
	cin >> states_count;

	while (cin.fail()) {
		cin.clear();
		cin.ignore();
		cout << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Number of states: " << RESET_COLORING;
		cin >> states_count;
	}
	cin.get();

	readStatesFromConsole(states_count);

	unsigned transitions_count;
	cout << BRIGHT_CYAN_TEXT << "Number of transitions: " << RESET_COLORING;
	cin >> transitions_count;

	while (cin.fail()) {
		cin.clear();
		cin.ignore();
		cout << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Number of transitions: " << RESET_COLORING;
		cin >> transitions_count;
	}
	cin.get();

	readTransitionsFromConsole(transitions_count);

	cout << BRIGHT_GREEN_TEXT << "Machine read succesfuly!" << endl << RESET_COLORING;
	machine_loaded = true;
}

Machine& MachineFactory::getMachine() {
	if (!machine_loaded) {
		throw invalid_argument("Machine was not loaded");
	}
	MachineSymbols* m_symbols = new MachineSymbols(sigma, gamma, blank_symbol);
	StatesUtil* m_states = new StatesUtil(states, transitions_map, starting);
	return *(new Machine(*m_symbols, *m_states, tape)); 
	
}
