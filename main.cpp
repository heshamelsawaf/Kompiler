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
  ofstream ttab_ofs;

  if (argc < 4) {
    perror("Invalid number of arguments");
    exit(1);
  }

  specs_ifs.open(argv[1]);
  src_ifs.open(argv[2]);
  ttab_ofs.open(argv[3]);

  if (!specs_ifs || !src_ifs || !ttab_ofs) {
    perror("Unable to read file");
    exit(1);
  }

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

  ttab_ofs << min_dfa;

  lexer lex(min_dfa);
  parser prs(&src_ifs, lex);

  std::vector<lexer::token> v = prs.parse();

  for (lexer::token tok : v)
    std::cout << "str: " << tok.get_str() << " --- class: " << tok.get_class() << endl;

    specs_ifs.close();
    src_ifs.close();
    ttab_ofs.close();

  return 0;
}

