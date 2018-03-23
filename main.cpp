//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include "machine.h"
#include "rexplib.h"
#include "rexparser.h"

using namespace std;

int main ()
{

  rexparser rx;

   rx.rules2nfa ("letter = a-z | A-Z\ndigit = 0-9\nid: letter (letter|digit)*").print_machine ();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}
