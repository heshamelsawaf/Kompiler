#include "parser.h"

parser::parser(std::ifstream *_ifs, lexer &_lex) : lex{_lex} {
    ifs = _ifs;
}


std::vector<lexer::token> parser::parse() {
    std::vector<lexer::token> v;
    lexer::token tok ("", "");
    do {
        tok = lex.next_token(*ifs);
        v.push_back(tok);
    } while (tok.get_class() != "eof");
    return v;
}
