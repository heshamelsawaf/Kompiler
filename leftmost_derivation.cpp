#include "leftmost_derivation.h"

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
    for (const sentential_expression &expr : derivation.expressions) {
        stream << expr << "\n";
    }
    if (!derivation.errors.empty()) {
        stream << "\n" << "Errors: \n";
        for (const error & err : derivation.errors) {
            stream << err << std::endl;
        }
    }
    return stream;
}

