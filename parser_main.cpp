#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

#include "options/OptionPrinter.hpp"
#include "machine.h"
#include "parsetable.h"
#include "leftmost_derivation.h"
#include "ll1_parser.h"

namespace {
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace

namespace po = boost::program_options;
po::variables_map vm;
std::string appName;
std::string app_description;
std::string src;
std::ifstream ttab_ifs;
std::string ptab_ifs; // TODO: Change serialization to take input stream
std::ifstream src_ifs;
std::ofstream lmost_derivation_ofs;
int retval = 0;

int stack_parse(std::istream &ttab_in, std::string ptab_in, std::istream &src_in, std::ostream &lmost_derivation_out);

void handle_options();

void handle_transition();

void handle_parse_table();

void handle_output();

void handle_src();


//-----------------------------------------------------------------------------
int main(int argc, char **argv) {
    try {

        appName = boost::filesystem::basename(argv[0]);
        app_description = "A simple java Compiler";

        /** Define and parse the program options
         */
        po::options_description desc("Options");
        desc.add_options()
                ("help,h", "Print (on the standard output) a description of the command-line options understood"
                           " by Kompiler.")
                ("verbose,v", "Verbosely list files processed.")
                ("transition,t", po::value<std::string>(), "Input transition-table file to read from.\n"
                                                           "\n"
                                                           "\t                    \tIf -t is not specified, the default"
                                                           " is to read from in.ttab file.")
                ("parse-table,p", po::value<std::string>(), "Input parse-table file to read from.\n"
                                                            "\n"
                                                            "\t                    \tIf -p is not specified, the default"
                                                            " is to read from in.ptab file.")
                ("src", po::value<std::string>(&src)->required(), "Input src file to be compiled.")
                ("output,o", po::value<std::string>(),
                 "Place output in specified file.  This applies to whatever sort of output\n"
                 "\t                    \tis being produced.\n"
                 "\n"
                 "\t                    \tIf -o is not specified, the default is to print on the standard output.")
                ("version", "Display the version number and copyrights of the invoked Kompiler.");

        po::positional_options_description positionalOptions;
        positionalOptions.add("src", 1);
//        positionalOptions.add("like", 1);


        try {
            po::store(po::command_line_parser(argc, argv).options(desc)
                              .positional(positionalOptions).run(),
                      vm);

            /** --help option
             */
            if (vm.count("help")) {
                rad::OptionPrinter::printStandardAppDesc(appName,
                                                         app_description,
                                                         std::cout,
                                                         desc,
                                                         &positionalOptions);
                return SUCCESS;
            }

            po::notify(vm); // throws on error, so do after help in case
            // there are any problems
        }
        catch (boost::program_options::required_option &e) {
            rad::OptionPrinter::formatRequiredOptionError(e);
            std::cerr << "ERROR: " << e.what() << std::endl;
//      rad::OptionPrinter::printStandardAppDesc(appName,
//                                               app_description,
//                                               std::cout,
//                                               desc,
//                                               &positionalOptions);
            std::cerr << "Try '" << appName << " --help' for more information.";
            return ERROR_IN_COMMAND_LINE;
        }
        catch (boost::program_options::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
//      rad::OptionPrinter::printStandardAppDesc(appName,
//                                               app_description,
//                                               std::cout,
//                                               desc,
//                                               &positionalOptions);
            std::cerr << "Try '" << appName << " --help' for more information.";
            return ERROR_IN_COMMAND_LINE;
        }

        // can do this without fear because it is required to be present
//        std::cout << "Necessary = "
//                  << vm["necessary"].as<std::string>() << std::endl;

        if (vm.count("verbose")) {
            std::cout << "VERBOSE PRINTING" << std::endl;
        }

        handle_options();
        if (lmost_derivation_ofs.is_open())
            stack_parse(ttab_ifs, ptab_ifs, src_ifs, lmost_derivation_ofs);
        else
            stack_parse(ttab_ifs, ptab_ifs, src_ifs, std::cout);
    }
    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;

    }

    return SUCCESS;

} // main

void handle_options() {
    handle_transition();
    handle_parse_table();
    handle_output();
    handle_src();
}

void handle_transition() {
    if (vm.count("transition")) {
        ttab_ifs.open(vm["transition"].as<std::string>());
    } else {
        ttab_ifs.open("in.ttab");
    }
}

void handle_parse_table() {
    if (vm.count("parse-table")) {
        ptab_ifs = (vm["parse-table"].as<std::string>());
    } else {
        ptab_ifs = ("in.ptab");
    }
}

void handle_output() {
    if (vm.count("output"))
        lmost_derivation_ofs.open(vm["output"].as<std::string>());
}

void handle_src() {
    src_ifs.open(src);
}

void read_ttab(std::istream &in, machine &m) {
    in >> m;
}

int stack_parse(std::istream &ttab_in, std::string ptab_in, std::istream &src_in, std::ostream &lmost_derivation_out) {
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
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    } catch (const std::string &ex) {
        std::cout << ex << std::endl;
        return -1;
    } catch (...) {
        return -1;
    }
}