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

void StatesUtil::printStates(ostream& os) {
	for (auto it = states.begin(); it != states.end(); it++) {
		switch (it->second) {
		case StateType::ACCEPTING: os << "A "; break;
		case StateType::REJECTING: os << "R "; break;
		case StateType::NORMAL: os << "N "; break;
		}
		os << it->first << '\n';
	}
	os << "Current: " << current << '\n';
}

void StatesUtil::printTransitions(ostream& os) {
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
