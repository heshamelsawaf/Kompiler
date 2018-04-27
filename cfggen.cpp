//
// Created by hsawaf on 4/21/18.
//

//#include "test/OptionPrinter.hpp"
//
//#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
//
//#include <iostream>
//#include <string>
//
//namespace
//{
//    const size_t ERROR_IN_COMMAND_LINE = 1;
//    const size_t SUCCESS = 0;
//    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
//
//} // namespace
//
////-----------------------------------------------------------------------------
//int main(int argc, char** argv)
//{
//    try
//    {
//        std::string appName = boost::filesystem::basename(argv[0]);
//        int add = 0;
//        int like = 0;
//        std::vector<std::string> sentence;
//
//        /** Define and parse the program options
//         */
//        namespace po = boost::program_options;
//        po::options_description desc("Options");
//        desc.add_options()
//                ("help,h", "Print help messages")
//                ("verbose,v", "print words with verbosity")
//                ("word,w", po::value<std::vector<std::string> >(&sentence),
//                 "words for the sentence, specify multiple times")
//                (",t", "just a temp option that does very little")
//                ("necessary,n", po::value<std::string>()->required(), "give me anything")
//                ("manual,m", po::value<std::string>(), "extract value manually")
//                ("add", po::value<int>(&add)->required(), "additional options")
//                ("like", po::value<int>(&like)->required(), "this");
//
//        po::positional_options_description positionalOptions;
//        positionalOptions.add("add", 1);
//        positionalOptions.add("like", 1);
//
//        po::variables_map vm;
//
//        try
//        {
//            po::store(po::command_line_parser(argc, argv).options(desc)
//                              .positional(positionalOptions).run(),
//                      vm); // throws on error
//
//            /** --help option
//             */
//            if ( vm.count("help")  )
//            {
//                std::cout << "This is just a template app that should be modified"
//                          << " and added to in order to create a useful command line"
//                          << " application" << std::endl << std::endl;
//                    rad::OptionPrinter::printStandardAppDesc(appName,
//                                                         std::cout,
//                                                         desc,
//                                                         &positionalOptions);
//                return SUCCESS;
//            }
//
//            po::notify(vm); // throws on error, so do after help in case
//            // there are any problems
//        }
//        catch(boost::program_options::required_option& e)
//        {
//            rad::OptionPrinter::formatRequiredOptionError(e);
//            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
//            rad::OptionPrinter::printStandardAppDesc(appName,
//                                                     std::cout,
//                                                     desc,
//                                                     &positionalOptions);
//            return ERROR_IN_COMMAND_LINE;
//        }
//        catch(boost::program_options::error& e)
//        {
//            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
//            rad::OptionPrinter::printStandardAppDesc(appName,
//                                                     std::cout,
//                                                     desc,
//                                                     &positionalOptions);
//            return ERROR_IN_COMMAND_LINE;
//        }
//
//        // can do this without fear because it is required to be present
//        std::cout << "Necessary = "
//                  << vm["necessary"].as<std::string>() << std::endl;
//
//        if ( vm.count("verbose") )
//        {
//            std::cout << "VERBOSE PRINTING" << std::endl;
//        }
//        if (vm.count("verbose") && vm.count("t"))
//        {
//            std::cout << "heeeeyahhhhh" << std::endl;
//        }
//
//        std::cout << "Required Positional, add: " << add
//                  << " like: " << like << std::endl;
//
//        if ( sentence.size() > 0 )
//        {
//            std::cout << "The specified words: ";
//            std::string separator = " ";
//            if (vm.count("verbose"))
//            {
//                separator = "__**__";
//            }
//            for(size_t i=0; i<sentence.size(); ++i)
//            {
//                std::cout << sentence[i] << separator;
//            }
//            std::cout << std::endl;
//
//        }
//
//        if ( vm.count("manual") )
//        {
//            std::cout << "Manually extracted value: "
//                      << vm["manual"].as<std::string>() << std::endl;
//        }
//
//    }
//    catch(std::exception& e)
//    {
//        std::cerr << "Unhandled Exception reached the top of main: "
//                  << e.what() << ", application will now exit" << std::endl;
//        return ERROR_UNHANDLED_EXCEPTION;
//
//    }
//
//    return SUCCESS;
//
//} // main

//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include "cfgparser.h"
#include "parsetable.h"
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
    ifstream grammar_ifs;

    if (argc < 2) {
        perror("Invalid number of arguments!\n USAGE: CFG <grammar_file> <parsetable_output_file>");
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
    // std::cout << _cfg << std::endl;

    _cfg.to_ll1();
    _cfg.build();

    parsetable ptab(_cfg);

    grammar_ifs.close();


    return 0;
}

