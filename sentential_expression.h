#ifndef SENTENTIAL_EXPRESSION_H
#define SENTENTIAL_EXPRESSION_H

#include <vector>
#include <string>
#include <iostream>

class sentential_expression {
private:
    std::vector<std::string> symbols;
    
    std::string production;

    friend std::ostream &operator<<(std::ostream &stream, const sentential_expression &expr);
public:
    sentential_expression(std::vector<std::string> _symbols, std::string _production);

    std::vector<std::string> get_symbols();
    
    std::string get_production();
};

#endif //SENTENTIAL_EXPRESSION_H
