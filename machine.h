//
// Created by hesham on 3/18/18.
// Initial definion to the Machine strcture
//

#ifndef KOMPILAR_MACHINE_H
#define KOMPILAR_MACHINE_H

#include <map>
#include <vector>
#include <set>

#define EPS 0xDE

class machine {

 private:
  std::string machine_identifier;
  std::set<int> accepting;
  std::set<char> language;
  int starting;

  class state {
   private:
    std::string token_class;
    std::string key;

    class transition {
     private:
      int to_identifier;
      char on_input;
     public:
      transition (int, char);
      int get_to_identifier () const;
      char get_transition_char () const;
    };
    std::map<char, std::vector<transition> > transitions;
   public:
    state (std::string _token_class, std::string key);
    std::string get_token_class () const;
    bool add_new_transition (int to_id, char on_input = EPS);
    std::string get_key () const;

  };
  std::vector<state> states;

 public:
  machine (std::string _machine_identifier);

  int
  add_new_state (std::string key, std::string token_class = "", bool is_starting = false, bool is_accepting = false);

  bool add_new_transition (int from_id, int to_id, char on_input = EPS);

  bool set_starting_state (int _starting_id);
 
  int add_starting_state (std::string token_class = "", bool is_accepting = false);

  int get_starting_state () const;

  std::string get_machine_identifier () const;

  std::set<int> get_accepting_states () const;

  std::vector<int> get_transitions_for (int id) const;

  std::string get_token_class (int id) const;

  std::string get_key_for (int id) const;
 
  std::string set_key_for (int id, std::string key);

  std::set<char> get_language () const;
 
  int get_states_count () const;

  void print_machine ();
};


#endif //KOMPILAR_MACHINE_H