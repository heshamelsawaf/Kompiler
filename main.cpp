//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "rexparser.h"
#include "lexer.h"
#include "parser.h"
#include "dfa.h"

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

  // auto s = [&inFile]{
  //   std::ostringstream ss;
  //   ss << inFile.rdbuf();
  //   return ss.str();
  // }();

  // machine nfa = rx.rules2nfa(s);
  // // machine nfa = rx.rules2nfa("letter: ((a-b)|(b?)");
  // // nfa.print_machine();
  // machine dfa = dfa::to_dfa(nfa);
  // // dfa.print_machine();
  // // for (int s = 1 ; s <= dfa.get_states_count() ; s++) {
  // //   std::cout << (dfa.is_accepting(s) ? "Accepting: " : "Not Accepting: ");
  // //   std::cout << s << ": " << dfa.get_token_class(s) << std::endl;
  // // }
  // machine min_dfa = dfa::minimize_dfa(dfa);
  // for (int s = 1 ; s <= min_dfa.get_states_count() ; s++) {
  //   std::cout << (min_dfa.is_accepting(s) ? "Accepting: " : "Not Accepting: ");
  //   std::cout << s << ": " << min_dfa.get_token_class(s) << std::endl;
  // }
  // min_dfa.print_machine();
  //rx.rules2nfa ("id: a b*").print_machine ();
  
  auto s = [&specs_ifs] {
      std::ostringstream ss;
      ss << specs_ifs.rdbuf();
      return ss.str();
    }();

    if (s.empty())
        return 0;

    rexparser rx;

  machine nfa = rx.rules2nfa(s);
  machine dfa = dfa::to_dfa(nfa);
  machine min_dfa = dfa::minimize_dfa(dfa);

  lexer lex(min_dfa);
  parser prs(&src_ifs, lex);

  std::vector<lexer::token> v = prs.parse();

  for (lexer::token tok : v)
    std::cout << "str: " << tok.get_str() << " --- class: " << tok.get_class() << endl;
  // lex.lex_analyze();
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
