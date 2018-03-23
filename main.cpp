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

int main (int argc, char** argv)
{
  ifstream specs_ifs;
  ifstream src_ifs;

  if (argc < 3) {
    perror("Invalid number of arguments");
    exit(1);
  }

  specs_ifs.open(argv[1]);
  src_ifs.open(argv[2]);

  if (!specs_ifs || !src_ifs) {
    perror("Unable to read file");
    exit(1);
  }

  lexer lex(*specs_ifs);
  lex.lex_analyze();

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
