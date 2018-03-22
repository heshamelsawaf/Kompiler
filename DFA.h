#ifndef KOMPILER_DFA_H
#define KOMPILER_DFA_H
#include "machine.h"
typedef int sid_t;

namespace dfa {
std::set<sid_t> epsilon_closure(machine m, sid_t s);
std::set<sid_t> epsilon_closure(machine m, std::set<sid_t> state_set);
std::set<sid_t> move(machine m, sid_t s, char in);
std::set<sid_t> move(machine m, std::set<sid_t> state_set, char in);
machine to_dfa(machine &nfa);
machine minimize_dfa(machine &dfa);
}

#endif
