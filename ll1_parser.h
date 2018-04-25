#ifndef KOMPILER_LL1_PARSER_H
#define KOMPILER_LL1_PARSER_H

#include "parsetable.h"
#include "lexer.h"
#include "leftmost_derivation.h"
#include <string>
#include <iostream>

namespace parse {

// void parse_ll1(parsetable &parsetable, std::vector<lexer::token> &tokenized_input);

leftmost_derivation parse_ll1(parsetable &parsetable, machine &mac, std::istream &input_file);

}

#endif //KOMPILER_LL1_PARSER_H