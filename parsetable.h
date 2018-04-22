#ifndef KOMPILER_PARSETABLE_H
#define KOMPILER_PARSETABLE_H

#include <unordered_map>
#include <string>
#include "cfg.h"

#define EOI std::string(1, 0x03)

class parsetable {

public:
    class entry {
    public:
        enum States {
            ERROR, SYNC, PROD
        };
        States state;
        cfg::symbol::production prod;

        entry();

        entry(States _state);
    };

    parsetable(cfg);

    entry get_entry(std::string nonterm, std::string next_input);

    std::string production_to_str(cfg::symbol::production p);

    void print_table();

    std::string get_starting_symbol_key();

    void set_starting_symbol_key(std::string s);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, entry>> table;
    std::string starting_symbol_key;
};


#endif //KOMPILER_PARSETABLE_H
