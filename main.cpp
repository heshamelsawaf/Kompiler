//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include "machine.h"
#include "rexplib.h"
#include "rexparser.h"
#include "dfa.h"

using namespace std;

int main ()
{

  rexparser rx;

  // machine nfa = rx.rules2nfa ("letter = a-z | A-Z\ndigit = 0-9\nid: letter (letter|digit)*\ndigits: digit+\n{boolean int float}\nnum: digit+ | digit+ . digits (\L | E digits)");
  machine nfa = rx.rules2nfa("letter: ((A-C) | (a-c) | (0-1))+");
  // for (int s = 1 ; s <= nfa.get_states_count() ; s++) {
  //   cout << s << ": " << nfa.get_token_class(s) << std::endl;
  // }
  // nfa.print_machine();
  machine dfa = dfa::to_dfa(nfa);
  // dfa.print_machine();
  machine min_dfa = dfa::minimize_dfa(dfa);
  min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}
