#include "MachineParser.h"

symbol_set& MachineParser::parseSymbolsLine(string line) {
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

bool MachineParser::symbolSetsAreCorrect(symbol_set& sigma, symbol_set& gamma, char blank_symbol) {
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

bool MachineParser::tapeIsCorrect(Tape& tape, symbol_set& gamma, char blank_symbol) {
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

void MachineParser::readSymbolsFromConsole(symbol_set& sigma, symbol_set& gamma, char blank_symbol) {
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

void MachineParser::parseFromConsole() {
	symbol_set sigma = *(new symbol_set());
	symbol_set gamma = *(new symbol_set());
	char blank_symbol = ' ';

	readSymbolsFromConsole(sigma, gamma, blank_symbol);

	if (!symbolSetsAreCorrect(sigma, gamma, blank_symbol)) {
		cout << RED_TEXT << "Incorrect sets! Sigma MUST be subset of gamma. Sigma MUST NOT contain the blank symbol." << RESET_COLORING;
		return;
	}
	factory->setBlankSymbol(blank_symbol);
	factory->setSigma(sigma);
	factory->setGamma(gamma);

	cout << BRIGHT_CYAN_TEXT << "Initialize tape with string [y/n]? " << RESET_COLORING;
	char initialize_tape = 'n';
	cin >> initialize_tape;
	cin.get();
	Tape tape = *(new Tape);
	if (initialize_tape == 'y') {
		cout << BRIGHT_CYAN_TEXT << "Tape: " << RESET_COLORING;
		string tape_line;
		getline(cin, tape_line);
		tape = Tape(tape_line, blank_symbol);
	}
	else {
		tape = Tape(blank_symbol);
	}
	while (!tapeIsCorrect(tape, gamma, blank_symbol)) {
		cout << RED_TEXT << "Tape contains symbols that are not present in gamma!" << RESET_COLORING;
		cout << BRIGHT_CYAN_TEXT << "Tape: " << RESET_COLORING;
		string tape_line;
		getline(cin, tape_line);
		tape = Tape(tape_line, blank_symbol);
	}

	factory->setTape(tape);

	unsigned states_count;
	cout << BRIGHT_CYAN_TEXT << "Number of states: " << RESET_COLORING;
	cin >> states_count;

	while (cin.fail() || states_count < 1) {
		cin.clear();
		cin.ignore();
		cout << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Number of states: " << RESET_COLORING;
		cin >> states_count;
	}
	cin.get();

	state_map states = *(new state_map);
	string starting = *(new string);
	transitions_map transitions = *(new transitions_map);

	readStatesFromConsole(states, starting, states_count);

	unsigned transitions_count;
	cout << BRIGHT_CYAN_TEXT << "Number of transitions: " << RESET_COLORING;
	cin >> transitions_count;

	while (cin.fail() || transitions_count < 1) {
		cin.clear();
		cin.ignore();
		cout << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Number of transitions: " << RESET_COLORING;
		cin >> transitions_count;
	}
	cin.get();

	readTransitionsFromConsole(transitions,gamma, states, transitions_count);

	factory->setStateMap(states);
	factory->setStartingState(starting);
	factory->setTransitionsMap(transitions);

	cout << BRIGHT_GREEN_TEXT << "Machine read succesfuly!" << endl << RESET_COLORING;

	factory->setLoaded();
}

void MachineParser::readStatesFromConsole(state_map& states, string& starting, unsigned count) {
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

void MachineParser::readTransitionsFromConsole(transitions_map& transitions, symbol_set& gamma, state_map& states, unsigned count) {
	cout << endl;
	cout << CYAN_TEXT << "Input transitions in format: \"<statename> <symbol> => <statename> <symbol> <TM>\"" << endl;
	cout << '\t' << "<TM> <- {L, R, S} where A = LEFT; R = RIGHT; S = STAY" << endl;
	cout << '\t' << "<statename> = Existing state" << endl << RESET_COLORING;
	cout << endl;

	for (size_t i = 0; i < count; i++) {
		transition current_transition = parseTransition(gamma, states, i);
		transitions.insert(current_transition);
	}
}

transition MachineParser::parseTransition(symbol_set& gamma, state_map& states, size_t number) {

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

