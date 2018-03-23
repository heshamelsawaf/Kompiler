#include "lexer.h"
#include <sstream>

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
