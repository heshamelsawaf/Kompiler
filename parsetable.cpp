#include "parsetable.h"

parsetable::entry::entry() {
}

parsetable::entry::entry(parsetable::entry::States _state, cfg::symbol::production *_prod) {
    state = _state;
    prod = _prod;
}

parsetable::parsetable(cfg grammar) {
    std::unordered_map<std::string, parsetable::entry> entries;
    for (std::string &sym : grammar.get_symbols()) {
        if (grammar.get_symbol(sym)->is_terminal() && !grammar.get_symbol(sym)->is_eps()) {
            entries[sym] = parsetable::entry::entry(parsetable::entry::States::ERROR, nullptr);
        }
    }
    entries[EOI] = parsetable::entry::entry(parsetable::entry::States::ERROR, nullptr);

    for (std::string &sym : grammar.get_symbols()) {
        if (!grammar.get_symbol(sym)->is_terminal()) {
            table[sym] = entries;
        }
    }

    for (std::string &sym : grammar.get_symbols()) {
        bool has_eps_prod = false;
        cfg::symbol::production *eps_prod = nullptr;
        for (cfg::symbol::production &prod : grammar.get_symbol(sym)->get_productions()) {
            for (std::string first : prod.get_first()) {
                if (grammar.get_symbol(first)->is_eps()) {
                    has_eps_prod = true;
                    eps_prod = &prod;
                } else {
                    if (table[sym][first].state != parsetable::entry::States::ERROR) {
                        throw std::invalid_argument(
                                "Grammar is not LL(1). Entry [" + sym + "," + first + "] has duplicate values.");
                    }
                    table[sym][first].state = parsetable::entry::States::PROD;
                    table[sym][first].prod = &prod;
                }
            }
        }
        for (std::string follow : grammar.get_symbol(sym)->get_follow()) {
            if (table[sym][follow].state != parsetable::entry::States::ERROR) {
                throw std::invalid_argument(
                        "Grammar is not LL(1). Entry [" + sym + "," + follow + "] has duplicate values.");
            }
            if (has_eps_prod) {
                table[sym][follow].prod = eps_prod;
                table[sym][follow].state = parsetable::entry::States::PROD;
            } else {
                table[sym][follow].prod = nullptr;
                table[sym][follow].state = parsetable::entry::States::SYNC;
            }
        }
    }
}

parsetable::entry parsetable::get_entry(std::string nonterm, std::string next_input) {
    if (!parsetable::table.count(nonterm)) {
        throw std::invalid_argument("received invalid nonterminal symbol");
    }
    if (!parsetable::table[nonterm].count(next_input)) {
        cfg::symbol::production p;
        return parsetable::entry(parsetable::entry::States::ERROR, nullptr);
    }
    return parsetable::table[nonterm][next_input];
}

void parsetable::print_table() {

}
