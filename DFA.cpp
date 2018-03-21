#include "DFA.h"
#include "machine.h"
#define EPS 0xDE

std::set<state> epsilon_closure(state s)
{
    std::set<state> res;
    std::stack<state> stack;

    res.insert(s);

    stack.push(s);
    while (!stack.empty()) {
        state u = stack.pop();
        std::vector<transition> eps_trans = u.get_transitions_for(EPS);
        for (transition t : eps_trans) {
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

    for (auto i : state_set) {
        state s = *i;
        std::set eps_set = epsilon_closure(s);

        res.insert(eps_set.begin(), eps_set.end());
    }

    return res;
}

std::set<state> dfa::move(state s, char in)
{
    std::set<state> res;

    std::vector<transition> in_trans = s.get_transition_for(in);
    for (transition t : in_trans) {
        state u = t.get_end_state();
        res.insert(u);
    }

    return res;
}

std::set<state> dfa::move(set<state> state_set, char in)
{
    st::set<state> res = state_set;

    for (auto i : state_set) {
        state s = *i;
        std::set ch_set = dfa::move(s, in);

        res.insert(ch_set.begin(), ch_set.end());
    }

    return res;
}
