#ifndef KOMPILER_DFA_H
#define KOMPILER_DFA_H
#include "machine.h"

namespace dfa {
std::set<state> epsilon_closure(state state);
std::set<state> epsilon_closure(std::set<state> states);
std::set<state> move(state states, char in);
std::set<state> move(std::set<state> states, char in);
machine* to_dfa(machine &nfa);
}

#endif
