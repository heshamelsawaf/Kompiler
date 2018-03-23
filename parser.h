#ifndef KOMPILAR_PARSER_H
#define KOMPILAR_PARSER_H

#include <fstream>
#include "lexer.h"

class parser {
private:
    std::ifstream *ifs;
    lexer lex;
public:
    parser(std::ifstream *_ifs, lexer &_lex);
    std::vector<lexer::token> parse();
};

#endif //KOMPILAR_PARSER_H
