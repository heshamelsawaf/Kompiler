//
// Created by hsawaf on 4/20/18.
//

#ifndef KOMPILER_CFG_H
#define KOMPILER_CFG_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>

#define EPS std::string(1, 0x01)
#define EOI std::string("$")

class cfg {

public:

    class symbol {
    public:
        class production {

            /* A production is represented by an ordered
             *  list of symbols */
            std::vector<symbol *> symbols;

            /* Each production has a first set, which is a set
             * of terminals that can be matched from the non-terminal
             * at a given input */
            std::unordered_set<std::string> first;

            /* Key of the nonterminal lhs of this production */
            std::string lhs;

            /* Overloading the ostream operator */
            friend std::ostream &operator<<(std::ostream& stream, const production &prod);

        public:
            /* Default Constructor */
            production();
            /* Construct production from list of symbols and rhs */
            production(std::string _lhs, std::vector<symbol *> symbols);
            /* Appends symbol to production */
            void add_symbol(symbol *sym);

            /* Return list representation of production. */
            std::vector<symbol *> get_symbols(void);

            std::unordered_set<std::string> get_first() const;

            /* Add a symbol to the first set of this production */
            /* TODO: add_first can be implemented internally, hiding
             *       implementaiton details, to disallow inconsistency
             *       caused by manipulation outside cfg class */
            bool add_first(std::string _key);

            bool contains_first(std::string _key) const;

        };

        symbol();

        symbol(std::string _key, bool _terminal = false);

        bool is_terminal(void) const;

        bool is_eps(void) const;

        std::string get_key(void) const;

        void add_production(production _production);

        void add_production(std::vector<symbol *> &rhs);

        void clear_productions();

        bool add_follow(std::string _key);

        bool contains_first(std::string _key) const;

        bool contains_follow(std::string _key) const;

        std::unordered_set<std::string> get_first() const;

        std::unordered_set<std::string> get_follow() const;

        std::vector<production> get_productions();

        production &operator[](std::size_t idx);

        std::size_t get_production_count() const;

    private:
        bool terminal;
        std::string key;
        std::vector<production> productions;
        std::unordered_set<std::string> follow;

    };

    cfg(std::string _grammar);

    /* Adds and returns the symbol c by _key if it does not exist.
     * if it does, the identified symbol is simply returned.
     */
    symbol *add_symbol(std::string _key, bool _terminal = false);

    /* Adds a new production identified as lhs -> rhs;
     * rhs is a list of all symbol names in the production.
     * all the symbols will be added to the cfg symbols automatically if they do not exist.
     * */
    bool add_production(std::string lhs, std::vector<std::string> rhs);

    /* Adds a new production identified as lhs -> rhs;
     * rhs is a list of all symbols in the production.
     * all the symbols will be added to the cfg symbols automatically if they do not exist.
     * */
    bool add_production(std::string lhs, std::vector<symbol *> rhs);


    symbol *get_symbol(std::string _key);

    symbol *get_starting_symbol(void);

    void set_starting_symbol(symbol *s);


    std::vector<std::string> get_symbols();

    /* Tries to convert the cfg to LL(1)
    *  returns true if the cfg was converted successfully
    *  returns false if the cfg was already LL(1) */
    bool to_ll1();

    /* Builds CFG by computing first and follow sets for
     * each non-terminal, which are later used to build
     * parsing table */
    void build();

private:
    std::unordered_map<std::string, symbol> symbols;
    symbol *starting_symbol;
    std::string grammar;
    /* Overloading the ostream operator */
    friend std::ostream &operator<<(std::ostream& stream, cfg &grmr);

};


#endif //KOMPILER_CFG_H