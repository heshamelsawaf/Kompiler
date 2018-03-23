#include "parser.h"
#include "lexer.h"

parser::parser(const std::ifstream &_ifs, lexer &_lex) {
    ifs = _ifs;
    lex = _lex;
}

std::vector<lexer::token> parse() {
    std::vector<lexer::token> v;

    while ((lexer::token tok = lex.next_token) != NULL)
        v.push_back(tok);

    return v;
}
