//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

int test();

int main (int argc, char** argv)
{

  //test();

  ifstream inFile;
  // rexparser rx;

  if (argc < 2) {
    perror("Huh ?");
    exit(1);
  }

  inFile.open (argv[1]);

  if (!inFile) {
    perror("Can't read file mate");
    exit(1);
  }

  char s;
  while (inFile.get(s))
    cout << s << endl;
  // auto s = [&inFile]{
  //   std::ostringstream ss;
  //   ss << inFile.rdbuf();
  //   return ss.str();
  // }();
  //
  // machine nfa = rx.rules2nfa(s);
  //machine nfa = rx.rules2nfa("letter: a+");
  // for (int s = 1 ; s <= nfa.get_states_count() ; s++) {
  //   cout << s << ": " << nfa.get_token_class(s) << std::endl;
  // }
  // nfa.print_machine();
  // machine dfa = dfa::to_dfa(nfa);
  // dfa.print_machine();
  // machine min_dfa = dfa::minimize_dfa(dfa);
//  min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}

int test () {
  // rexparser rx;
  // machine nfa = rx.rules2nfa("letter: a+");
  // for (int s = 1 ; s <= nfa.get_states_count() ; s++) {
  //   cout << s << ": " << nfa.get_token_class(s) << std::endl;
  // }
  // cout << nfa << endl;
  // machine dfa = dfa::to_dfa(nfa);
  // cout << dfa << endl;
  // machine min_dfa = dfa::minimize_dfa(dfa);
  // cout << min_dfa << endl;
  // rx.rules2nfa ("id: a b*").print_machine ();
  return 0;
}
