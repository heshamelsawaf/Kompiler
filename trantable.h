#ifndef KOMPILAR_TRANTABLE_H
#define KOMPILAR_TRANTABLE_H

#include "machine.h"

class transition_table {
private:
    machine m;
    sid_t cur_state;
public:
    inline transition_table(machine _m) {
        m = _m;
        cur_state = m.get_starting_state();
    }

    inline sid_t get_current_state() {
        return cur_state;
    }

    inline sid_t move(char in) {
        cur_state = m.get_transitions_for(cur_state, in)[0];
        return cur_state;
    }

    inline bool is_accepting(char in) const {
        return m.is_accepting(cur_state);
    }

    inline std::string get_token_class() const {
        return m.get_token_class(cur_state);
    }

    inline sid_t reset() {
        cur_state = m.get_starting_state();
        return cur_state;
    }
};
#endif //KOMPILAR_TRANTABLE_H
