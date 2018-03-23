#include "lexer.h"
#include "machine.h"
#include "rexparser.h"
#include "dfa.h"
#include <sstream>

lexer::lexer(machine &mac) : ttab(mac) {
    
}

// void lexer::lex_analyze() {
//     ttab = transition_table(min_dfa);
// }


lexer::token lexer::next_token(std::ifstream &ifs) {
    std::ostringstream accum_ss;
    std::ostringstream token_ss;
    std::string token_class = "error";

    if (ifs.eof() || !ifs.is_open())
        return lexer::token("", token_class);

    char c = ifs.peek();
    if (c == EOF)
        return lexer::token("", token_class);

    while (isspace((char) ifs.peek()))
        ifs >> c;

    while (ifs.peek()) {
        sid_t st = ttab.move(c);
        if (st == -1) {    // reached a deadend state
            ttab.reset();
            break;
        }
        
        ifs.get(c);
        accum_ss << c;

        if (ttab.is_accepting()) {
            // append accum_ss to token_ss, clear accum_ss afterwards
            token_ss << accum_ss.str() ;
            accum_ss.str(""); accum_ss.clear();

            token_class = ttab.get_token_class();
        }
    }

    return lexer::token(token_class.empty() ? accum_ss.str() : token_ss.str(),
            token_class);
}
