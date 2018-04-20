#include "cfg.h"

bool build_first_util(cfg &grmr) {
    bool done = false;

    for (std::string sym_key : grmr.get_symbols()) {
        cfg::symbol *s = grmr.get_symbol(sym_key);

        if (s->is_terminal())
            continue;
        
        for (cfg::symbol::production prod : s->get_productions()) {
            /* For each production S->T where T is non-terminal,
             * Add FIRST(T) to FIRST(S) */
            cfg::symbol *t = *prod.get_symbols().begin();
            if (!t->is_terminal())
                for (std::string a: t->get_first())
                    done ^= prod.add_first(a);
        }
    }
    
    return done;
}

void build_first(cfg &grmr) {
    /* initialize FIRST sets for symbols */
    for (std::string sym_key : grmr.get_symbols()) {
        cfg::symbol *s = grmr.get_symbol(sym_key);
        
        /* If symbol S is terminal, then FIRST(S) = { S } */
        if (s->is_terminal())
            continue;

        /* If symbol S is non-terminal, add all terminals in the
           beginning of its productions (including EPS) to its first set */
        for (cfg::symbol::production prod : s->get_productions()) {
            cfg::symbol *t = *prod.get_symbols().begin();
            if (t->is_terminal())
                prod.add_first(t->get_key());
        }
    }

    /* For non-terminals S, A where A->S, FIRST(S) is
       a subset of FIRST(A), add all of FIRST(A) to FIRST(S),
       repeat until no changes can be made */
    while (build_first_util(grmr));
}

void build_follow(cfg &grmr) {

}

void cfg::build() {
    /* TODO: either make `build_first()`,
     * `build_follow()` non-static member functions
     * or separate them outside cfg.cpp */
    build_first(*this);
    build_follow(*this);
}