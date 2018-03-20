//
// Created by Marwan Tammam on 3/20/18.
//

#include "rexplib.h"
#include "machine.h"

machine machine_concat(const machine &a_, const machine &b_) {
    machine a = new machine(a_);
    machine b = new machine(b_);

    for (state &s: a.get_accepting_states()) {
        s.set_accepting(false);
        s.add_new_transition(b.get_starting_state());
    }
    for (state &s: b.get_states()) {
        a.add_new_state(s);
    }
    return a;
}

machine machine_or(const machine &a_, const machine &b_) {
    machine a = new machine(a_);
    machine b = new machine(b_);

    state starting_state = new state();
    starting_state.set_accepting(false);
    starting_state.add_new_transition(a.get_starting_state());
    starting_state.add_new_transition(b.get_starting_state());

    state ending_state = new state();
    starting_state.set_accepting(true);

    for (state &s : a.get_accepting_states()) {
        s.set_accepting(false);
        s.add_new_transition(ending_state);
    }
    for (state &s : b.get_accepting_states()) {
        s.set_accepting(false);
        s.add_new_transition(ending_state);
    }

    for (state &s: b.get_states()) {
        a.add_new_state(s);
    }

    a.add_new_state(starting_state);
    a.add_new_state(ending_state);
    a.set_starting_state(starting_state);
    return a;
}

machine machine_star(const machine &a_) {
    machine a = new machine(a_);

    state starting_state = new state();
    starting_state.set_accepting(false);
    state ending_state = new state();
    starting_state.set_accepting(true);

    starting_state.add_new_transition(ending_state);
    starting_state.add_new_transition(a.get_starting_state());

    for (state &s: a.get_accepting_states()) {
        s.set_accepting(false);
        s.add_new_transition(starting_state);
        s.add_new_transition(ending_state);
    }

    a.add_new_state(starting_state);
    a.add_new_state(ending_state);
    a.set_starting_state(starting_state);

    return a;
}

machine machine_plus(const machine &a_) {
    return machine_concat(a_, machine_star(a_));
}

machine single_char(char c) {
    machine m = new machine(std::string(c));

    state starting_state = new state();
    starting_state.set_accepting(false);
    state ending_state = new state();
    starting_state.set_accepting(true);

    starting_state.add_new_transition(ending_state, c);
    m.add_new_state(starting_state);
    m.add_new_state(ending_state);
    m.set_starting_state(starting_state);

    return m;
}

machine char_range(char start, char end) {
    machine m = single_char(start);
    for (char c = start + 1; c <= end; c++) {
        machine p = single_char(c);
        m = machine_or(m, p);
    }
    return m;
}

machine string_concat(std::string s) {
    if (s == "")
        return single_char(EPS);

    machine m = single_char(s[0]);
    for (int i = 1; i < s.length(); i++) {
        machine p = single_char(s[i]);
        m = machine_concat(m, p);
    }
    return m;
}
