#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "../cfgparser.h"
#include <algorithm>

using namespace std;

TEST(FIRST, ALL1) {
    ifstream grammar_ifs;

    grammar_ifs.open("lan2.cfg");

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
        return;

    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    cfgparser _cfgparser;

    cfg _cfg = _cfgparser.rules2cfg(s);
    std::cout << "Before:\n\n" << _cfg << std::endl;
    _cfg.to_ll1();
    std::cout << "=======================\nAfter:\n\n" << _cfg << std::endl;


    grammar_ifs.close();
}