#ifndef KOMPILAR_PARSER_H
#define KOMPILAR_PARSER_H

#include <fstream>
#include "lexer.h"

namespace parser {
    std::vector<lexer::token> parse_input(ifstream &ifs, transition_table ttab);
}

#endif //KOMPILAR_PARSER_H
