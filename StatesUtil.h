#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <stdexcept>
#include <ostream>
#include "Tape.h"

using std::string;
using std::unordered_map;
using std::pair;
using std::tuple;
using std::hash;
using std::out_of_range;
using std::get;
using std::ostream;

class StatesUtil;
struct State;
enum class TapeMovement { LEFT, RIGHT, STAY };
enum class StateType { ACCEPTING, REJECTING, NORMAL };

typedef pair<string, char> state_pair;
typedef tuple<string, char, TapeMovement> state_tuple;
typedef unordered_map<string, StateType> state_map;
typedef unordered_map<state_pair, state_tuple> transitions_map;

namespace std {
	template<>
	struct hash<state_pair> {
		size_t operator()(state_pair const& v) const {
			size_t s1 = hash<string>{}(v.first);
			size_t s2 = hash<char>{}(v.second);
			return s1 ^ (s2 << 1);
		}
	};
}

class StatesUtil {
	friend class MachineFactory;
public:
	StatesUtil(state_map states = state_map(), transitions_map transitions = transitions_map(), string current = "") : states(states), transitions(transitions), current(current) {};

	StateType transition(Tape& tape);
	bool addTrasition(string from, string to, char old_tape, char new_tape, TapeMovement movement);
	bool addState(string name, StateType type);
	void printStates(ostream& os);
	void printTransitions(ostream& os);
	
private:
	string current;
	state_map states;
	transitions_map transitions;

	void moveTape(Tape& tape, TapeMovement movement);
	void setCurrent(string newCurrent);
	
};

