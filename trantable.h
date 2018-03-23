#ifndef KOMPILAR_TRANTABLE_H
#define KOMPILAR_TRANTABLE_H

#include "machine.h"
#include "vector"
#include "string"

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

#endif //KOMPILAR_TRANTABLE_H
