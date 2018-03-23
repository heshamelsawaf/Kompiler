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

machine::state::state(std::string _token_class) {
    token_class = _token_class;
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

void machine::state::set_key(std::string _key) {
    key = _key;
}

void machine::state::set_token_class(std::string new_token_class) {
    token_class = new_token_class;
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
    starting = -1;
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

int machine::add_new_state(std::string token_class, bool is_starting, bool is_accepting) {
    machine::state s = machine::state(token_class);
    states.push_back(s);
    if (is_accepting) {
        accepting.insert((int) states.size());
    }
    int id = (int) states.size();
    if (is_starting) {
        starting = id;
    }
    s.set_key("" + id);

    return id;
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
void machine::set_machine_identifier (std::string identifier)
{
    machine_identifier = identifier;
}

void machine::set_key_for(sid_t id, std::string new_key) {
    states[id - 1].set_key(new_key);
}

bool machine::set_token_class(sid_t id, std::string new_token_class) {
  if (id < 1 && id > states.size())
    return false;
  states[id - 1].set_token_class(new_token_class);
  return true;
}

bool machine::set_token_class(std::string new_token_class) {
    for (auto i = accepting.begin (); i != accepting.end (); i++)
      states[(*i)-1].set_token_class (new_token_class);
    return true;
}

std::set<char> machine::get_language() const {
    return language;
}

int machine::get_states_count() const {
    return (int) states.size();
}

bool machine::is_starting(int id) {
    return starting == id;
}

bool machine::is_accepting(int id) {
    return accepting.count(id) != 0;
}

void machine::clear_accepting_states() {
    accepting.clear();
}

bool machine::set_accepting(int id) {
    if (id - 1 >= get_states_count()) {
        return false;
    }
    accepting.insert(id);
    return true;
}

int machine::merge(machine other) {
    int starting_other = other.get_starting_state();
    int old_cnt = get_states_count();
    for (int i = 1; i <= other.get_states_count(); i++) {
        add_new_state(other.get_key_for(i), other.get_token_class(i), false, other.is_accepting(i));
    }
    for (int i = 1; i <= other.get_states_count(); i++) {
        for (char c : other.get_language()) {
            std::vector<int> t = other.get_transitions(i, c);
            for (int id : t) {
                add_new_transition(i + old_cnt, id + old_cnt, c);
            }
        }
    }
    return starting_other + old_cnt;
}

void machine::print_machine() {
  using namespace std;

  // cout << "Machine ID: " << machine_identifier << endl;
  // cout << "States Cnt: " << get_states_count() << endl;

  cout << "digraph {\n";

  cout << "node [shape = doublecircle]; ";
  for (int i : get_accepting_states())
    cout << i << " ";
  cout << ";\n";

  cout << "node [shape = point ]; qi;\n";
  cout << "node [shape = circle]; \n";

  cout << "qi -> " << get_starting_state() << ";\n";
  for (int i = 1; i <= states.size(); i++) {
      for (char c : language) {
          vector<int> v = get_transitions(i, c);
          for (auto to : v) {
              cout << i << " -> " << to << ' ';
              if (c == EPS)
                cout << "[label=\"ε\"];";
              else
                cout << "[label=\"" << c << "\"];";
              cout << endl;
            }
        }
    }
  cout << endl << "}";

}