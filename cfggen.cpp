//
// Created by hsawaf on 4/21/18.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "cfgparser.h"
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
    ifstream grammar_ifs;

    if (argc < 2) {
        perror("Invalid number of arguments!\n USAGE: CFG <grammar_file>");
        errno = EINVAL;
        exit(-1);
    }

    grammar_ifs.open(argv[1]);

    if (!grammar_ifs) {
        perror("Unable to read file");
        exit(-1);
    }

    auto s = [&grammar_ifs] {
        std::ostringstream ss;
        ss << grammar_ifs.rdbuf();
        return ss.str();
    }();

    if (s.empty())
        return 0;

    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    cfgparser _cfgparser;

    cfg _cfg = _cfgparser.rules2cfg(s);
    std::cout << _cfg << std::endl;

    grammar_ifs.close();


    return 0;
}

