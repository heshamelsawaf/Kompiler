#include "dfa.h"
#include <stack>
#include <iostream>
#include <unordered_map>

typedef int sid_t;

std::string get_key(std::set<sid_t> &states);

void dfa::epsilon_closure(machine m, sid_t s, std::set<sid_t> &res)
{
    std::stack<sid_t> stack;
    // std::cout << "14-Eps Closure For: " << s << std::endl;
    res.insert(s);

    stack.push(s);
    while (!stack.empty()) {
        sid_t u = stack.top();
        stack.pop();
        std::vector<sid_t> eps_trans =
            m.get_transitions(u, EPS_CHAR);
        for (sid_t t : eps_trans) {
            stack.push(t);
            // std::cout << "25-Eps Trans For: " << u << " is: " << t << std::endl; 
            res.insert(t);
        }
    }
}

void dfa::epsilon_closure(machine m, std::set<sid_t> state_set, std::set<sid_t> &res)
{
    res.insert(state_set.begin(), state_set.end());
    // std::cout << get_key(res) << std::endl;
    for (sid_t s : state_set) {
        std::set<sid_t> eps_set;
        epsilon_closure(m, s, eps_set);

        res.insert(eps_set.begin(), eps_set.end());
    }
}

void dfa::move(machine m, sid_t s, std::set<sid_t> &res, char in)
{
    // std::set<sid_t> res;

    std::vector<sid_t> in_trans = m.get_transitions(s, in);
    for (sid_t t : in_trans) {
        res.insert(t);
    }
}

void dfa::move(machine m, std::set<sid_t> state_set, std::set<sid_t> &res, char in)
{
    // std::set<sid_t> res;
    // std::cout << "58-" << get_key(state_set) << std::endl;
    for (sid_t s : state_set) {
        std::set<sid_t> ch_set;
        dfa::move(m, s, ch_set, in);

        res.insert(ch_set.begin(), ch_set.end());
    }
    // std::cout << "62-" << get_key(res) << std::endl;
}

std::string get_key(std::set<sid_t> &states) {
    std::string key = "";
    if (states.empty())
        return key;
    for (sid_t s : states) {
        key += "" + std::to_string(s) + ",";
    }
    key.pop_back();
    return key;
}

void get_states(std::string key, std::set<sid_t> &res) {
    if (key.empty())
        return;
    int state = 0;
    for (char i : res) {
        if (i != ',') {
            state = state * 10 + (i - '0');
        } else {
            res.insert(state);
            state = 0;
        }
    }
    res.insert(state);
}

std::string get_token_type(std::set<sid_t> &states, machine &fa, bool &is_final) {
    for (sid_t s : states) {
        // if (fa.is_accepting(s)) {
        //     is_final = true;
        //     return fa.get_token_class(s);
        // }
        if (fa.get_token_class(s) != "") {
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
    std::set<sid_t> cur_states;
    epsilon_closure(nfa, nfa.get_starting_state(), cur_states);
    std::vector<std::set<sid_t> > states_vec;
    std::set<sid_t> dfa_states;
    states_vec.push_back(cur_states);
    machine dfa_machine("dfa");
    bool is_final = false;
    std::string token_class = get_token_type(cur_states, nfa, is_final);
    sid_t starting_id = dfa_machine.add_new_state(token_class, true, is_final);
    unmarked_states.push_back(starting_id);
    std::unordered_map<std::string, sid_t> keys;
    // std::cout << "100-Start " << get_key(cur_states) << std::endl; 
    dfa_machine.set_key_for(starting_id, get_key(cur_states));
    // std::cout << "Count: " << nfa.get_states_count() << "Inputs: " << nfa.get_language().size() << std::endl;
    // std::cout << get_key(cur_states) << std::endl;
    // return dfa_machine;
    int t = 0;
    while (!unmarked_states.empty()) {
        sid_t cur = unmarked_states.back();
        unmarked_states.pop_back();
        cur_states = states_vec.back();
        // std::cout << get_key(cur_states) << std::endl;
        states_vec.pop_back();
        for (char input : nfa.get_language()) {
            if (input == EPS_CHAR)
                continue;
            std::set<sid_t> temp;
            dfa::move(nfa, cur_states, temp, input);
            // std::cout << "118-" << get_key(temp) << std::endl;
            std::set<sid_t> eps;
            dfa::epsilon_closure(nfa, temp, eps);
            if (eps.empty())
                continue;
            // std::cout << "121-Move Size: " << temp.size() << "Eps Size" << eps.size() << std::endl;
            int found_state = -1;
            std::string new_key = get_key(eps);
            if (keys.find(new_key) != keys.end()) {
                found_state = keys[new_key];
            }
            // for (sid_t s = 1 ; s <= dfa_machine.get_states_count() ; s++) {
            //     if (dfa_machine.get_key_for(s) == new_key) {
            //         found_state = s;
            //         break;
            //     }
            // }

            if (found_state == -1) {
                bool is_final = false;
                std::string token_type = get_token_type(eps, nfa, is_final);
                sid_t new_dfa_state = dfa_machine.add_new_state(new_key, token_type, false, is_final);
                unmarked_states.push_back(new_dfa_state);
                states_vec.push_back(eps);
                dfa_machine.add_new_transition(cur, new_dfa_state, input);
                keys[new_key] = new_dfa_state;
                // std::cout << (++t) << ": " <<  new_key << std::endl;
            } else {
                dfa_machine.add_new_transition(cur, found_state, input);
            }
        }
    }
    return dfa_machine;
}

void print_partitions(std::vector<std::vector<sid_t> > cur_set) {
    for (std::vector<sid_t> v : cur_set) {
        for (sid_t s : v) {
            std::cout << s << ' ';
        }
        std::cout << '\n';
    }
}

void print_partitions(std::vector<int> state_partitions) {
    for (int i = 0 ; i < state_partitions.size() ; i++) {
        std::cout << (i+1) << ": " << state_partitions[i] << std::endl;
    }
}

inline int find_set(std::vector<int> &state_partitions, sid_t state) {
    // print_partitions(state_partitions);
    return state_partitions[state - 1];
}

bool same_partition(machine &dfa, sid_t a, sid_t b, std::vector<int> &state_partitions) {
    for (char input : dfa.get_language()) {
        std::vector<sid_t> a_transitions = dfa.get_transitions(a, input);
        std::vector<sid_t> b_transitions = dfa.get_transitions(b, input);
        if (b_transitions.size() > 1 || a_transitions.size() > 1) {
            std::cerr << "Error: more than 1 transition over the same symbol in DFA" << std::endl;
            return false;
        }
        if (a_transitions.size() != b_transitions.size()) {
            return false;
        }
        if (a_transitions.size() == 1 && b_transitions.size() == 1) {
            if (find_set(state_partitions, a_transitions[0]) != find_set(state_partitions, b_transitions[0]))
                return false;
        }
    }
    return true;
}

void refine(machine &dfa, const std::vector<std::vector<sid_t> > sets,
 std::vector<std::vector<sid_t> > &new_sets, std::vector<int> &state_partitions) {
    int count = 0;
    std::vector<int> prev_partitions(state_partitions);
    for (std::vector<sid_t> working_set : sets) {
        while (!working_set.empty()) {
            sid_t s = working_set.front();
            std::vector<sid_t> new_set;
            new_set.push_back(s);
            state_partitions[s - 1] = count;
            std::vector<sid_t> new_working_set;
            for (int i = 1 ; i < working_set.size() ; i++) {
                sid_t q = working_set[i];
                if (same_partition(dfa, s, q, prev_partitions)) {
                    new_set.push_back(q);
                    state_partitions[q - 1] = count;
                } else {
                    new_working_set.push_back(q);
                }
            }
            working_set = new_working_set;
            // std::cout << "REF" << _t << ": " << new_set.size() << std::endl;
            new_sets.push_back(new_set);
            count++;
        }
    }
}

machine build_dfa(machine &org_dfa, std::vector<std::vector<sid_t> > sets,
 std::vector<int> &state_partitions, sid_t starting_state) {
    machine dfa("min_dfa");
    if (sets.size() == 0)
        return dfa;
    int starting_set = find_set(state_partitions, starting_state);
    bool is_final = false;
    std::string token_type = get_token_type(sets[starting_set], org_dfa, is_final);
    dfa.add_new_state(token_type, true, is_final);

    for (int i = 0 ; i < sets.size() ; i++) {
        if (i != starting_set) {
            token_type = get_token_type(sets[i], org_dfa, is_final);
            dfa.add_new_state(token_type, false, is_final);
        }
    }
    

    for (int i = 0 ; i < sets.size() ; i++) {
        std::vector<sid_t> set = sets[i];
        for (char input : org_dfa.get_language()) {
            for (sid_t state : set) {
                std::vector<sid_t> transitions = org_dfa.get_transitions(state, input);
                if (transitions.empty())
                    continue;
                int to = find_set(state_partitions, transitions[0]);
                if (to == -1) {
                    std::cerr << "Internal Error: target can't be -1" << std::endl;
                    return dfa;
                }
                dfa.add_new_transition(i + 1, to + 1, input);
                break;
            }
        }
    }
    return dfa;
}

machine dfa::minimize_dfa(machine& dfa) {
    std::vector<std::vector<sid_t> > cur_set;
    sid_t starting_state = dfa.get_starting_state();
    std::unordered_map<std::string, int> tokenIdx;
    int count = 0;
    std::vector<int> state_partitions(dfa.get_states_count()); 
    for (sid_t s = 1 ; s <= dfa.get_states_count() ; s++) {
        std::string token_type = dfa.get_token_class(s);
        // std::cout << s << ": " << token_type << std::endl;
        // TODO: remove this;
        if (tokenIdx.find(token_type) == tokenIdx.end()) {
            state_partitions[s - 1] = count;
            tokenIdx[token_type] = count++;
            std::vector<sid_t> temp;
            temp.push_back(s);
            cur_set.push_back(temp);
        } else {
            cur_set[tokenIdx[token_type]].push_back(s);
            state_partitions[s - 1] = tokenIdx[token_type];
        }
    }
    
    // std::cout << "248:" << std::endl;
    // print_partitions(state_partitions);
    // std::cout << "----------------------\n";
    while (true) {
        std::vector<std::vector<sid_t> > new_set;
        refine(dfa, cur_set, new_set, state_partitions);
        // print_partitions(state_partitions);
    // std::cout << "----------------------\n";
        if (new_set.size() == cur_set.size())
            break;
        cur_set = new_set;
        // std::cout << "267: \n";
    }
    return build_dfa(dfa, cur_set, state_partitions, starting_state);
}
