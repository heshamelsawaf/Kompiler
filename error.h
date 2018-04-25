#ifndef KOMPILER_ERROR_H
#define KOMPILER_ERROR_H

#include <string>
#include <iostream>

class error {
private:
    std::string message;
    int line;
    int col;
    friend std::ostream &operator<<(std::ostream &stream, const error &t);
public:
    error(int _line, int _col, std::string _msg);
};

#endif //KOMPILER_ERROR_H