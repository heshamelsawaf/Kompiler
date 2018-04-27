//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "parsetable.h"
#include "cfg.h"
#include "cfgparser.h"

using namespace std;

std::string read_buf(std::istream &grammar_ifs) {
    std::ostringstream ss;
    ss << grammar_ifs.rdbuf();
    return ss.str();
}

int main(int argc, char **argv) {

    ifstream grammar_ifs;
    std::string ptab_out;
    std::string s;
    std::string usage = "USAGE:\n\n-./parsergen <language_rules_file> <parser_table_output>\n\n\
-./parsergen <parser_table_output>\n\treads language rules from stdin\n\n\
-./parsergen\n\treads from stdin and outputs to ptab.in\n";

    if(argc == 3) {
        ptab_out = argv[2];
        grammar_ifs.open(argv[1]);
        s = read_buf(grammar_ifs);
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            std::cout << usage << std::endl;
            return 0;
        }
        ptab_out = argv[2];
        s = read_buf(std::cin);
    } else if (argc == 1) {
        ptab_out = "ptab.in";
        s = read_buf(std::cin);
        
    } else {
        std::string err_msg = "Invalid number of arguments!\n" + usage;
        perror(err_msg.c_str());
        errno = EINVAL;
        exit(-1);
    }

    if (s.empty()) {
        std::cout << "Error: Input File is Empty" << std::endl;
        return -1;
    }

    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    cfgparser _cfgparser;

    cfg _cfg = _cfgparser.rules2cfg(s);

    _cfg.to_ll1();
    _cfg.build();
    
    parsetable ptab(_cfg);
    ptab.serialize(ptab_out);
}

