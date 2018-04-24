#include "error.h"


std::ostream &operator<<(std::ostream &stream, const error &error) {
    stream << error.line << ":" << error.col << ": " << error.message;
    return stream;
}

error::error(int _line, int _col, std::string _msg) {
    line = _line;
    col = _col;
    message = _msg;
}
