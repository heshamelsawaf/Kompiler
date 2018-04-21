#include "cfg.h"
#define EPS "eps"

/* Utility function to find each production A->B where B
 * is non-terminal and perform: FIRST(A) = FIRST(A) U FIRST(B).
 * @return true if a change has occured, false otherwise */
bool build_first_util(cfg *grmr) {
    bool updated = false;

    for (std::string sym_key : grmr->get_symbols()) {
        cfg::symbol *s = grmr->get_symbol(sym_key);

        if (s->is_terminal())
            continue;
        
        for (cfg::symbol::production prod : s->get_productions()) {
            /* For each production S->T where T is non-terminal,
             * Add FIRST(T) to FIRST(S), do not add eps if it is
             * in FIRST(T) */
            cfg::symbol *t = *prod.get_symbols().begin();
            if (!t->is_terminal())
                for (std::string a : t->get_first())
                    if (a != EPS)
                        updated |= prod.add_first(a);

            /* For production S -> X1 X2 X3... Xk, if eps is in
             * FIRST(X1..k), then eps is in FIRST(S) */
            while (t != *prod.get_symbols().end())
                if(!t->contains_first(EPS))
                    break;
            
            /* If reached end of production, then eps is in
             * FIRST(S), else if it reached Xi, then FIRST(Xi) 
             * is in FIRST(S) */
            if (t == *prod.get_symbols().end())
                updated |= prod.add_first(EPS);
            else {
                if (t->is_terminal())
                    updated |= prod.add_first(t->get_key());
                else
                    for (std::string a : t->get_first())
                        updated |= prod.add_first(a);
            }
        }
    }

    return updated;
}

/*      A -> BCd
        B -> Cd
        C -> Ad | eps
*
/* Build first set for each and every production
 * in cfg grammar */
void build_first(cfg *grmr) {
    /* Initialize FIRST sets for non-terminals */
    for (std::string sym_key : grmr->get_symbols()) {
        cfg::symbol *s = grmr->get_symbol(sym_key);
        
        /* If symbol S is terminal, then FIRST(S) = { S } */
        if (s->is_terminal())
            continue;

        /* If symbol S is non-terminal, add all terminals in the
         * beginning of its productions (including EPS) to its first set */
        for (cfg::symbol::production prod : s->get_productions()) {
            cfg::symbol *t = *prod.get_symbols().begin();

            /* If S -> a(X) is a production where a is terminal,
             * add a to FIRST(S) */
            /* If S -> eps is a production then add eps to FIRST(S) */
            if (t->is_terminal() || t->is_eps())
                prod.add_first(t->get_key());
        }
    }

    /* For non-terminals S, A where A->S, FIRST(S) is
     * a subset of FIRST(A), add all of FIRST(A) to FIRST(S),
     * repeat until no additional changes can be made */
    while (build_first_util(grmr));
}

void build_follow(cfg *grmr) {
    
}

void cfg::build() {
    /* TODO: either make `build_first()`,
     * `build_follow()` non-static member functions
     * or separate them outside cfg.cpp */
    build_first(this);
    build_follow(this);
}