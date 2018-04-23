#include "ll1_parser.h"
#include <stack>

void parse::parse_ll1(parsetable &parsetable, std::vector<lexer::token> &tokenized_input) {
    
    std::vector<std::string> stack;
    stack.push_back(EOI);
    std::string starting_symbol = "temp";
    stack.push_back(starting_symbol);
    // std::ostringstream leftmost_derivation;

    std::vector<std::vector<std::string> > leftmost_derivation;

    bool substitute = false;
    std::string substitute_str = "";

    int i = 0;
    int line = 0;
    while (i < tokenized_input.size()) {
        leftmost_derivation.push_back(std::vector<std::string> ());

        int cur_symbol_idx = 0;

        if (line > 0) {
            for (std::string symbol : leftmost_derivation[line - 1]) {
                if (parsetable.is_nonterm(symbol))
                    break;
                leftmost_derivation[line].push_back(symbol);
            }
        }

        cur_symbol_idx = leftmost_derivation[line].size();

        if (substitute) {
            substitute = false;
            leftmost_derivation[line][cur_symbol_idx - 1] = substitute_str;
        }

        for (auto it = stack.end() - 1; it > stack.begin() ; it--) {
            leftmost_derivation[line].push_back(*it);
        }

        std::string cur_symbol = stack.back();
        if (!parsetable.is_nonterm(cur_symbol)) {
            if (cur_symbol == tokenized_input[i].get_class()) {
                i++;
                stack.pop_back();
                substitute = true;
                substitute_str = tokenized_input[i].get_str();
                // leftmost_derivation[line][cur_symbol_idx] = tokenized_input[i].get_str();
            } else {
                if (cur_symbol == EOI) {
                    std::cerr << "Error near: \"" << tokenized_input[i].get_str() << "\"" << std::endl;
                    break;
                }
                stack.pop_back();
                std::cerr << "Error: Expected \"" << cur_symbol
                         << "\"\nAutomatically Inserted: " << cur_symbol << std::endl;
            }
        } else {
            parsetable::entry entry = parsetable.get_entry(cur_symbol, tokenized_input[i].get_class());
            std::vector<cfg::symbol *> symbols;
            
            switch (entry.state) {
            case parsetable::entry::PROD:
                stack.pop_back();
                symbols = entry.prod.get_symbols();
                for (int i = symbols.size() - 1 ; i >= 0 ; i--) {
                    stack.push_back(symbols[i]->get_key());
                }
                break;
            case parsetable::entry::SYNC:
                stack.pop_back();
                break;
            case parsetable::entry::ERROR:
                std::cerr << "Error near: \"" << tokenized_input[i].get_str() << "\"" << std::endl;
                i++;
                break;
            default:
                break;
            }
        }
        
    }
    if (i != tokenized_input.size()) {
        // TODO: Handle Error.
    }
    if (stack.back() != EOI) {
        // TODO: Handle Error.
    }
}
