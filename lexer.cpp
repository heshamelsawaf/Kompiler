#include "lexer.h"
#include "machine.h"
#include <sstream>
#include "rexparser.h"
#include "dfa.h"

lexer::lexer(const std::ifstream &_ifs) {
    rules_ifs = _ifs;
}

void lexer::lex_analyze() {
    auto s = [&rules_ifs] {
      std::ostringstream ss;
      ss << rulse_ifs.rdbuf();
      return ss.str();
    }();

    if (s.empty())
        return NULL;

    rexparser rx;

    machine nfa = rx.rules2nfa(s);
    machne  dfa = dfa::to_dfa(nfa);
    machine min_dfa = dfa::minimize_dfa(dfa);

    ttab = transition_table(min_dfa);
}


lexer::token next_token(std::ifstream &ifs) {
    std::ostringstream accum_ss;
    std::ostringstream token_ss;
    std::string token_class = "error";

    if (ifs.eof() || !ifs.is_open())
        return NULL;

    char c = ifs.peek();
    if (c == EOF)
        return NULL;

    while (isspace((char) ifs.peek()))
        ifs >> c;

    while (ifs.peek()) {
        sid_t st = ttab.move(c);
        if (st == -1) {    // reached a deadend state
            ttab.reset();
            break;
        }

        accum_ss << ifs.get(c);

        else if (ttab.is_accepting()) {
            // append accum_ss to token_ss, clear accum_ss afterwards
            token_ss << accum_ss;
            accum_ss.str(""); accum_ss.clear();

            token_class = ttab.get_token_class();
        }
    }

    return token(token_class.empty() ? accum_ss.str() : token_ss.str(),
            token_class);
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


lexer::token next_token(const std::string &str,
                        transition_table ttab, int &ptr) {
    if (ptr == NULL || ptr >= str.size())
        return NULL;

    std::ostringstream ss;
    ttab.reset();

    while (isspace(str[ptr] && ptr < str.size())
        ptr++;

    while (!isspace(str[ptr] && ptr < str.size()) {
        ss << str[ptr];
        ttab.move(str[ptr]);
        ptr++;
    }

    return lexer::token(ss.str(), get_token_class());
}
