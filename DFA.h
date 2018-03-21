#ifndef KOMPILER_DFA_H
#define KOMPILER_DFA_H
#include "machine.h"

std::vector<state> epsilon_closure(state &state);
machine* to_dfa(machine &nfa);
#endif
