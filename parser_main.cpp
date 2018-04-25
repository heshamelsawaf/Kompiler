#include "machine.h"
#include "lexer.h"
#include "parser.h"
#include "ll1_parser.h"
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>

int stack_parse(std::istream &ttab_in, std::string ptab_in, std::istream &src_in, std::ostream &lmost_derivation_out);

int main(int argc, char** argv) {
    std::ifstream ttab_ifs;
    std::string ptab_ifs; // TODO: Change serialization to take input stream
    std::ifstream src_ifs;
    std::ofstream lmost_derivation_ofs;
    int retval = 0;
    std::string usage = "USAGE:\n\n-./parser <transition_table_input_file> <parsetable_input_file> <src_input_file>\
<leftmost_derivation_output>\n\n\
-./parser <transition_table_input_file> <parsetable_input_file> <src_input_file>\n\toutputs leftmost_derivation to stdout\n\n\
-./parser <transition_table_input_file> <parsetable_input_file>\n\treads src from stdin and outputs leftmost_derivation to stdout\n\n\
-./parser <parsetable_input_file>\n\treads transition table from ttab.in, src from stdin and outputs leftmost_derivation to stdout\n";
    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            std::cout << usage << std::endl;
            return 0;
        }
        ttab_ifs.open("ttab.in");
        ptab_ifs = argv[1];
        retval = stack_parse(ttab_ifs, ptab_ifs, std::cin, std::cout);
    } else if (argc == 3) {
        ttab_ifs.open(argv[1]);
        ptab_ifs = argv[2];
        retval = stack_parse(ttab_ifs, ptab_ifs, std::cin, std::cout);
    } else if (argc == 4) {
        ttab_ifs.open(argv[1]);
        ptab_ifs = argv[2];
        src_ifs.open(argv[3]);
        retval = stack_parse(ttab_ifs, ptab_ifs, src_ifs, std::cout);
    } else if (argc == 5) {
        ttab_ifs.open(argv[1]);
        ptab_ifs = argv[2];
        src_ifs.open(argv[3]);
        lmost_derivation_ofs.open(argv[4]);
        retval = stack_parse(ttab_ifs, ptab_ifs, src_ifs, lmost_derivation_ofs);
    } else {
        std::string err_msg = "Invalid number of arguments!\n" + usage;
        perror(err_msg.c_str());
        errno = EINVAL;
        exit(-1);
    }
    return retval;
}

void read_ttab(std::istream &in, machine &m) {
    in >> m;
}

int parse(std::istream &ttab_in, std::string ptab_in, std::istream &src_in, std::ostream &lmost_derivation_out) {
    try {
        if (!ttab_in | !src_in | !lmost_derivation_out) {
            perror("Unable to read file");
            return -1;
        }
        machine m("");
        read_ttab(ttab_in, m);

        parsetable ptab;

        ptab.deserialize(ptab_in);

        leftmost_derivation derivation = parse::parse_ll1(ptab, m, src_in);

        lmost_derivation_out << derivation << std::endl;

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