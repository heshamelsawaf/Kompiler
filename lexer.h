#ifndef KOMPILAR_LEXER_H
#define KOMPILAR_LEXER_H

#include <fstream>
#include "machine.h"
#include "trantable.h"

class lexer {
public:
        class token {
        private:
            std::string t_str;
            std::string t_class;
        public:
            token(std::string _str, std::string _class) {
                t_str = _str;
                t_class = _class;
            }

            std::string get_str() {
                return t_str;
            }

            std::string get_class() {
                return t_class;
            }

            friend std::ostream &operator <<(std::ostream &os, token &tok);
        };

private:
    // std::ifstream rules_ifs;
    transition_table ttab;

public:
    lexer(machine &mac);
    // void lex_analyze();
    token next_token(std::ifstream &ifs);
};

#endif //KOMPILAR_PARSER_H
