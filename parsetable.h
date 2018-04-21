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
        cfg::symbol::production *prod;

        entry();

        entry(States _state, cfg::symbol::production *_prod);
    };

    parsetable(cfg);

    entry get_entry(std::string nonterm, std::string next_input);

    void print_table();

private:
    std::unordered_map<std::string, std::unordered_map<std::string, entry>> table;
};


#endif //KOMPILER_PARSETABLE_H
