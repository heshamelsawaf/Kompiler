//
// Created by hsawaf on 4/21/18.
//

#include "cfgparser.h"
#include <boost/tokenizer.hpp>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

static inline void find_and_replace(std::string &source, std::string const &find, std::string const &replace) {
    for (std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;) {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}

void cfgparser::replace_escapes(std::string &s, bool encode) {
    if (encode) {
        find_and_replace(s, "\\#", "0x23"), escapes["0x23"] = "\\#";
        find_and_replace(s, "\\|", "0x7C"), escapes["0x7C"] = "\\|";
    } else {
        for (auto &&entry : escapes)
            find_and_replace(s, entry.first, entry.second);
    }
}


cfg cfgparser::rules2cfg(std::string rules) {
    int productions_cnt = 0;
    cfg _cfg("JAVA");
    replace_escapes(rules, true);
    boost::tokenizer<boost::escaped_list_separator<char>>
            tok(rules, boost::escaped_list_separator<char>("", "#", "\""));
    for (auto i : tok) {
        std::vector<std::string> lhs_and_rhs = split(i, "::=");
        if (lhs_and_rhs.empty())
            continue;
        if (lhs_and_rhs.size() < 2) {
            std::cerr << "Error in production '" << i << "'\n";
            std::cerr << "Make sure you follow the grammar input file specification.";
            exit(EXIT_FAILURE);
        }
        std::string _lhs = trim_copy(lhs_and_rhs[0]);
        std::string _rhs;
        for (size_t j = 1; j < lhs_and_rhs.size(); j++)
            _rhs += lhs_and_rhs[j];

        boost::tokenizer<boost::escaped_list_separator<char>>
                rhs_tok(_rhs, boost::escaped_list_separator<char>("", "|", "\""));
        for (auto j : rhs_tok) {
            j = trim_copy(j);
            if (j.empty()) {
                std::cerr << "Error in token '" << j << "' in production '" << i << "'\n";
                std::cerr << "Make sure you follow the grammar input file specification.";
                exit(EXIT_FAILURE);
            }

            std::vector<std::string> symbols;
            boost::tokenizer<boost::escaped_list_separator<char>>
                    prod_tok(j, boost::escaped_list_separator<char>("", " ", "\""));
            for (auto k : prod_tok) {
                if (k.empty()) {
                    std::cerr << "Error in symbol '" << k << "' in token '" << j << "' in production '" << i << "'\n";
                    std::cerr << "Make sure you follow the grammar input file specification.";
                    exit(EXIT_FAILURE);
                }

                symbols.push_back(k);
            }

            add_production_to_cfg(_cfg, _lhs, symbols);
        }
        if (!productions_cnt)
            _cfg.set_starting_symbol(_cfg.add_symbol(_lhs));
        productions_cnt++;
    }
    return _cfg;
}

std::vector<std::string> cfgparser::split(std::string stringToBeSplitted, std::string delimeter) {
    std::vector<std::string> splittedString;
    int startIndex = 0;
    int endIndex = 0;
    while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size()) {

        std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
        splittedString.push_back(val);
        startIndex = endIndex + delimeter.size();

    }
    if (startIndex < stringToBeSplitted.size()) {
        std::string val = stringToBeSplitted.substr(startIndex);
        splittedString.push_back(val);
    }
    return splittedString;
}


bool cfgparser::is_terminal(std::string s) {
    return s.length() > 2 && s.at(0) == '\'' && s.back() == '\'';
}

void cfgparser::add_production_to_cfg(cfg &_cfg, std::string _lhs, std::vector<std::string> &_rhs) {
    _cfg.add_symbol(_lhs);

    for (std::string &s: _rhs) {
        if (is_terminal(s)) {
            s = s.substr(1, s.length() - 2);
            replace_escapes(s, false);

            if (!s.compare("\\L"))
                s = EPS;
            else if (s.at(0) == '\\')
                s = s.substr(1, s.length() - 1);

            _cfg.add_symbol(s, true);
        }
        else
            _cfg.add_symbol(s);
    }

    _cfg.add_production(_lhs, _rhs);
}