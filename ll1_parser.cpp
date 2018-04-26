#include "ll1_parser.h"
#include <stack>

enum error_type {
    MISSING_SYMBOL,
    INVALID_TOKEN,
    ERROR_PRODUCTION,
    EXCESS_SYMBOL,
    REACHED_EOF
};

std::string production_to_string(std::string lhs, std::vector<std::string> rhs) {
    std::string prod = lhs + " ->";
    for (std::string sym : rhs) {
        prod += " " + sym;
    }
    return prod;
}


std::string get_message(std::string cur_symbol, std::string cur_token, error_type error) {
    switch (error) {
    case MISSING_SYMBOL:
        return "Error: Expected \"" + cur_symbol + "\"";
        break;
    case INVALID_TOKEN:
        return "Error: Invalid token or identifier \"" + cur_token + "\"";
        break;
    case ERROR_PRODUCTION:
        return "Error production near: \"" + cur_token + "\"";
        break;
    case EXCESS_SYMBOL:
        return "Error near: \"" + cur_token + "\"";
        break;
    case REACHED_EOF:
        return "Error: Expected \"" + cur_symbol + "\"";
        break;
    default:
        return "";
        break;
    }
}

leftmost_derivation parse::parse_ll1(parsetable &parsetable, machine &mac, std::istream &input_stream) {
    
    std::vector<std::string> stack;
    lexer lex(mac);
    stack.push_back(EOI);
    std::string starting_symbol = parsetable.get_starting_symbol_key();
    stack.push_back(starting_symbol);
    // for (lexer::token t : tokenized_input)  {
    //     std::cout << t.get_class() << ' ' << t.get_str() << std::endl;
    // }
    // std::ostringstream derivations;

    std::vector<std::vector<std::string> > derivations;
    std::vector<std::string> productions;
    std::vector<error> errors;

    bool substitute = false;
    std::string substitute_str = "";
    bool prev_is_production = false;
    std::string prev_production = "";


    int step = 0;
    lexer::token cur_token = lex.next_token(input_stream);
    while (true) {
        std::string cur_symbol = stack.back();
        std::string cur_token_class = cur_token.get_class() == EOF_MARK ? EOI : cur_token.get_class();
        
        if (cur_token_class == ERROR_CLASS) {
            errors.push_back(error(cur_token.get_line(),
                                           cur_token.get_col(),
                                           get_message(cur_symbol, cur_token.get_str(), INVALID_TOKEN)));
            cur_token = lex.next_token(input_stream);
            continue;
        }
        
        derivations.push_back(std::vector<std::string> ());
        productions.push_back("");

        int cur_symbol_idx = 0;


        if (step > 0) {
            for (std::string symbol : derivations[step - 1]) {
                if (!parsetable.is_nonterm(symbol))
                    derivations[step].push_back(symbol);
                else
                    break;
            }
        }

        cur_symbol_idx = derivations[step].size();

        if (prev_is_production) {
            productions[step] = std::string(prev_production);
            prev_is_production = false;
        }

        if (substitute) {
            substitute = false;
            productions[step] = "Substitute: "
            + derivations[step][cur_symbol_idx - 1] + " = " + substitute_str;
            derivations[step][cur_symbol_idx - 1] = substitute_str;
        }

        for (auto it = stack.end() - 1; it > stack.begin() ; it--) {
            derivations[step].push_back(*it);
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
                // derivations[step][cur_symbol_idx] = tokenized_input[i].get_str();
            } else {
                if (cur_symbol == EOI) {
                    errors.push_back(error(cur_token.get_line(),
                                           cur_token.get_col(),
                                           get_message(cur_symbol, cur_token.get_str(), EXCESS_SYMBOL)));
                    prev_production = get_message(cur_symbol, cur_token.get_str(), EXCESS_SYMBOL);
                    prev_is_production = true;
                    // std::cerr << "Error near: \"" << cur_token.get_str() << "\"" << std::endl;
                    cur_token = lex.next_token(input_stream);
                } else {
                    stack.pop_back();
                    errors.push_back(error(cur_token.get_line(),
                                        cur_token.get_col(),
                                        get_message(cur_symbol, cur_token.get_str(), MISSING_SYMBOL)));
                    prev_production = get_message(cur_symbol, cur_token.get_str(), MISSING_SYMBOL);
                    prev_is_production = true;
                }
            }
        } else {
            parsetable::entry entry = parsetable.get_entry(cur_symbol, cur_token_class);
            std::vector<cfg::symbol *> symbols;
            
            switch (entry.state) {
            case parsetable::entry::PROD:
                stack.pop_back();

                prev_production = production_to_string(cur_symbol, entry.productions);
                prev_is_production = true;
                
                if (entry.productions.size() == 1 && entry.productions[0] == EPS_STR)
                    break;

                for (int i = entry.productions.size() - 1 ; i >= 0 ; i--) {
                    stack.push_back(entry.productions[i]);
                }
                break;
            case parsetable::entry::SYNC:
                prev_production = production_to_string(cur_symbol, entry.productions);
                prev_is_production = "SYNC";
                stack.pop_back();
                break;
            case parsetable::entry::ERROR:
                if (cur_token_class != EOI) {
                    errors.push_back(error(cur_token.get_line(),
                                           cur_token.get_col(),
                                           get_message(cur_symbol, cur_token.get_str(), ERROR_PRODUCTION)));
                    prev_production = get_message(cur_symbol, cur_token.get_str(), ERROR_PRODUCTION);
                    prev_is_production = true;
                    cur_token = lex.next_token(input_stream);
                } else {
                    errors.push_back(error(cur_token.get_line(),
                                           cur_token.get_col(),
                                           get_message(cur_symbol, cur_token.get_str(), REACHED_EOF)));
                    prev_production = get_message(cur_symbol, cur_token.get_str(), REACHED_EOF);
                    prev_is_production = true;
                    stack.pop_back();
                    // std::cerr << "Error: Expected \"" << cur_symbol
                    //      << "\"\nAutomatically Inserted: " << cur_symbol << std::endl;
                }
                break;
            default:
                break;
            }
        }
    }
    std::vector<sentential_expression> expressions;
    for (int i = 0 ; i < derivations.size() ; i++) {
        expressions.push_back(sentential_expression(derivations[i], productions[i]));
    }
    leftmost_derivation lm_derivation(expressions, errors);
    return lm_derivation;
}
