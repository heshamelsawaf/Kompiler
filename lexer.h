#ifndef KOMPILAR_LEXER_H
#define KOMPILAR_LEXER_H

#include <iostream>
#include "machine.h"
#include "trantable.h"

#define EOF "eof"

class lexer {
public:
        class token {
        private:
            std::string t_str;
            std::string t_class;
            int line;
            int col;
        public:
            token () {};

            token(std::string _str, std::string _class) {
                t_str = _str;
                t_class = _class;
            }

            token(std::string _str, std::string _class, int _line, int _col) {
                t_str = _str;
                t_class = _class;
                line = _line;
                col = _col;
            }

            int get_line() {
                return line;
            }

            int get_col() {
                return col;
            }

            std::string get_str() {
                return t_str;
            }

            std::string get_class() {
                return t_class;
            }

            void set_str(std::string str) {
                t_str = str;
            }

            void set_class(std::string _class) {
                t_class = _class;
            }

            void set_line(int _line) {
                line = _line;
            }

            void set_str(int _col) {
                col = _col;
            }

            friend std::ostream &operator <<(std::ostream &os, token &tok);
        };

private:
    // std::ifstream rules_ifs;
    transition_table ttab;
    int line;
    int col;

public:
    lexer(machine &mac);
    // void lex_analyze();
    token next_token(std::istream &ifs);
};

#endif //KOMPILAR_PARSER_H
