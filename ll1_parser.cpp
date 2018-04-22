#include "ll1_parser.h"
#include <stack>

void parse::parse_ll1(parsetable &parsetable, std::vector<lexer::token> &tokenized_input) {
    
    std::stack<std::string> stack;
    stack.push("$");
    std::string starting_symbol = "temp";
    stack.push(starting_symbol);

    int i = 0;
    while (i < tokenized_input.size()) {
        std::string cur_symbol = stack.top();
        if (!parsetable.is_nonterm(cur_symbol)) {
            if (cur_symbol == tokenized_input[i].get_class()) {
                i++;
                stack.pop();
            } else {
                    // TODO: Handle Error.
                    i++;

            }
        } else {
            parsetable::entry entry = parsetable.get_entry(cur_symbol, tokenized_input[i].get_class());
            std::vector<cfg::symbol *> symbols;
            
            switch (entry.state) {
            case parsetable::entry::PROD:
                stack.pop();
                symbols = entry.prod.get_symbols();
                for (int i = symbols.size() - 1 ; i >= 0 ; i--) {
                    stack.push(symbols[i]->get_key());
                }
                break;
            case parsetable::entry::SYNC:
                stack.pop();
                break;
            case parsetable::entry::ERROR:
                // TODO: Handle Error
                i++;
                break;
            default:
                break;
            }
        }
        
    }

    if (stack.top() != "$") {
        // TODO: Handle Error.
    }
}
