#include "leftmost_derivation.h"
#include <iomanip>

leftmost_derivation::leftmost_derivation(std::vector<sentential_expression> _expressions,
                                         std::vector<error> _errors) {
    expressions = _expressions;
    errors = _errors;
}

std::vector<sentential_expression> leftmost_derivation::get_expressions() {
    return expressions;
}

std::vector<error> leftmost_derivation::get_errors() {
    return errors;
}

std::ostream &operator<<(std::ostream &stream, const leftmost_derivation &derivation) {
    int max_sentential = -1;
    int max_production = -1;
    for (const sentential_expression &expr : derivation.expressions) {
        max_production = std::max(max_production, (int) expr.get_production().length());
        max_sentential = std::max(max_sentential, (int) expr.get_symbols_str().length());
    }
    
    for (const sentential_expression &expr : derivation.expressions) {
        stream << std::setw(max_sentential + 4) << std::left << expr.get_symbols_str()
               << expr.get_production() << std::endl;   
    }

    if (!derivation.errors.empty()) {
        stream << "\n" << "Errors: \n";
        for (const error & err : derivation.errors) {
            stream << err << std::endl;
        }
    }
    return stream;
}

