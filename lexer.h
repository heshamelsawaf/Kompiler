#ifndef KOMPILAR_LEXER_H
#define KOMPILAR_LEXER_H

#include <fstream>
#include "machine.h"

class lexer {

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
    };

    class transition_table {
    private:
        machine m;
        sid_t cur_state;
    public:
        transition_table(machine _m) {
            m = _m;
            cur_state = m.get_starting_state();
        }

        sid_t get_current_state() {
            return cur_state;
        }

        sid_t move(char in) {
            std::vector<sid_t> transition = m.get_transitions(cur_state, in);
            if (transition.empty())
                return -1;
            cur_state = m.get_transitions(cur_state, in)[0];
            return cur_state;
        }

        bool has_transition(sid_t state, c) {
            std::vector<sid_t> transitions = m.get_transitions(state, in);
            if (transition.empty())
                return false;
            return true;
        }

        bool is_accepting() {
            return m.is_accepting(cur_state);
        }

        std::string get_token_class() {
            return m.get_token_class(cur_state);
        }

        sid_t reset() {
            cur_state = m.get_starting_state();
            return cur_state;
        }
    };
private:
    std::ifstream

public:
    lexer(const std::ifstream &ifs);
    lex_analyze();
    token next_token(const std::ifstream &ifs);
};

#endif //KOMPILAR_PARSER_H
