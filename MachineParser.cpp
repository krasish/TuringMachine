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

symbol_set& MachineParser::parseFileSymbolsLine(string line) {
	symbol_set* res_set = new symbol_set();
	for (int i = 0; i < line.length(); i++) {
		if (i % 3 == 1 || i % 3 == 2) {
			continue;
		}
		else {
			res_set->emplace(line[i]);
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

string MachineParser::getAfterColumn(string& s) {
	size_t it = s.find(": ");
	if (it == string::npos || it + 2 == s.length())
		return "";
	return s.substr(it + 2);
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
		cerr << RED_TEXT << "Incorrect sets! Sigma MUST be subset of gamma. Sigma MUST NOT contain the blank symbol." << RESET_COLORING;
		return;
	}
	builder->setBlankSymbol(blank_symbol);
	builder->setSigma(sigma);
	builder->setGamma(gamma);

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
		cerr << RED_TEXT << "Tape contains symbols that are not present in gamma!" << RESET_COLORING;
		cout << BRIGHT_CYAN_TEXT << "Tape: " << RESET_COLORING;
		string tape_line;
		getline(cin, tape_line);
		tape = Tape(tape_line, blank_symbol);
	}

	builder->setTape(tape);

	unsigned states_count;
	cout << BRIGHT_CYAN_TEXT << "Number of states: " << RESET_COLORING;
	cin >> states_count;

	while (cin.fail() || states_count < 1) {
		cin.clear();
		cin.ignore();
		cerr << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
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
		cerr << RED_TEXT << "Incorrect input!" << RESET_COLORING << endl;
		cout << BRIGHT_CYAN_TEXT << "Number of transitions: " << RESET_COLORING;
		cin >> transitions_count;
	}
	cin.get();

	readTransitionsFromConsole(transitions, gamma, states, transitions_count);

	builder->setStateMap(states);
	builder->setStartingState(starting);
	builder->setTransitionsMap(transitions);

	cout << BRIGHT_GREEN_TEXT << "Machine read succesfuly!" << endl << RESET_COLORING;

	builder->setLoaded();
}



void MachineParser::parseFromFile(istream& is) {
	if (!is.good()) {
		printFileErrorMessage();
		return;
	}
	string line;
	getline(is, line);

	if (getAfterColumn(line).length() == 0) {
		printFileErrorMessage();
		return;
	}
	char blank_symbol = getAfterColumn(line)[0];

	if (!blank_symbol) {
		blank_symbol = ' ';
	}

	getline(is, line);
	if (getAfterColumn(line).length() == 0) {
		printFileErrorMessage();
		return;
	}

	symbol_set gamma = parseFileSymbolsLine(getAfterColumn(line));
	gamma.emplace(blank_symbol);

	getline(is, line);
	if (getAfterColumn(line).length() == 0) {
		printFileErrorMessage();
		return;
	}

	symbol_set sigma = parseFileSymbolsLine(getAfterColumn(line));

	getline(is, line);
	if (!symbolSetsAreCorrect(sigma, gamma, blank_symbol)) {
		printFileErrorMessage();
		return;
	}

	builder->setBlankSymbol(blank_symbol);
	builder->setGamma(gamma);
	builder->setSigma(sigma);

	Tape tape;
	if (getAfterColumn(line).length() == 0) {
		tape = *(new Tape(blank_symbol));
	}
	else {
		tape = *(new Tape(getAfterColumn(line), blank_symbol));
	}

	if (!tapeIsCorrect(tape, gamma, blank_symbol)) {
		printFileErrorMessage();
		return;
	}

	builder->setTape(tape);

	getline(is, line);

	if (!(line == "States: ")) {
		printFileErrorMessage();
		return;
	}

	state_map states = *(new state_map);

	do {
		getline(is, line);
		if (isLineForCurrentState(line)) {
			break;
		}
		string name = line.substr(2);

		if ((line[0] != 'A' && line[0] != 'R' && line[0] != 'N') || (states.find(name) != states.end())) { // checks whether a state is existing or not
			printFileErrorMessage();
			return;
		}

		StateType type;
		switch (line[0]) {
		case 'A': type = StateType::ACCEPTING; break;
		case 'R': type = StateType::REJECTING; break;
		case 'N': type = StateType::NORMAL; break;
		default: throw invalid_argument("Cannot define type of state");
		}

		states.emplace(name, type);
	} while (!isLineForCurrentState(line));

	string current = getAfterColumn(line);

	if (states.find(current) == states.end()) {
		printFileErrorMessage();
		return;
	}

	getline(is, line);

	if (!(line == "Transitions: ")) {
		printFileErrorMessage();
		return;
	}

	transitions_map transitions = *(new transitions_map);

	try {
		while (!is.eof()) {
			transition t = parseTransition(gamma, states, 0, false, is);
			transitions.insert(t);
		}
	}
	catch (invalid_argument ia) {
		if (strcmp(ia.what(), "STOP")) {
			printFileErrorMessage();
			return;
		}
	}

	builder->setStateMap(states);
	builder->setTransitionsMap(transitions);
	builder->setStartingState(current);

	builder->setLoaded();
	cout << GREEN_TEXT << "Machine loaded from file successfully!" << RESET_COLORING << endl;
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

		while ((state_line[0] != 'A' && state_line[0] != 'R' && state_line[0] != 'N') || (states.find(state_name) != states.end())) { // checks whether a state is existing or not
			cerr << RED_TEXT << "Incorrect state format or states with duplicate names!" << endl << RESET_COLORING;
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
		cerr << RED_TEXT << "Inexisting state!" << RESET_COLORING << endl;
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
		transition current_transition = parseTransition(gamma, states, i, true, std::cin);
		transitions.insert(current_transition);
	}
}

transition MachineParser::parseTransition(symbol_set& gamma, state_map& states, size_t number, bool console, istream& is) {
	if (console) {
		cout << BRIGHT_CYAN_TEXT << "Transition #" << number << " : " << RESET_COLORING;
	}
	string transition_line;
	getline(is, transition_line);
	if (transition_line.empty()) {
		throw invalid_argument("STOP");
	}
	size_t delimiter = transition_line.find("=>");

	if (console) {
		while (delimiter == string::npos
			|| (transition_line[transition_line.length() - 1] != 'L'
				&& transition_line[transition_line.length() - 1] != 'R'
				&& transition_line[transition_line.length() - 1] != 'S'
				)) {
			cerr << RED_TEXT << "Incorrect transition format!" << RESET_COLORING << endl;
			cout << BRIGHT_CYAN_TEXT << "Transition #" << number << " : " << RESET_COLORING;
			getline(cin, transition_line);
			delimiter = transition_line.find("=>");
		}
	}
	else {
		if ((delimiter == string::npos
			|| (transition_line[transition_line.length() - 1] != 'L'
				&& transition_line[transition_line.length() - 1] != 'R'
				&& transition_line[transition_line.length() - 1] != 'S'
				))) {
			throw invalid_argument("Incorrect transition");
		}
	}
	string first_part = transition_line.substr(0, delimiter - 1);
	string second_part = transition_line.substr(delimiter + 3);
	size_t first_space = first_part.find(" ");
	size_t second_space = second_part.find(" ");
	if (console) {
		while (first_space == string::npos
			|| second_space == string::npos
			|| states.find(first_part.substr(0, first_space)) == states.end()
			|| states.find(second_part.substr(0, second_space)) == states.end()
			|| gamma.find(first_part[first_space + 1]) == gamma.end()
			|| gamma.find(second_part[second_space + 1]) == gamma.end()
			) {
			cerr << RED_TEXT << "Incorrect transition format!" << RESET_COLORING << endl;
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
	}
	else {
		if (first_space == string::npos
			|| second_space == string::npos
			|| states.find(first_part.substr(0, first_space)) == states.end()
			|| states.find(second_part.substr(0, second_space)) == states.end()
			|| gamma.find(first_part[first_space + 1]) == gamma.end()
			|| gamma.find(second_part[second_space + 1]) == gamma.end()) {
			throw invalid_argument("Incorrect transition");
		}
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

