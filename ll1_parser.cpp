#include "ll1_parser.h"
#include <stack>

void parse::parse_ll1(parsetable &parsetable, std::vector<lexer::token> &tokenized_input) {
    
    std::vector<std::string> stack;
    stack.push_back(EOI);
    std::string starting_symbol = parsetable.get_starting_symbol_key();
    stack.push_back(starting_symbol);
    // for (lexer::token t : tokenized_input)  {
    //     std::cout << t.get_class() << ' ' << t.get_str() << std::endl;
    // }
    // std::ostringstream leftmost_derivation;

    std::vector<std::vector<std::string> > leftmost_derivation;

    bool substitute = false;
    std::string substitute_str = "";

    int i = 0;
    int line = 0;
    while (true) {
        std::string cur_symbol = stack.back();
        std::string cur_token = i == tokenized_input.size() ? EOI : tokenized_input[i].get_class();
        leftmost_derivation.push_back(std::vector<std::string> ());

        int cur_symbol_idx = 0;

        if (line > 0) {
            for (std::string symbol : leftmost_derivation[line - 1]) {
                if (!parsetable.is_nonterm(symbol))
                    leftmost_derivation[line].push_back(symbol);
                else
                    break;
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
        if (cur_symbol == EOI && i == tokenized_input.size()) {
            break;
        }
        line++;


        if (i < tokenized_input.size() && !parsetable.is_nonterm(cur_symbol)) {
            if (cur_symbol == tokenized_input[i].get_class()) {
                stack.pop_back();
                substitute = true;
                substitute_str = tokenized_input[i].get_str();
                // std::cout << cur_symbol << ' ' << substitute_str << std::endl;
                i++;
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
            parsetable::entry entry = parsetable.get_entry(cur_symbol, cur_token);
            std::vector<cfg::symbol *> symbols;
            
            switch (entry.state) {
            case parsetable::entry::PROD:
                stack.pop_back();
                
                if (entry.productions.size() == 1 && entry.productions[0] == EPS_STR)
                    break;

                for (int i = entry.productions.size() - 1 ; i >= 0 ; i--) {
                    stack.push_back(entry.productions[i]);
                }
                break;
            case parsetable::entry::SYNC:
                stack.pop_back();
                break;
            case parsetable::entry::ERROR:
                if (cur_token != EOI) {
                    std::cerr << "Error near: \"" << tokenized_input[i].get_str() << "\"" << std::endl;
                    i++;
                } else {
                    std::cerr << "Error: Expected \"" << cur_symbol
                         << "\"\nAutomatically Inserted: " << cur_symbol << std::endl;
                }
                break;
            default:
                break;
            }
        }
    }
    for (std::vector<std::string> &line : leftmost_derivation) {
        for (std::string &sym : line) {
            std::cout << sym << ' ';
        }
        std::cout << std::endl;
    }
    if (i != tokenized_input.size()) {
        // TODO: Handle Error.
    }
    if (stack.back() != EOI) {
        std::cout << stack.back() << std::endl;
    } else {
        std::cout << "Parsed Successfully" << std::endl;
    }
}
