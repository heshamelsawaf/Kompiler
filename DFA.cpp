#include "DFA.h"
#include "machine.h"
#include <stack>
#include <iostream>
#define EPS 0xDE

typedef int sid_t;

std::set<sid_t> dfa::epsilon_closure(machine m, sid_t s)
{
    std::set<sid_t> res;
    std::stack<sid_t> stack;

    res.insert(s);

    stack.push(s);
    while (!stack.empty()) {
        state u = stack.top();
        stack.pop();
        std::vector<state::transition> eps_trans =
            m.get_transitions_for(s, EPS);
        for (state::transition t : eps_trans) {
            sid_t v = t.get_to_identifier();
            stack.push(v);

            res.insert(v);
        }
    }
    return res;
}

std::set<sid_t> dfa::epsilon_closure(machine m,, std::set<sid_t> state_set)
{
    std::set<sid_t> res = state_set;

    for (sid_t s : state_set) {
        std::set<sid_t> eps_set = epsilon_closure(m, s);

        res.insert(eps_set.begin(), eps_set.end());
    }
    return res;
}

std::set<sid_t> dfa::move(machine m, sid_t s, char in)
{
    std::set<sid_t> res;

    std::vector<state::transition> in_trans = m.get_transitions_for(s, in);
    for (state::transition t : in_trans) {
        sid_t u = t.get_to_identifier();
        res.insert(u);
    }
    return res;
}

std::set<sid_t> dfa::move(machine m, std::set<sid_t> state_set, char in)
{
    std::set<sid_t> res = state_set;

    for (sid_t s : state_set) {
        std::set<sid_t> ch_set = dfa::move(s, in);

        res.insert(ch_set.begin(), ch_set.end());
    }
    return res;
}

std::string get_key(std::set<int> &s) {
    return "";
}

std::string get_token_type(std::set<int> &s, machine &nfa, bool &is_final) {

    return "";
}

machine* dfa::to_dfa(machine &nfa) {
    std::vector<sid_t> unmarked_states;
    std::set<sid_t> cur_states = epsilon_closure(nfa.get_starting_state());
    std::set<sid_t> dfa_states;
    machine dfa_machine("dfa");
    sid_t starting_id = dfa_machine.add_starting_state();
    unmarked_states.push_back(starting_id);
    dfa_machine.set_key_for(starting_id, "" + starting_id);
    while (!unmarked_states.empty()) {
        sid_t cur = unmarked_states.back();
        unmarked_states.pop_back();
        for (char input : nfa.get_language()) {
            std::set<sid_t> temp = dfa::move(cur_states, input);
            cur_states = dfa::epsilon_closure(temp);
            int found_state = -1;
            std::string new_key = get_key(cur_states);

            for (sid_t s = 0 ; s < dfa_machine.get_states_count() ; s++) {
                if (dfa_machine.get_key_for(s) == new_key)
                    found_state = s;
            }

            if (found_state == -1) {
                bool is_final = false;
                std::string token_type = get_token_type(cur_states, nfa, is_final);
                sid_t new_dfa_state = dfa_machine.add_new_state(token_type, is_final);
                unmarked_states.push_back(new_dfa_state);
                dfa_machine.add_new_transition(cur, new_dfa_state, input);
            } else {
                dfa_machine.add_new_transition(cur, found_state, input);
            }
        }
    }
    return &dfa_machine;
}

bool same_partition(const machine &dfa, const state &a, const state &b) {
    for (char input : dfa.get_inputs()) {
        std::vector<state::transition> a_transitions = a.get_transitions_for(input);
        std::vector<state::transition> b_transitions = b.get_transitions_for(input);
        if (b_transitions.size() > 1 || a_transitions.size() > 1) {
            std::cerr << "Error: more than 1 transition over the same symbol in DFA" << std::endl;
            return false;
        }
        if (b_transitions.size() != a_transitions.size())
            return false;
        if (a_transitions.size() == 1) {
            if (a_transitions[0] != b_transitions[0])
                return false;
        }
    }
    return true;
}

void refine(const machine &dfa, const std::vector<std::vector<sid_t> > &sets, std::vector<std::vector<sid_t> > new_sets) {
    for (std::vector<sid_t> states : sets) {
        std::vector<sid_t> working_set(states);
        while (working_set.size() > 0) {
            sid_t s = working_set.front();
            working_set.erase(working_set.begin());
            std::vector<sid_t> new_set;
            new_set.push_back(s);
            for (sid_t q : working_set) {
                if (same_partition(dfa, s, q)) {
                    new_set.push_back(q);
                }
            }
            new_sets.push_back(new_set);
        }
    }
}

machine* build_dfa(std::vector<std::vector<sid_t> > sets) {
    machine dfa("min_dfa");
    if (sets.size() == 0)
        return &dfa;
    std::vector<std::set<state> > sets_vec(sets.begin(), sets.end());
    state starting_state(0, sets_vec[0]);
    dfa.set_starting_state(&starting_state);
    for (int i = 1 ; i < sets_vec.size() ; i++) {
        state cur_state(i, sets_vec[i]);
        dfa.add_new_state(cur_state);
    }
    return &dfa;
}

machine* dfa::minimize_dfa(machine& dfa) {
    std::vector<std::vector<sid_t> > cur_set;
    std::vector<sid_t> accepting;
    std::vector<sid_t> non_accepting;
    for (sid_t s = 0 ; s < dfa.get_states_count() ; s++) {
        if (dfa.is_accepting(s)) {
            accepting.push_back(s);
        } else {
            non_accepting.push_back(s);
        }
    }
    cur_set.push_back(accepting);
    cur_set.push_back(non_accepting);

    while (true) {
        std::vector<std::vector<sid_t> > new_set;
        refine(dfa, cur_set, new_set);
        if (new_set.size() == cur_set.size())
            break;
        cur_set = new_set;
    }
    return build_dfa(cur_set);
}
