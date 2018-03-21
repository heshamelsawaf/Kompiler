#include "DFA.h"

machine* dfa::to_dfa(machine &nfa) {
    std::vector<state> unmarked_states;
    std::set<state> cur_states = epsilon_closure(nfa.get_starting_state);
    std::set<state> dfa_states;
    int count = 0;
    state dfa_start_state(count, cur_states);
    machine dfa_machine("dfa");
    dfa_machine.set_starting_state(&dfa_start_state);
    unmarked_states.push_back(dfa_start_state);
    while (!unmarked_states.empty()) {
        state cur = unmarked_states.back();
        unmarked_states.pop_back();
        for (char input : nfa.get_inputs()) {
            std::set<state> temp = dfa::move(cur_states, input);
            cur_states = dfa::epsilon_closure(temp);
            state new_dfa_state(++count, cur_states);
            bool found = false;
            // TODO: Search in DFA Machine for new_dfa_state
            if (!found) {
                unmarked_states.push_back(new_dfa_state);
                // TODO add new state to DFA Machine
            }
            dfa_machine.add_new_transition(cur, new_dfa_state, input);
        }
    }
    return nullptr;
}
