//
// Created by hsawaf on 4/20/18.
//

#ifndef KOMPILER_CFG_H
#define KOMPILER_CFG_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <list>

class cfg {

    class symbol {

        class production {

            std::list<symbol *> symbols;

        public:
            void add_symbol(symbol *sym);

            std::list<symbol *> get_symbols(void) const;
        };

        bool terminal;
        std::string key;
        std::list<production> productions;
        std::unordered_set<std::string> first;
        std::unordered_set<std::string> follow;

    public:
        symbol(std::string _key, bool _terminal = false);

        bool is_terminal(void) const;

        std::string get_key(void) const;

        void add_production(production _production);

        void add_first(std::string _key);

        void add_follow(std::string _key);

        bool contains_first(std::string _key) const;

        bool contains_follow(std::string _key) const;

        std::unordered_set<std::string> get_first() const;

        std::unordered_set<std::string> get_follow() const;

    };

    std::unordered_map<std::string, symbol> symbols;
    std::string grammar;

public:
    cfg(std::string _grammar);

    /* Adds and returns the symbol c by _key if it does not exist.
     * if it does, the identified symbol is simply returned.
     */
    symbol *add_symbol(std::string _key, bool _terminal = false);

    /* Adds a new production identified as lhs -> rhs;
     * rhs is a list of all symbols in the production.
     * all the symbols will be added to the cfg symbols automatically if they do not exist.
     * */
    bool add_production(std::string lhs, std::list<std::string> &rhs);


};


#endif //KOMPILER_CFG_H
