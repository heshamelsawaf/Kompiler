//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include "machine.h"

machine::state::transition::transition(int to, char input) {
    to_identifier = to;
    on_input = input;
}

int machine::state::transition::get_to_identifier() const {
    return to_identifier;
}

char machine::state::transition::get_transition_char() const {
    return on_input;
}

machine::state::state() {
}

machine::state::state(std::string _token_class, std::string _key) {
    token_class = _token_class;
    key = _key;
}

std::string machine::state::get_token_class() const {
    return token_class;
}

std::string machine::state::get_key() const {
    return key;
}

std::vector<int> machine::state::get_transitions_for(char input) {
    std::vector<int> t;
    for (machine::state::transition &trans : transitions[input]) {
        t.push_back(trans.get_to_identifier());
    }
    return t;
}

bool machine::state::add_new_transition(int to_id, char on_input) {
    if (transitions.count(on_input) == 0) {
        transitions[on_input] = std::vector<machine::state::transition>();
    }
    transitions[on_input].push_back(machine::state::transition(to_id, on_input));
    return true;
}


machine::machine(std::string _machine_identifier) {
    machine_identifier = _machine_identifier;
    language.insert(EPS);
}

int machine::add_new_state(bool is_starting, bool is_accepting) {
    states.push_back(machine::state());
    if (is_accepting) {
        accepting.insert((int) states.size());
    }
    if (is_starting) {
        starting = (int) states.size();
    }
    return (int) states.size();
}

int machine::add_new_state(std::string key, std::string token_class, bool is_starting, bool is_accepting) {
    states.push_back(machine::state(token_class, key));
    if (is_accepting) {
        accepting.insert((int) states.size());
    }
    if (is_starting) {
        starting = (int) states.size();
    }
    return (int) states.size();
}

bool machine::add_new_transition(int from_id, int to_id, char on_input) {
    if (from_id - 1 >= get_states_count())
        return false;
    states[from_id - 1].add_new_transition(to_id, on_input);
    language.insert(on_input);
    return true;
}

bool machine::set_starting_state(int _starting_id) {
    if (_starting_id - 1 >= get_states_count())
        return false;
    starting = _starting_id;
    return true;
}

int machine::add_starting_state(std::string key, std::string token_class, bool is_accepting) {
    return machine::add_new_state(key, token_class, true, is_accepting);
}

int machine::get_starting_state() const {
    return starting;
}

std::string machine::get_machine_identifier() const {
    return machine_identifier;
}

std::set<int> machine::get_accepting_states() const {
    return accepting;
}

std::vector<int> machine::get_transitions(int id, char input) {
    return states[id - 1].get_transitions_for(input);
}

std::string machine::get_token_class(int id) const {
    return states[id - 1].get_token_class();
}

std::string machine::get_key_for(int id) const {
    return states[id - 1].get_key();
}

std::set<char> machine::get_language() const {
    return language;
}

int machine::get_states_count() const {
    return (int) states.size();
}

void machine::print_machine() {
    using namespace std;

    cout << "Machine ID: " << machine_identifier << endl;
    cout << "States Cnt: " << get_states_count() << endl;
    for (int i = 1; i <= states.size(); i++) {
        for (auto c : language) {
            vector<int> v = get_transitions(i, c);
            for (auto to : v) {
                cout << i << ' ' << to << ' ' << c << endl;
            }
        }
    }
    cout << endl;

}

