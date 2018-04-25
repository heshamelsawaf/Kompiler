#include "sentential_expression.h"

sentential_expression::sentential_expression(std::vector<std::string> _symbols, std::string _production) {
    symbols = _symbols;
    production = _production;
}

std::vector<std::string> sentential_expression::get_symbols() const {
    return symbols;
}

std::string sentential_expression::get_production() const {
    return production;
}

std::string sentential_expression::get_symbols_str() const {
    std::string str = "";
    for (const std::string &sym : symbols) {
        str += sym + " ";
    }
    return str;
}

std::ostream &operator<<(std::ostream &stream, const sentential_expression &expr) {
    for (std::string symbol : expr.symbols) {
        stream << symbol << ' ';
    }
    stream << '\t' << expr.production;
    return stream;
}
