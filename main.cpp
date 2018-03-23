//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "machine.h"
#include "rexplib.h"
#include "rexparser.h"
#include "dfa.h"

using namespace std;


int main (int argc, char** argv)
{

  ifstream inFile;
  rexparser rx;

  if (argc < 2) {
    perror("Huh ?");
    exit(1);
  }

  inFile.open (argv[1]);

  if (!inFile) {
    perror("Can't read file mate");
    exit(1);
  }

  auto s = [&inFile]{
    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
  }();

  machine nfa = rx.rules2nfa(s);
  // machine nfa = rx.rules2nfa("letter: ((a-b)|(b?)");
  // nfa.print_machine();
  machine dfa = dfa::to_dfa(nfa);
  // dfa.print_machine();
  // for (int s = 1 ; s <= dfa.get_states_count() ; s++) {
  //   std::cout << (dfa.is_accepting(s) ? "Accepting: " : "Not Accepting: ");
  //   std::cout << s << ": " << dfa.get_token_class(s) << std::endl;
  // }
  machine min_dfa = dfa::minimize_dfa(dfa);
  // for (int s = 1 ; s <= min_dfa.get_states_count() ; s++) {
  //   std::cout << (min_dfa.is_accepting(s) ? "Accepting: " : "Not Accepting: ");
  //   std::cout << s << ": " << min_dfa.get_token_class(s) << std::endl;
  // }
  min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}


