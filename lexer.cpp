#include "lexer.h"
#include "machine.h"
#include "rexparser.h"
#include "dfa.h"
#include <sstream>

lexer::lexer(machine &mac) : ttab(mac) {
    line = 1;
    col = 0;
}

lexer::token lexer::next_token(std::istream &ifs) {
    std::ostringstream accum_ss;
    std::ostringstream token_ss;
    std::string token_class = ERROR_CLASS;
    
    if (!ifs || ifs.eof())
        return lexer::token("", EOF, line, col);
    std::string str;
    char c = ifs.peek();

    while (isspace((char) ifs.peek())) {
        if(ifs.peek() == '\n') {
            line++;
            col = 0;
        } else {
            col++;
        }
        ifs.get(c);
    }

    if (ifs.eof())
        return lexer::token("", EOF, line, col);

    // Recognize when starting state is also a final state
    if (ttab.is_accepting())
        token_class = ttab.get_token_class();
        
    while (ifs.peek()) {
        c = ifs.peek();
        sid_t st = ttab.move(c);

        if (st == -1)    // reached a deadend state
            break;
        
        ifs.get(c);
        col++;
        accum_ss << c;
        if (ttab.is_accepting()) {
            // append accum_ss to token_ss, clear accum_ss afterwards
            
            token_ss << accum_ss.str() ;
            accum_ss.str(""); accum_ss.clear();

            token_class = ttab.get_token_class();
        }
    }
    if (token_class == ERROR_CLASS) {
        ifs.get(c);
        token_ss << c;
    }

    ttab.reset();
    return lexer::token(token_class.empty() ? accum_ss.str() : token_ss.str(),
            token_class, line, col);
}

std::ostream &operator <<(std::ostream &os, lexer::token &tok) {
    os << std::left
       << tok.get_line()
       << ":"
       << tok.get_col()
       << ":"
       << "\t"
       << tok.get_str()
       << "\t"
       << tok.get_class();
    return os;
}
