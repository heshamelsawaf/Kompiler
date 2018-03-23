#include "lexer.h"
#include "machine.h"
#include <sstream>
#include "rexparser.h"
#include "dfa.h"

lexer::transition_table lex_analyze(std::ifstream &ifs, bool verbose) {
    auto s = [&ifs]{
      std::ostringstream ss;
      ss << ifs.rdbuf();
      return ss.str();
    }();

    if (s.empty())
        return NULL;

    rexparser rx;

    machine nfa = rx.rules2nfa(s);
    machne  dfa = dfa::to_dfa(nfa);
    machine min_dfa = dfa::minimize_dfa(dfa);

    return transition_table(min_dfa);
}

lexer::token next_token(std::ifstream &ifs, transition_table &ttab) {
    if (ifs.eof() || !ifs.is_open())
        return NULL;

    char c;
    if (!ifs >> s)
        return NULL;


}

std::vector<lexer::token> tokenize(std::ifstream &ifs, transition_table &ttab) {
    std::vector<lexer::token> v;
    std::unordered_map<int, int> active_states;
    char c;

    if (ifs.eof() || !ifs.is_open()) {
        perror("Error reading from file.")
        return v;
    }

    while (ifs.get(s)) {

    }

}
