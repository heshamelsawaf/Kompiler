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
        sid_t u = stack.top();
        stack.pop();
        std::vector<sid_t> eps_trans =
            m.get_transitions_for(u, EPS);
        for (sid_t t : eps_trans) {
            stack.push(t);

            res.insert(t);
        }
    }
    return res;
}

std::set<sid_t> dfa::epsilon_closure(machine m, std::set<sid_t> state_set)
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

    std::vector<sid_t> in_trans = m.get_transitions_for(s, in);
    for (sid_t t : in_trans) {
        res.insert(t);
    }
    return res;
}

std::set<sid_t> dfa::move(machine m, std::set<sid_t> state_set, char in)
{
    std::set<sid_t> res = state_set;

    for (sid_t s : state_set) {
        std::set<sid_t> ch_set = dfa::move(m, s, in);

        res.insert(ch_set.begin(), ch_set.end());
    }
    return res;
}

std::string get_key(std::set<sid_t> &states) {
    std::string key = "";
    for (sid_t s : states)
        key += s + ",";
    key.pop_back();
    return key;
}

std::string get_token_type(std::set<sid_t> &states, machine &fa, bool &is_final) {
    for (sid_t s : states) {
        if (fa.is_accepting(s)) {
            is_final = true;
            return fa.get_token_class(s);
        }
    }
    return "";
}

std::string get_token_type(std::vector<sid_t> &states_vec, machine &fa, bool &is_final) {
    std::set<sid_t> states(states_vec.begin(), states_vec.end());
    return get_token_type(states, fa, is_final);
}

machine dfa::to_dfa(machine &nfa) {
    std::vector<sid_t> unmarked_states;
    std::set<sid_t> cur_states = epsilon_closure(nfa, nfa.get_starting_state());
    std::set<sid_t> dfa_states;
    machine dfa_machine("dfa");
    bool is_final = false;
    std::string token_class = get_token_type(cur_states, nfa, is_final);
    sid_t starting_id = dfa_machine.add_starting_state(token_class, is_final);
    unmarked_states.push_back(starting_id);
    dfa_machine.set_key_for(starting_id, get_key(cur_states));
    while (!unmarked_states.empty()) {
        sid_t cur = unmarked_states.back();
        unmarked_states.pop_back();
        for (char input : nfa.get_language()) {
            std::set<sid_t> temp = dfa::move(nfa, cur_states, input);
            cur_states = dfa::epsilon_closure(nfa, temp);
            int found_state = -1;
            std::string new_key = get_key(cur_states);

            for (sid_t s = 0 ; s < dfa_machine.get_states_count() ; s++) {
                if (dfa_machine.get_key_for(s) == new_key) {
                    found_state = s;
                    break;
                }
            }

            if (found_state == -1) {
                bool is_final = false;
                std::string token_type = get_token_type(cur_states, nfa, is_final);
                sid_t new_dfa_state = dfa_machine.add_new_state(token_type, is_final);
                dfa_machine.set_key_for(new_dfa_state, new_key);
                unmarked_states.push_back(new_dfa_state);
                dfa_machine.add_new_transition(cur, new_dfa_state, input);
            } else {
                dfa_machine.add_new_transition(cur, found_state, input);
            }
        }
    }
    return dfa_machine;
}

int find_set(std::vector<std::vector<sid_t> > sets, sid_t state) {
    for (int i = 0 ; i < sets.size() ; i++) {
        for (sid_t s : sets[i]) {
            if (s == state)
                return i;
        }
    }
    return -1;
}

bool same_partition(const machine &dfa, const sid_t a, const sid_t b, std::vector<std::vector<sid_t> > sets) {
    for (char input : dfa.get_language()) {
        std::vector<sid_t> a_transitions = dfa.get_transitions_for(a, input);
        std::vector<sid_t> b_transitions = dfa.get_transitions_for(b, input);
        if (b_transitions.size() > 1 || a_transitions.size() > 1) {
            std::cerr << "Error: more than 1 transition over the same symbol in DFA" << std::endl;
            return false;
        }
        if (b_transitions.size() != a_transitions.size())
            return false;
        if (a_transitions.size() == 1) {
            
            if (find_set(sets, a_transitions[0]) != find_set(sets, b_transitions[0]))
                return false;
        }
    }
    return true;
}

void refine(const machine &dfa, const std::vector<std::vector<sid_t> > sets, std::vector<std::vector<sid_t> > new_sets) {
    for (std::vector<sid_t> working_set : sets) {
        while (working_set.size() > 0) {
            sid_t s = working_set.front();
            working_set.erase(working_set.begin());
            std::vector<sid_t> new_set;
            new_set.push_back(s);
            for (sid_t q : working_set) {
                if (same_partition(dfa, s, q, sets)) {
                    new_set.push_back(q);
                }
            }
            new_sets.push_back(new_set);
        }
    }
}

machine build_dfa(machine &org_dfa, std::vector<std::vector<sid_t> > sets, sid_t starting_state) {
    machine dfa("min_dfa");
    if (sets.size() == 0)
        return dfa;
    int starting_set = find_set(sets, starting_state);
    bool is_final = false;
    std::string token_type = get_token_type(sets[starting_set], org_dfa, is_final);
    dfa.add_starting_state(token_type, is_final);

    for (int i = 0 ; i < sets.size() ; i++) {
        if (i != starting_set) {
            token_type = get_token_type(sets[i], org_dfa, is_final);
            dfa.add_new_state(token_type, is_final);
        }    
    }
    return dfa;
}

machine dfa::minimize_dfa(machine& dfa) {
    std::vector<std::vector<sid_t> > cur_set;
    std::vector<sid_t> accepting;
    std::vector<sid_t> non_accepting;
    sid_t starting_state = dfa.get_starting_state();
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
    return build_dfa(dfa, cur_set, starting_state);
}
