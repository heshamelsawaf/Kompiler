//
// Created by Marwan Tammam on 3/20/18.
//

#ifndef KOMPILER_REXP_LIB_H
#define KOMPILER_REXP_LIB_H

#include "machine.h"

machine machine_concat(const machine &a_, const machine &b_);

machine machine_or(const machine &a_, const machine &b_);

machine machine_star(const machine &a_);

machine machine_plus(const machine &a_);

machine single_char(char c);

machine char_range(char start, char end);

machine string_concat(std::string s);

#endif //KOMPILER_REXP_LIB_H
