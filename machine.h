//
// Created by hesham on 3/18/18.
// Initial definion to the Machine strcture
//

#ifndef KOMPILER_MACHINE_H
#define KOMPILER_MACHINE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#define EPS 0xDE

class state {

  class transition {
   private:
    state *to;
    char on_input;
   public:
    void get_end_state () const;
    void get_transition_char () const;
  };
 private:
  int state_identifier;
  std::string token_class;
  bool _is_accepting;
  std::map<char, std::vector<transition> > transitions;
 public:
  state();
  state(int state_identifier, std::string token_class, bool is_accepting);
  state(int state_identifier, std::set<state> states);//constructs a dfa state from nfa states.
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
  std::set<char> inputs;

 public:
  machine (std::string _machine_identifier);
  inline int count_states() {return states.size();}
  machine (const machine& copy);
//  void add_new_state (int state_identifier, std::string token_class, bool is_accepting = false);
  void add_new_state (state);
  void add_new_transition (state from, state to, char on_input = EPS);
  void set_starting_state (state *_starting);
  state get_starting_state () const;
  std::string get_machine_identifier () const;
  std::vector<state> get_states () const;
  std::set<char> get_inputs() const;
  std::vector<state> get_accepting_states () const;
};

#endif //KOMPILER_MACHINE_H
