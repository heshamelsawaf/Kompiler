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

using namespace std;


int main (int argc, char** argv)
{

  ifstream inFile;
  rexparser rx;

<<<<<<< HEAD
  if (argc < 2){
      perror ("Huh ?");
      exit (1);
    }

  inFile.open (argv[1]);

  if (!inFile){
      perror ("Can't read file mate");
      exit (1);
    }

  auto s = [&inFile]{
    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
  }();

   rx.rules2nfa (s).print_machine ();
  //machine nfa = rx.rules2nfa("letter: a+");
  // for (int s = 1 ; s <= nfa.get_states_count() ; s++) {
  //   cout << s << ": " << nfa.get_token_class(s) << std::endl;
  // }
  // nfa.print_machine();
//  machine dfa = dfa::to_dfa(nfa);
  // dfa.print_machine();
  //machine min_dfa = dfa::minimize_dfa(dfa);
 // min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();

  return 0;
}


