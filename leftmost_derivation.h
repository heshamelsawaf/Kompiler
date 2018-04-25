#ifndef KOMPILER_LEFTMOST_DERIVATION_H
#define KOMPILER_LEFTMOST_DERIVATION_H

#include "sentential_expression.h"
#include "error.h"

class leftmost_derivation {
private:
    std::vector<sentential_expression> expressions;

    std::vector<error> errors;
    
    friend std::ostream &operator<<(std::ostream &stream, const leftmost_derivation &derivation);

public:
    leftmost_derivation(std::vector<sentential_expression> _expressions, std::vector<error> _errors);

    std::vector<sentential_expression> get_expressions();

    std::vector<error> get_errors();
};


#endif //KOMPILER_LEFTMOST_DERIVATION_H
