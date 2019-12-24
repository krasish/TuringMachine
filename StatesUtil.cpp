#include "StatesUtil.h"

StateType StatesUtil::transition(Tape& tape) {
	state_pair current_pair = { current, tape.getSymbol() };
	state_tuple tuple;

	try {
		tuple = transitions.at(current_pair);
	}
	catch (out_of_range exception) {
		return StateType::REJECTING;
	}

	setCurrent(get<0>(tuple));
	
	tape.replaceSymbol(get<1>(tuple));
	moveTape(tape, get<2>(tuple));

	StateType result;
	try {
		result = states.at(current);
	}
	catch (out_of_range exception) {
		return StateType::REJECTING;
	}
	
	return result;
}

bool StatesUtil::addTrasition(string from, string to, char old_tape, char new_tape, TapeMovement movement) {
	if (states.find(from) == states.end() || states.find(to) == states.end()) {
		return false;
	}
	state_pair key = { from, old_tape };
	state_tuple value = { to, new_tape, movement };
	return (transitions.emplace(key, value)).second;
}

bool StatesUtil::addState(string name, StateType type) {
	return states.try_emplace(name, type).second;
}

void StatesUtil::printStates(ostream& os, bool yellow_starting) const{
	for (auto it = states.begin(); it != states.end(); it++) {
		switch (it->second) {
		case StateType::ACCEPTING: os << "A "; break;
		case StateType::REJECTING: os << "R "; break;
		case StateType::NORMAL: os << "N "; break;
		}
		os << it->first << '\n';
	}

	if (yellow_starting) {
		os << "\x1B[33m" << "Current: " << "\033[0m" << current << '\n';
	}
	else {
	os << "Current: " << current << '\n';
	}
}

void StatesUtil::printTransitions(ostream& os) const{
	for (auto it = transitions.begin(); it != transitions.end(); it++) {
		os << it->first.first << ' ' << it->first.second;
		os << " => ";
		os << get<0>(it->second) << ' ' << get<1>(it->second) << ' ';
		switch (get<2>(it->second)) {
		case TapeMovement::LEFT : os << "L"; break;
		case TapeMovement::RIGHT : os << "R"; break;
		case TapeMovement::STAY : os << "S"; break;
		}
		os << '\n';
	}
}

StatesUtil& StatesUtil::compose(const StatesUtil& other) const{
	state_map new_states = *(new state_map(states));

	for (auto& state : new_states) {
		if (state.second == StateType::ACCEPTING) {
			state.second = StateType::NORMAL;
		}
	}

	set<string> renamed;
	
	for (auto el : other.states) {
		if (new_states.find(el.first) != new_states.end()) {
			new_states.emplace(el.first + "-RNMD", el.second);
			renamed.insert(el.first);
		}
		else {
			new_states.insert(el);
		}
	}

	transitions_map new_transitions = *(new transitions_map(transitions));
	for (auto& el : other.transitions) {
		if (renamed.find(el.first.first) == renamed.end() && renamed.find(get<0>(el.second)) == renamed.end()) {
			new_transitions.insert(el);
		}
		else if (renamed.find(el.first.first) != renamed.end() && renamed.find(get<0>(el.second)) == renamed.end()) {
			new_transitions.emplace(state_pair(el.first.first + "-RNMD", el.first.second), el.second);
		}
		else if (renamed.find(get<0>(el.second)) != renamed.end() && renamed.find(el.first.first) == renamed.end()) {
			new_transitions.emplace(el.first, state_tuple(get<0>(el.second) + "-RNMD", get<1>(el.second), get<2>(el.second)));
		}
		else {
			new_transitions.emplace(state_pair(el.first.first + "-RNMD", el.first.second), state_tuple(get<0>(el.second) + "-RNMD", get<1>(el.second), get<2>(el.second)));
		}
	}
	
	return *(new StatesUtil(new_states, new_transitions, this->current));
}

void StatesUtil::moveTape(Tape& tape, TapeMovement movement) {

	switch (movement) {
	case TapeMovement::LEFT:
		tape.goLeft(); break;
	case TapeMovement::RIGHT:
		tape.goRight(); break;
	case TapeMovement::STAY:
		break;
	default:
		break;
	}
}

void StatesUtil::setCurrent(string newCurrent) {
	if (!(states.find(newCurrent) == states.end())) {
		current = newCurrent;
	}
}
