//
// Created by Marwan Tammam on 3/21/18.
//

#include "machine.h"

void state::transition::get_end_state() const {
    return;
}

void state::transition::get_transition_char() const {
    return;
}


int state::get_state_identifier() const {
    return 0;
}

std::string state::get_token_class() const {
    return NULL;
}

bool state::is_accepting() const {
    return false;
}

void state::set_accepting(bool) {
    return;
}

std::vector<state::transition> state::get_transitions_for(char in) const {
    return std::vector<state::transition>();
}

void state::add_new_transition(state to, char on_input) {
    return;
}


machine::machine(std::string _machine_identifier) {
    return;
}

machine::machine(const machine &copy) {
    return;
}

void machine::add_new_state(state) {
    return;
}

void machine::add_new_transition(state from, state to, char on_input) {
    return;
}

void machine::set_starting_state(state *_starting) {
    return;
}

state machine::get_starting_state() const {
    return state();
}

std::string machine::get_machine_identifier() const {
    return NULL;
}

std::vector<state> machine::get_states() const {
    return std::vector<state>();
}

std::vector<state> machine::get_accepting_states() const {
    return std::vector<state>();
}
