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

public:

    class symbol {
    public:
        class production {

            /* A production is represented by an ordered
             *  list of symbols */
            std::list<symbol *> symbols;
            
            /* Each production has a first set, which is a set
             * of terminals that can be matched from the non-terminal
             * at a given input */
            std::unordered_set<std::string> first;

            /* Key of the nonterminal lhs of this production */
            std::string lhs;

        public:
            /* Appends symbol to production */
            void add_symbol(symbol *sym);
            
            /* Return list representation of produciton. */
            std::list<symbol *> get_symbols(void) const;
            
            /* Add a symbol to the first set of this production */
            /* TODO: add_first can be implemented internally, hiding
             *       implementaiton details, to disallow inconsistency 
             *       caused by manipulation outside cfg class */
            bool add_first(std::string _key);
        };

        symbol(std::string _key, bool _terminal = false);

        bool is_terminal(void) const;

        std::string get_key(void) const;

        void add_production(production _production);

        void add_follow(std::string _key);

        bool contains_first(std::string _key) const;

        bool contains_follow(std::string _key) const;
        
        bool is_eps() const;

        std::unordered_set<std::string> get_first() const;

        std::unordered_set<std::string> get_follow() const;

        std::list<production> get_productions();

    private:
        bool terminal;
        std::string key;
        std::list<production> productions;
        std::unordered_set<std::string> follow;

    };

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

    symbol *get_symbol(std::string _key);

    std::list<std::string> get_symbols();

    /* Builds CFG by computing first and follow sets for
     * each non-terminal, which are later used to build
     * parsing table */
    void build();

private:
    std::unordered_map<std::string, symbol> symbols;
    std::string grammar;

};


#endif //KOMPILER_CFG_H