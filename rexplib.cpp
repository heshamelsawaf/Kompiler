//
// Created by Marwan Tammam on 3/20/18.
//

#include "rexplib.h"
#include <iostream>

machine machine_ops::concat(std::vector<machine> &machines) {
    if (machines.empty()) {
        throw std::invalid_argument("received 0 machines to concatenate");
    }
    if ((int) machines.size() == 1) {
        return machines[0];
    }

    machine m = machines[0];

    for (int i = 1; i < (int) machines.size(); i++) {
        std::set<int> accepting = m.get_accepting_states();
        m.clear_accepting_states();

        int s = m.merge(machines[i]);
        for (int a : accepting) {
            m.add_new_transition(a, s);
        }
    }

    return m;
}

machine machine_ops::concat(const machine &a, const machine &b) {
    std::vector<machine> temp = {a, b};
    return machine_ops::concat(temp);
}

machine machine_ops::oring(std::vector<machine> &machines) {
    if (machines.empty()) {
        throw std::invalid_argument("received 0 machines to or");
    }
    if ((int) machines.size() == 1) {
        return machines[0];
    }

    machine m = machine(machines[0].get_machine_identifier());

    int starting = m.add_new_state(true, false);
    int ending = m.add_new_state(false, true);

    for (int i = 0; i < (int) machines.size(); i++) {
        int s = m.merge(machines[i]);
        m.add_new_transition(starting, s);
    }

    for (int a : m.get_accepting_states()) {
        if (a == ending) continue;
        m.add_new_transition(a, ending);
    }
    return m;
}

machine machine_ops::oring(const machine &a, const machine &b) {
    std::vector<machine> temp = {a, b};
    return machine_ops::oring(temp);
}

machine machine_ops::star(const machine &a_) {
    machine m = a_;

    int starting = m.add_new_state(false, false);
    int ending = m.add_new_state(false, false);

    m.add_new_transition(starting, ending);
    m.add_new_transition(starting, m.get_starting_state());
    m.set_starting_state(starting);

    for (int a : m.get_accepting_states()) {
        m.add_new_transition(a, starting);
        m.add_new_transition(a, ending);
    }
    m.clear_accepting_states();
    m.set_accepting(ending);

    return m;
}

machine machine_ops::plus(const machine &a_) {
    return machine_ops::concat(a_, machine_ops::star(a_));
}

machine machine_ops::single_char(char c) {
    machine m = machine(std::string(1, c));

    int starting = m.add_new_state(true, false);
    int ending = m.add_new_state(false, true);

    m.add_new_transition(starting, ending, c);
    return m;
}

machine machine_ops::char_range(char start, char end) {
    std::vector<machine> m;
    for (char c = start; c <= end; c++) {
        m.push_back(single_char(c));
    }
    return machine_ops::oring(m);
}

machine machine_ops::string_concat(std::string s) {
    if (s == "")
        return single_char(EPS);

    std::vector<machine> m;
    for (int i = 0; i < s.length(); i++) {
        m.push_back(single_char(s[i]));
    }
    return machine_ops::concat(m);
}
