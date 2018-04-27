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
        return "Error: Missing \"" + cur_symbol + "\", Inserted.";
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
        return "Error: Expected \"" + cur_symbol + "\" , but end of file was found.";
        break;
    default:
        return "";
        break;
    }
}

void panic(parsetable &parsetable, std::vector<std::string> &stack,
           lexer &lex, lexer::token &cur_token, std::string &prev_production,
           std::string cur_symbol, error_type err_type, std::istream &input_stream,
           bool &prev_is_production, std::vector<error> &errors) {
    errors.push_back(error(cur_token.get_line(),
                                           cur_token.get_col(),
                                           get_message(cur_symbol, cur_token.get_str(), err_type)));
    prev_production = get_message(cur_symbol, cur_token.get_str(), err_type);
    prev_is_production = true;
    if (err_type == REACHED_EOF) {
        stack.push_back(EOI);
    } else if (err_type == MISSING_SYMBOL) {
        stack.pop_back();
    } else {
        cur_token = lex.next_token(input_stream);
    }
}

leftmost_derivation parse::parse_ll1(parsetable &parsetable, machine &mac, std::istream &input_stream) {
    
    std::vector<std::string> stack;
    lexer lex(mac);
    stack.push_back(EOI);
    std::string starting_symbol = parsetable.get_starting_symbol_key();
    stack.push_back(starting_symbol);

    std::vector<std::vector<std::string> > derivations;
    std::vector<std::string> productions;
    std::vector<error> errors;

    bool substitute = false;
    bool inserted_missing_sym = false;
    std::string substitute_str = "";
    bool prev_is_production = false;
    std::string prev_production = "";


    int step = 0;
    int cur_symbol_idx = 0;
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

        if (step > 0) {
            for (int i = 0 ; i < cur_symbol_idx ; i++) {
                std::string symbol = derivations[step - 1][i];
                if (!parsetable.is_nonterm(symbol))
                    derivations[step].push_back(symbol);
                else
                    break;
            }
        }


        if (prev_is_production) {
            productions[step] = std::string(prev_production);
            prev_is_production = false;
        }

        if (substitute) {
            substitute = false;
            if (inserted_missing_sym) {
                inserted_missing_sym = false;
                productions[step] = "Inserted Missing Symbol: " + substitute_str;
            } else {
                productions[step] = "Substitute: "
                + derivations[step][cur_symbol_idx - 1] + " = " + substitute_str;
            }
            derivations[step][cur_symbol_idx - 1] = substitute_str;
        }

        for (auto it = stack.end() - 1; it > stack.begin() ; it--) {
            if (*it != EOI)
                derivations[step].push_back(*it);
        }
        if (cur_symbol == EOI && cur_token_class == EOI) {
            break;
        }
        step++;


        if (!parsetable.is_nonterm(cur_symbol)) {
            if (cur_symbol == cur_token_class) {
                stack.pop_back();
                substitute = true;
                cur_symbol_idx++;
                substitute_str = cur_token.get_str();
                cur_token = lex.next_token(input_stream);
            } else {
                error_type err_type;
                if (cur_symbol == EOI) {
                    err_type = EXCESS_SYMBOL;
                } else if (cur_token_class == EOI) {
                    err_type = REACHED_EOF;
                } else {
                    err_type = MISSING_SYMBOL;
                    substitute = true;
                    substitute_str = "'" + cur_symbol + "'";
                    inserted_missing_sym = true;
                    cur_symbol_idx++;
                }
                panic(parsetable, stack, lex, cur_token, prev_production,
                          cur_symbol, err_type, input_stream, prev_is_production, errors);
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
                prev_production = "SYNC";
                prev_is_production = true;
                stack.pop_back();
                break;
            case parsetable::entry::ERROR:
                if (cur_token_class != EOI) {
                    panic(parsetable, stack, lex, cur_token, prev_production,
                          cur_symbol, ERROR_PRODUCTION, input_stream, prev_is_production, errors);
                } else {
                    panic(parsetable, stack, lex, cur_token, prev_production,
                          cur_symbol, REACHED_EOF, input_stream, prev_is_production, errors);
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
