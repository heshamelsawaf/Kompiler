#include "DFA.h"
#include "machine.h"
#include <stack>
#define EPS 0xDE

std::set<state> epsilon_closure(state s)
{
    std::set<state> res;
    std::stack<state> stack;

    res.insert(s);

    stack.push(s);
    while (!stack.empty()) {
        state u = stack.top();
        stack.pop();
        std::vector<state::transition> eps_trans = u.get_transitions_for(EPS);
        for (state::transition t : eps_trans) {
            state v = t.get_end_state();
            stack.push(v);

            res.insert(v);
        }
    }

    return res;
}

std::set<state> epsilon_closure(std::set<state> state_set)
{
    std::set<state> res = state_set;

    for (state i : state_set) {
        state s = i;
        std::set<state> eps_set = epsilon_closure(s);

        res.insert(eps_set.begin(), eps_set.end());
    }

    return res;
}

std::set<state> dfa::move(state s, char in)
{
    std::set<state> res;

    std::vector<state::transition> in_trans = s.get_transitions_for(in);
    for (state::transition t : in_trans) {
        state u = t.get_end_state();
        res.insert(u);
    }

    return res;
}

std::set<state> dfa::move(std::set<state> state_set, char in)
{
    std::set<state> res = state_set;

    for (state i : state_set) {
        state s = i;
        std::set<state> ch_set = dfa::move(s, in);

        res.insert(ch_set.begin(), ch_set.end());
    }

    return res;
}

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
