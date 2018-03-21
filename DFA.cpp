#include "DFA.h"
#include "machine.h"
#define EPS 0xDE

std::set<state> epsilon_closure (state state)
{
    std::set<state> res;
    std::stack<state> stack;

    res.insert (state);

    stack.push (state);
    while (!stack.empty ()) {
        state s = stack.pop ();

        std::vector<transition> eps_trans = s.get_transitions_for (EPS);
        for (int i = 0; i < eps_trans.size (); i++) {
            state t = eps_trans[i].get_end_state ();
            res.insert (t);

            stack.push (t);
        }
    }

    return res;
}

std::set<state> epsilon_closure (std::set<state> state_set)
{
    std::set<state> res = state_set;

    for (auto i : state_set) {
        state s = *i;
        std::set eps_set = epsilon_closure (s);

        res.insert (eps_set.begin (), eps_set.end ());
    }
    return res;
}
