#ifndef KOMPILAR_PARSER_H
#define KOMPILAR_PARSER_H

#include <iostream>
#include "lexer.h"

class parser {
private:
    std::istream *ifs;
    lexer lex;
public:
    parser(std::istream *_is, lexer &_lex);
    std::vector<lexer::token> parse();
};

#endif //KOMPILAR_PARSER_H
