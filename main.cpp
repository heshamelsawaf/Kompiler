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

  rx.rules2nfa ("h: hello").print_machine ();

    return 0;
}
