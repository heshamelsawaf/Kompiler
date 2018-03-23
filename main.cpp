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

  // machine nfa = rx.rules2nfa ("letter = a-z | A-Z\ndigit = 0-9\nid: letter (letter|digit)*");
  machine nfa = rx.rules2nfa("letter: (a|b)+");
  // nfa.print_machine();
  machine dfa = dfa::to_dfa(nfa);
  // dfa.print_machine();
  // machine min_dfa = dfa::minimize_dfa(dfa);
  // min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}
