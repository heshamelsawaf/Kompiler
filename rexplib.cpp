//
// Created by Marwan Tammam on 3/20/18.
//

#include "rexplib.h"

machine machine_ops::concat(std::vector<const machine> &machines_) {
    if (machines_.empty()) {
        throw std::invalid_argument("received 0 machines to concatenate");
    }
    if ((int) machines_.size() == 1) {
        return machines_[0];
    }

    std::vector<machine> machines;
    for (int i = 0; i < (int) machines.size(); i++) {
        machines.push_back(machine(machines[i]));
    }

    machine m = machines[0];
    for (int i = 0; i < (int) machines.size() - 1; i++) {
        for (state &s: machines[i].get_accepting_states()) {
            s.set_accepting(false);
            s.add_new_transition(machines[i + 1].get_starting_state());
        }
    }

    for (int i = 1; i < (int) machines.size(); i++) {
        for (state &s : machines[i].get_states()) {
            m.add_new_state(s);
        }
    }

    return m;
}

machine machine_ops::concat(const machine &a, const machine &b) {
    std::vector<const machine> temp = {a, b};
    return machine_ops::concat(temp);
}

machine machine_ops::oring(std::vector<const machine> &machines_) {
    if (machines_.empty()) {
        throw std::invalid_argument("received 0 machines to or");
    }
    if ((int) machines_.size() == 1) {
        return machines_[0];
    }

    std::vector<machine> machines;
    for (int i = 0; i < (int) machines.size(); i++) {
        machines.push_back(machine(machines[i]));
    }

    machine m = machines[0];

    state starting_state = state();
    starting_state.set_accepting(false);
    for (int i = 0; i < (int) machines.size(); i++) {
        starting_state.add_new_transition(machines[i].get_starting_state());
    }
    m.add_new_state(starting_state);

    state ending_state = state();
    starting_state.set_accepting(true);
    for (int i = 0; i < (int) machines.size(); i++) {
        for (state &s : machines[i].get_accepting_states()) {
            s.set_accepting(false);
            s.add_new_transition(ending_state);
        }
    }
    m.add_new_state(ending_state);

    for (int i = 1; i < (int) machines.size(); i++) {
        for (state &s : machines[i].get_states()) {
            m.add_new_state(s);
        }
    }

    m.set_starting_state(&starting_state);
    return m;
}

machine machine_ops::oring(const machine &a, const machine &b) {
    std::vector<const machine> temp = {a, b};
    return machine_ops::oring(temp);
}

machine machine_ops::star(const machine &a_) {
    machine a = machine(a_);

    state starting_state = state();
    starting_state.set_accepting(false);
    state ending_state = state();
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
    a.set_starting_state(&starting_state);

    return a;
}

machine machine_ops::plus(const machine &a_) {
    return machine_ops::concat(a_, machine_ops::star(a_));
}

machine machine_ops::single_char(char c) {
    machine m = machine(std::string(1, c));

    state starting_state = state();
    starting_state.set_accepting(false);
    state ending_state = state();
    starting_state.set_accepting(true);

    starting_state.add_new_transition(ending_state, c);
    m.add_new_state(starting_state);
    m.add_new_state(ending_state);
    m.set_starting_state(&starting_state);

    return m;
}

machine machine_ops::char_range(char start, char end) {
    machine m = single_char(start);
    for (char c = start + 1; c <= end; c++) {
        machine p = single_char(c);
        m = machine_ops::oring(m, p);
    }
    return m;
}

machine machine_ops::string_concat(std::string s) {
    if (s == "")
        return single_char(EPS);

    machine m = single_char(s[0]);
    for (int i = 1; i < s.length(); i++) {
        machine p = single_char(s[i]);
        m = machine_ops::concat(m, p);
    }
    return m;
}

