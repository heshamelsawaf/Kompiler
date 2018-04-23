#include "parsetable.h"
#include <fstream>
#include <iostream>
#include <sstream>

parsetable::entry::entry() = default;

parsetable::entry::entry(parsetable::entry::States _state) : state(_state) {
}

parsetable::parsetable(cfg grammar) {
    if (grammar.get_starting_symbol() == nullptr) {
        throw std::invalid_argument("Grammar missing a starting symbol.");
    }
    starting_symbol_key = grammar.get_starting_symbol()->get_key();
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
        for (cfg::symbol::production prod : grammar.get_symbol(sym)->get_productions()) {
            for (std::string first : prod.get_first()) {
                if (grammar.get_symbol(first)->is_eps()) {
                    has_eps_prod = true;
                } else {
                    if (table[sym][first].state != parsetable::entry::States::ERROR) {
                        throw std::invalid_argument(
                                "Grammar is not LL(1). Entry [" + sym + "," + first + "] has duplicate values.");
                    }
                    table[sym][first].state = parsetable::entry::States::PROD;
                    for (auto prod_sym : prod.get_symbols()){
                        table[sym][first].productions.push_back(prod_sym->get_key());
                    }
                }
            }
        }
        for (std::string follow : grammar.get_symbol(sym)->get_follow()) {
            if (table[sym][follow].state != parsetable::entry::States::ERROR) {
                throw std::invalid_argument(
                        "Grammar is not LL(1). Entry [" + sym + "," + follow + "] has duplicate values.");
            }
            if (has_eps_prod) {
                table[sym][follow].productions.push_back("\\EPS");
                table[sym][follow].state = parsetable::entry::States::PROD;
            } else {
                table[sym][follow].state = parsetable::entry::States::SYNC;
            }
        }
    }
}

std::string parsetable::get_starting_symbol_key() const {
    return starting_symbol_key;
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

//bool parsetable::deserialize(std::string file_name) {
//    parser::ParseTable parse_table;
//    std::fstream input(file_name, std::ios::in | std::ios::binary);
//    if (!parse_table.ParseFromIstream(&input)) {
//        std::cerr << "Failed to parse " + file_name << std::endl;
//        return false;
//    }
//
//    starting_symbol_key = parse_table.starting_symbol();
//    table.clear();
//
//    for (int i = 0; i < parse_table.entries_size(); i++){
//        parsetable::entry entry;
//        switch(parse_table.entries(i).state()){
//            case parser::ParseTable::Entry::ERROR:
//                entry.state = parsetable::entry::States::ERROR;
//                break;
//            case parser::ParseTable::Entry::SYNC:
//                entry.state = parsetable::entry::States::SYNC;
//                break;
//            case parser::ParseTable::Entry::PROD:
//                entry.state = parsetable::entry::States::PROD;
//                entry.production = parse_table.entries(i).production();
//                break;
//        }
//        table[parse_table.entries(i).nonterm()][parse_table.entries(i).term()] = entry;
//    }
//    return true;
//}
//
//bool parsetable::serialize(std::string file_name) {
//    GOOGLE_PROTOBUF_VERIFY_VERSION;
//
//    parser::ParseTable parse_table;
//    parse_table.set_starting_symbol(starting_symbol_key);
//    for (auto& k : table){
//        for (auto& u : k.second){
//            parser::ParseTable_Entry *entry = parse_table.add_entries();
//            entry->set_nonterm(k.first);
//            entry->set_term(u.first);
//            entry->set_production(u.second.production);
//            switch (u.second.state){
//                case parsetable::entry::States::ERROR:
//                    entry->set_state(parser::ParseTable::Entry::ERROR);
//                    break;
//                case parsetable::entry::States::SYNC:
//                    entry->set_state(parser::ParseTable::Entry::SYNC);
//                    break;
//                case parsetable::entry::States::PROD:
//                    entry->set_state(parser::ParseTable::Entry::PROD);
//                    break;
//            }
//        }
//    }
//
//    std::fstream output(file_name, std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!parse_table.SerializeToOstream(&output)) {
//        std::cerr << "Failed to write parsetable." << std::endl;
//        return false;
//    }
//    google::protobuf::ShutdownProtobufLibrary();
//    return true;
//}

std::ostream &operator<<(std::ostream &stream, const parsetable &t) {

    if (t.table.empty()) {
        stream << "Table is empty\n";
        return stream;
    }

    std::vector<std::vector<std::string>> T;
    int max_len = 0;
    for (auto &k : t.table) {
        T.push_back(std::vector<std::string>());
        T.back().push_back("");
        for (auto &t : k.second) {
            T.back().push_back(t.first);
            max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        }
        break;
    }

    for (auto &k : t.table) {
        T.push_back(std::vector<std::string>());
        T.back().push_back(k.first);
        max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        for (int i = 1; i < T[0].size(); i++) {
            if (t.table.at(T.back()[0]).at(T[0][i]).state == parsetable::entry::States::ERROR) {
                T.back().push_back("\\ERROR");
            } else if (t.table.at(T.back()[0]).at(T[0][i]).state == parsetable::entry::States::SYNC) {
                T.back().push_back("\\SYNC");
            } else {
                std::string temp = k.first + " ->";
                for (auto &prod_sym : t.table.at(T.back()[0]).at(T[0][i]).productions){
                    temp += " " + prod_sym;
                }
                T.back().push_back(temp);
            }
            max_len = max_len < T.back().back().length() ? T.back().back().length() : max_len;
        }
    }

    for (int i = 0; i < T.size(); i++) {
        for (int j = 0; j < T[i].size(); j++) {
            std::string temp;
            if (T[i][j] == EOI) {
                temp = "\\$";
            } else {
                temp = T[i][j];
            }
            stream << temp;
            for (int k = (int) temp.length(); k <= max_len; k++) {
                stream << ' ';
            }
            stream << "|  ";
        }
        stream << std::endl;
    }
    stream << std::endl << "Starting Symbol: " << t.get_starting_symbol_key() << std::endl;
    return stream;
}

