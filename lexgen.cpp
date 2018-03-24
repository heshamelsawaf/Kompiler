#include "dfa.h"
#include "rexparser.h"
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>

int lexgen(std::istream &in, std::ostream &out);

int main(int argc, char** argv) {
    std::ifstream specs_ifs;
    std::ofstream ttab_ofs;
    int retval = 0;
    std::string usage = "USAGE:\n\t-./lexgen <specs_file> <transition_table_output>\n\
\t-./lexgen <specs_file>\toutputs to stdout\n\
\t-./lexgen\treads from stdin and outputs to stdout";
    if (argc == 1) {
        retval = lexgen(std::cin, std::cout);
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            std::cout << usage << std::endl;
            return 0;
        }
        specs_ifs.open(argv[1]);
        retval = lexgen(specs_ifs, std::cout);
        specs_ifs.close();
    } else if (argc == 3) {
        specs_ifs.open(argv[1]);
        ttab_ofs.open(argv[2]);
        retval = lexgen(specs_ifs, ttab_ofs);
        specs_ifs.close();
        ttab_ofs.close();
    } else {
        std::string err_msg = "Invalid number of arguments!\n" + usage;
        perror(err_msg.c_str());
        errno = EINVAL;
        exit(-1);
    }
    return retval;
}

std::string read_istream(std::istream &in) {
    return std::string(std::istreambuf_iterator<char>(in), {});
}

int lexgen(std::istream &in, std::ostream &out) {
    try {
        if (!in | !out) {
            perror("Unable to read file");
            return -1;
        }
        std::string specs = read_istream(in);
        rexparser rx;
        machine nfa = rx.rules2nfa(specs);
        machine dfa = dfa::to_dfa(nfa);
        machine min_dfa = dfa::minimize_dfa(dfa);
        out << min_dfa;
        return 0;
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    } catch (const std::string& ex) {
        std::cout << ex << std::endl;
        return -1;
    } catch (...) {
        return -1;
    }
}