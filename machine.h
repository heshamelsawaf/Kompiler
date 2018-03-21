//
// Created by hesham on 3/18/18.
// Initial definion to the Machine strcture
//

#ifndef KOMPILER_MACHINE_H
#define KOMPILER_MACHINE_H

#include <string>
#include <vector>
#include <map>
#define EPS 0xDE

class state {

  class transition {
   private:
    state *to;
    char on_input;
   public:
    state get_end_state () const;
    void get_transition_char () const;
  };
 private:
  int state_identifier;
  std::string token_class;
  bool _is_accepting;
  std::map<char, std::vector<transition> > transitions;

  inline bool operator < (const state& lhs, const state& rhs)
  {
    return lhs.state_identifier < rhs.state_identifier;
  }

 public:
  int get_state_identifier () const;
  std::string get_token_class () const;
  bool is_accepting () const;
  void set_accepting (bool);
  std::vector<transition> get_transitions_for (char in) const;
  /*
   * Adds a new transition from this state to TO state.
   */
  void add_new_transition (state to, char on_input = EPS);  // 0xDE = Epsilon
};

class machine {

 private:
  std::string machine_identifier;
  state *starting;
  std::vector<state> states;

 public:
  machine (std::string _machine_identifier);
  machine (const machine& copy);
//  void add_new_state (int state_identifier, std::string token_class, bool is_accepting = false);
  void add_new_state (state);
  void add_new_transition (state from, state to, char on_input = EPS);
  void set_starting_state (state *_starting);
  state get_starting_state () const;
  std::string get_machine_identifier () const;
  std::vector<state> get_states () const;
  std::vector<state> get_accepting_states () const;
};

#endif //KOMPILER_MACHINE_H
