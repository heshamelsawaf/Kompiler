#ifndef KOMPILAR_LEXER_H
#define KOMPILAR_LEXER_H

#include <fstream>
#include "machine.h"

namespace lexer {

    class token {

    private:
        std::string t_str;
        std::string t_class;
    public:
        inline token(std::string _str, std::string _class) {
            t_str = _str;
            t_class = _class;
        }

        inline std::string get_str() {
            return t_str;
        }

        inline std::string get_class() {
            return t_class;
        }
    };

public:
    token next_token(const std::string &str, int &ptr);
};

#endif //KOMPILAR_PARSER_H
