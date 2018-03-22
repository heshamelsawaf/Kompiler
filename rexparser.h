//
// Created by hesham on 3/21/18.
//

#ifndef KOMPILER_REXPARSER_H
#define KOMPILER_REXPARSER_H

#include <string>
#include <stack>
#include "machine.h"

struct machine_stacks{
  std::string identifier;
  bool is_operator;
};

class rexparser {

 private:
  std::map<std::string, machine> machines;
  std::vector<machine> regex;
  void process_line (const std::string line);
  machine handler_regular (const std::string line);
  void handler_keyword (const std::string line);
  void handler_punctuation (const std::string line);
  bool isseparator (char c);
  machine get_hashed_machine (std::string, std::map<std::string, machine>& machines);
  void push_to_stack (std::string s, std::stack<machine_stacks> &stk, std::map<std::string, machine> &_machines);

 public:
  machine rules2nfa (const std::string rules);

};

#endif //KOMPILER_REXPARSER_H
