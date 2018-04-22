#include "parsetable.h"

parsetable::entry::entry() = default;

parsetable::entry::entry(parsetable::entry::States _state) : state(_state) {
}

parsetable::parsetable(cfg grammar) {
    starting_symbol_key = grammar.get_starting_symbol().get_key();
    std::unordered_map<std::string, parsetable::entry> entries;
    for (std::string &sym : grammar.get_symbols()) {
        if (grammar.get_symbol(sym)->is_terminal() && !grammar.get_symbol(sym)->is_eps()) {
            entries[sym] = parsetable::entry(parsetable::entry::States::ERROR);
        }
    }
    entries[EOI] = parsetable::entry(parsetable::entry::States::ERROR);

    for (std::string &sym : grammar.get_symbols()) {
        if (!grammar.get_symbol(sym)->is_terminal()) {
            table[sym] = entries;
        }
    }

    for (std::string &sym : grammar.get_symbols()) {
        bool has_eps_prod = false;
        cfg::symbol::production eps_prod;
        for (cfg::symbol::production prod : grammar.get_symbol(sym)->get_productions()) {
            for (std::string first : prod.get_first()) {
                if (grammar.get_symbol(first)->is_eps()) {
                    has_eps_prod = true;
                    eps_prod = prod;
                } else {
                    if (table[sym][first].state != parsetable::entry::States::ERROR) {
                        throw std::invalid_argument(
                                "Grammar is not LL(1). Entry [" + sym + "," + first + "] has duplicate values.");
                    }
                    table[sym][first].state = parsetable::entry::States::PROD;
                    table[sym][first].prod = prod;
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
                table[sym][follow].state = parsetable::entry::States::SYNC;
            }
        }
    }
}

std::string parsetable::get_starting_symbol_key() {
    return starting_symbol_key;
}

void parsetable::set_starting_symbol_key(std::string s) {
    starting_symbol_key = s;
}

parsetable::entry parsetable::get_entry(std::string nonterm, std::string next_input) {
    if (!parsetable::table.count(nonterm)) {
        throw std::invalid_argument("received invalid nonterminal symbol");
    }
    if (!parsetable::table[nonterm].count(next_input)) {
        cfg::symbol::production p;
        return parsetable::entry(parsetable::entry::States::ERROR);
    }
    return parsetable::table[nonterm][next_input];
}

std::string parsetable::production_to_str(cfg::symbol::production p) {
    std::string s = "";
    for (cfg::symbol *sym : p.get_symbols()) {
        if (sym->is_eps()) {
            s += "\\L";
        }
        else {
            s += sym->get_key();
        }
    }
    return s;
}

void parsetable::print_table() {
    using namespace std;

    if (table.empty()) return;

    vector<vector<string>> T;
    int max_len = 0;

    for (auto &k : table) {
        T.push_back(vector<string>());
        T.back().push_back("");
        for (auto &t : k.second) {
            T.back().push_back(t.first);
            max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        }
        break;
    }

    for (auto &k : table) {
        T.push_back(vector<string>());
        T.back().push_back(k.first);
        max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        for (int i = 1; i < T[0].size(); i++) {
            if (table[T.back()[0]][T[0][i]].state == parsetable::entry::States::ERROR) {
                T.back().push_back("\\ERROR");
            } else if (table[T.back()[0]][T[0][i]].state == parsetable::entry::States::SYNC) {
                T.back().push_back("\\SYNC");
            } else {
                T.back().push_back(T.back()[0] + " -> " + production_to_str(table[T.back()[0]][T[0][i]].prod));
            }
            max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        }
    }

    for (int i = 0; i < T.size(); i++) {
        for (int j = 0; j < T[i].size(); j++) {
            string temp;
            if (T[i][j] == EOI) {
                temp = "\\$";
            } else {
                temp = T[i][j];
            }
            cout << temp;
            for (int k = temp.length(); k <= max_len; k++){
                cout << ' ';
            }
            cout << "|  ";
        }
        cout << endl;
    }
    cout << endl;
}
