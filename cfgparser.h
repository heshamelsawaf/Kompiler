//
// Created by hsawaf on 4/21/18.
//

#ifndef KOMPILER_CFGPARSER_H
#define KOMPILER_CFGPARSER_H

#include "cfg.h"

class cfgparser {

    void add_production_to_cfg(cfg &_cfg, std::string _lhs, std::vector<std::string> &_rhs);

    std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);

    bool is_terminal(std::string s);

public:
    cfg rules2cfg(std::string rules);
};


#endif //KOMPILER_CFGPARSER_H
