#include "ll1_parser.h"
#include "machine.h"
#include <stack>

void parse::parse_ll1(parsetable &parsetable, machine &mac, std::istream &input_stream) {
    
    std::vector<std::string> stack;
    lexer lex(mac);
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

    int step = 0;
    lexer::token cur_token = lex.next_token(input_stream);
    while (true) {
        std::string cur_symbol = stack.back();
        std::string cur_token_class = cur_token.get_class() == EOF ? EOI : cur_token.get_class();
        leftmost_derivation.push_back(std::vector<std::string> ());

        int cur_symbol_idx = 0;

        if (step > 0) {
            for (std::string symbol : leftmost_derivation[step - 1]) {
                if (!parsetable.is_nonterm(symbol))
                    leftmost_derivation[step].push_back(symbol);
                else
                    break;
            }
        }

        cur_symbol_idx = leftmost_derivation[step].size();

        if (substitute) {
            substitute = false;
            leftmost_derivation[step][cur_symbol_idx - 1] = substitute_str;
        }
        
        for (auto it = stack.end() - 1; it > stack.begin() ; it--) {
            leftmost_derivation[step].push_back(*it);
        }
        if (cur_symbol == EOI && cur_token_class == EOI) {
            break;
        }
        step++;


        if (cur_token_class != EOI && !parsetable.is_nonterm(cur_symbol)) {
            if (cur_symbol == cur_token_class) {
                stack.pop_back();
                substitute = true;
                substitute_str = cur_token.get_str();
                // std::cout << cur_symbol << ' ' << substitute_str << std::endl;
                cur_token = lex.next_token(input_stream);
                // leftmost_derivation[step][cur_symbol_idx] = tokenized_input[i].get_str();
            } else {
                if (cur_symbol == EOI) {
                    std::cerr << "Error near: \"" << cur_token.get_str() << "\"" << std::endl;
                    break;
                }
                stack.pop_back();
                std::cerr << "Error: Expected \"" << cur_symbol
                         << "\"\nAutomatically Inserted: " << cur_symbol << std::endl;
            }
        } else {
            parsetable::entry entry = parsetable.get_entry(cur_symbol, cur_token_class);
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
                if (cur_token_class != EOI) {
                    std::cerr << "Error near: \"" << cur_token.get_str() << "\"" << std::endl;
                    cur_token = lex.next_token(input_stream);
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
    for (std::vector<std::string> &step : leftmost_derivation) {
        for (std::string &sym : step) {
            std::cout << sym << ' ';
        }
        std::cout << std::endl;
    }
    if (cur_token.get_class() != EOF) {
        // TODO: Handle Error.
    }
    if (stack.back() != EOI) {
        std::cout << stack.back() << std::endl;
    } else {
        std::cout << "Parsed Successfully" << std::endl;
    }
}
