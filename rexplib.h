//
// Created by Marwan Tammam on 3/20/18.
//

#ifndef KOMPILER_REXP_LIB_H
#define KOMPILER_REXP_LIB_H

#define EPS_CHAR 0x1

#include "machine.h"

namespace machine_ops {
    machine concat(std::vector<machine> &);

    machine concat(const machine &, const machine &);

    machine oring(std::vector<machine> &);

    machine oring(const machine &, const machine &);

    machine star(const machine &);

    machine plus(const machine &);

    machine single_char(char);

    machine char_range(char, char);

    machine string_concat(std::string);
}

#endif //KOMPILER_REXP_LIB_H
