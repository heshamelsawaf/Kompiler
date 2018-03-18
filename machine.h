//
// Created by hesham on 3/18/18.
// Initial definion to the Machine strcture
//

#ifndef KOMPILER_MACHINE_H
#define KOMPILER_MACHINE_H

#include <string>
#include <vector>
#include <map>

struct state {

  struct transition{
    state* to;
    char on_input;
  };

  int state_identifier;
  std::string token_class;
  bool is_accepting;
  std::vector<transition> transitions;
};

class machine {

 private:
  std::string machine_identifier;
  state* starting;
  std::vector<state> states;

 public:
  machine (std::string _machine_identifier);
  void add_new_state (int state_identifier, std::string token_class, bool is_accepting = false);
  void add_new_transition (state from, state to, char on_input = 0xDE);  // 0xDE = Epsilon
  void set_starting_state (state* _starting);
  state get_starting_state () const;
  std::string get_machine_identifier () const;
  std::vector<state> get_states () const;
};

#endif //KOMPILER_MACHINE_H
