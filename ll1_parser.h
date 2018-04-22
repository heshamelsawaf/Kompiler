#include "parsetable.h"
#include "lexer.h"
#include <string>
#include <fstream>

namespace parse {

void parse_ll1(parsetable &parsetable, std::vector<lexer::token> &tokenized_input);

}