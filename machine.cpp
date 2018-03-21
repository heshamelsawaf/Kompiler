//
// Created by Marwan Tammam on 3/21/18.
//

#include "machine.h"

state::transition::transition(const transition &copy) {
    this->on_input = copy.get_transition_char();
    this->to = new state(*copy.get_end_state());
}

state::transition::transition(state *to, char on_input) {
    this->to = to;
    this->on_input = on_input;
}

state *state::transition::get_end_state() const {
    return this->to;
}

char state::transition::get_transition_char() const {
    return this->on_input;
}

state::state() {
}

state::state(const state &copy) {
    this->state_identifier = copy.get_state_identifier();
    this->token_class = copy.get_token_class();
    this->_is_accepting = copy.is_accepting();

    for (std::pair<const char, std::vector<transition> > &entry : copy.get_transitions()) {
        char c = entry.first;
        this->transitions[c] = std::vector<transition>();
        for (transition &t : entry.second){
            this->transitions[c].push_back(transition(t));
        }
    }
}

int state::get_state_identifier() const {
    return this->state_identifier;
}

std::string state::get_token_class() const {
    return this->token_class;
}

bool state::is_accepting() const {
    return this->_is_accepting;
}

void state::set_accepting(bool is_accepting) {
    this->_is_accepting = is_accepting;
}

std::vector<state::transition> state::get_transitions_for(char in) {
    return this->transitions[in];
}

std::map<char, std::vector<state::transition> > state::get_transitions() const {
    return this->transitions;
}

void state::add_new_transition(state to, char on_input) {
    if (this->transitions.count(on_input) == 0) {
        this->transitions[on_input] = std::vector<transition>();
    }
    this->transitions[on_input].push_back(transition(&to, on_input));
}


machine::machine(std::string _machine_identifier) {
    this->machine_identifier = _machine_identifier;
    this->states = std::vector<state>();
}

machine::machine(const machine &copy) {
    this->machine_identifier = copy.get_machine_identifier();
    this->starting = new state(copy.get_starting_state());

    for (state &s : copy.get_states()){
        this->states.push_back(state(s));
    }
}

void machine::add_new_state(state s) {
    this->states.push_back(s);
}

void machine::add_new_transition(state from, state to, char on_input) {
    from.add_new_transition(to, on_input);
}

void machine::set_starting_state(state *_starting) {
    this->starting = _starting;
}

state machine::get_starting_state() const {
    return *(this->starting);
}

std::string machine::get_machine_identifier() const {
    return this->machine_identifier;
}

std::vector<state> machine::get_states() const {
    return this->states;
}

std::vector<state> machine::get_accepting_states() const {
    std::vector<state> accepting_states;
    for (const state &s : this->states) {
        if (s.is_accepting()) {
            accepting_states.push_back(s);
        }
    }
    return accepting_states;
}
