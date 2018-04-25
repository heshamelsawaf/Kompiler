//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "rexparser.h"
#include "lexer.h"
#include "tokenizer.h"
#include "dfa.h"

using namespace std;

int main (int argc, char** argv)
{
  ifstream specs_ifs;
  ifstream src_ifs;
  ofstream ttab_ofs;
  ofstream ttok_ofs;

  if (argc < 5) {
    perror("Invalid number of arguments!\n \
            USAGE: ./Kompiler <specs_file> <input_file> \
             <transition_table_output> <token_list_output>");
    errno = EINVAL;
    exit(-1);
  }

  specs_ifs.open(argv[1]);
  src_ifs.open(argv[2]);
  ttab_ofs.open(argv[3]);
  ttok_ofs.open(argv[4]);

  if (!specs_ifs || !src_ifs || !ttab_ofs || !ttok_ofs) {
    perror("Unable to read file");
    exit(-1);
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

  for (lexer::token tok : v) {
    ttok_ofs << tok << endl;
    cout << tok << endl;
  }

  specs_ifs.close();
  src_ifs.close();
  ttab_ofs.close();
  ttok_ofs.close();

  return 0;
}

