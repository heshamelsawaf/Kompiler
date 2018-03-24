#include "machine.h"
#include "lexer.h"
#include "parser.h"
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>

int lex(std::istream &ttab_in, std::istream &src_in, std::ostream &symtab_out);

int main(int argc, char** argv) {
    std::ifstream ttab_ifs;
    std::ifstream src_ifs;
    std::ofstream symtab_ofs;
    int retval = 0;
    std::string usage = "USAGE:\n\n-./lex <transition_table_input_file> <src_input_file>\
<symbol_table_output_file>\n\n\
-./lex <transition_table_input_file> <src_input_file>\n\toutputs symbol table to stdout\n\n\
-./lex <transition_table_input_file>\n\treads src from stdin and outputs symbol table to stdout\n\n\
-./lexgen\n\treads transition table from ttab.in, src from stdin and outputs symbol table to stdout\n";
    if (argc == 1) {
        ttab_ifs.open("ttab.in");
        retval = lex(ttab_ifs, std::cin, std::cout);
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            std::cout << usage << std::endl;
            return 0;
        }
        ttab_ifs.open(argv[1]);
        retval = lex(ttab_ifs, std::cin, std::cout);
    } else if (argc == 3) {
        ttab_ifs.open(argv[1]);
        src_ifs.open(argv[2]);
        retval = lex(ttab_ifs, src_ifs, std::cout);
    } else if (argc == 4) {
        ttab_ifs.open(argv[1]);
        src_ifs.open(argv[2]);
        symtab_ofs.open(argv[3]);
        retval = lex(ttab_ifs, src_ifs, symtab_ofs);
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

int lex(std::istream &ttab_in, std::istream &src_in, std::ostream &symtab_out) {
    try {
        if (!ttab_in | !src_in | !symtab_out) {
            perror("Unable to read file");
            return -1;
        }
        machine m("");
        read_ttab(ttab_in, m);
        lexer lex(m);
        parser prs(&src_in, lex);

        std::vector<lexer::token> v = prs.parse();

        for (lexer::token tok : v) {
            symtab_out << tok << std::endl;
        }

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