#include "MachineBuilder.h"

Machine& MachineBuilder::getMachine() {
	if (!machine_loaded) {
		throw invalid_argument("Machine was not loaded");
	}
	MachineSymbols* m_symbols = new MachineSymbols(sigma, gamma, blank_symbol);
	StatesUtil* m_states = new StatesUtil(states, transitions, current, starting);
	return *(new Machine(*m_symbols, *m_states, tape)); 
	
}
