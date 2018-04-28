#include "cfg.h"

/* Utility function to find each production A->B where B
 * is non-terminal and perform: FIRST(A) = FIRST(A) U FIRST(B).
 * @return true if a change has occured, false otherwise */
bool build_first_util(cfg *grmr) {
    bool updated = false;

    for (std::string sym_key : grmr->get_symbols()) {
        cfg::symbol *s = grmr->get_symbol(sym_key);

        if (s->is_terminal())
            continue;

        for (int i = 0; i < s->get_production_count(); i++) {
            cfg::symbol::production &prod = (*s)[i];

            /* For each production S->T where T is non-terminal,
             * Add FIRST(T) to FIRST(S), do not add eps if it is
             * in FIRST(T) */
            std::vector<cfg::symbol *> symbols = prod.get_symbols();
            std::vector<cfg::symbol *>::iterator t = symbols.begin();
            std::vector<cfg::symbol *>::iterator eol = symbols.end();

            if (!(*t)->is_terminal())
                for (std::string a : (*t)->get_first())
                    if (a != EPS)
                        updated |= prod.add_first(a);

            /* For production S -> X1 X2 X3... Xk, if eps is in
             * FIRST(X1..k), then eps is in FIRST(S) */
//            while ((*t)->contains_first(EPS)){
//                t++;
//
//            }

            while (t != eol && (*t)->contains_first(EPS)) {
                t++;
                if (t == eol)
                    updated |= prod.add_first(EPS);
                else {
                    if ((*t)->is_terminal())
                        updated |= prod.add_first((*t)->get_key());
                    else
                        for (std::string a : (*t)->get_first())
                            updated |= prod.add_first(a);
                }
            }

//            /* If reached end of production, then eps is in
//             * FIRST(S), else if it reached Xi, then FIRST(Xi)
//             * is in FIRST(S) */
//            if (t == eol)
//                updated |= prod.add_first(EPS);
//            else {
//                if ((*t)->is_terminal())
//                    updated |= prod.add_first((*t)->get_key());
//                else
//                    for (std::string a : (*t)->get_first())
//                        updated |= prod.add_first(a);
//            }
        }
    }

    return updated;
}

/* Build first set for each and every production
 * in cfg grammar */
void build_first(cfg *grmr) {
    /* Initialize FIRST sets for non-terminals */
    for (std::string sym_key : grmr->get_symbols()) {
        cfg::symbol *s = grmr->get_symbol(sym_key);

        /* If symbol S is terminal, then FIRST(S) = { S } */
        if (s->is_terminal())
            continue;

        /* If symbol S is non-terminal, add all terminals in the
         * beginning of its productions (including EPS) to its first set */
        for (int i = 0; i < s->get_production_count(); i++) {
            cfg::symbol::production &prod = (*s)[i];
            cfg::symbol *t = prod.get_symbols().front();

            /* If S -> a(X) is a production where a is terminal,
             * add a to FIRST(S) */
            /* If S -> eps is a production then add eps to FIRST(S) */
            if (t->is_terminal() || t->is_eps())
                prod.add_first(t->get_key());
        }
    }

    /* For non-terminals S, A where A->S, FIRST(S) is
     * a subset of FIRST(A), add all of FIRST(A) to FIRST(S),
     * repeat until no additional changes can be made */
    while (build_first_util(grmr));
}

bool build_follow_util(cfg *grmr) {
    bool updated = false;

    for (std::string s_key : grmr->get_symbols()) {
        cfg::symbol *s = grmr->get_symbol(s_key);

        /* If symbol S is terminal, skip */
        if (s->is_terminal())
            continue;

        for (int i = 0; i < s->get_production_count(); i++) {
            cfg::symbol::production &prod = (*s)[i];
            std::vector<cfg::symbol *> symbols = prod.get_symbols();
            std::vector<cfg::symbol *>::iterator t = symbols.begin();
            std::vector<cfg::symbol *>::iterator eol = symbols.end();

            while (t != eol) {
                /* Skip any terminals or epsilon productions */
                if ((*t)->is_terminal() || (*t)->is_eps()) {
                    t++;
                    continue;
                }

                /* t is a nonterminal inside the production,
                 * u is the next symbol */
                std::vector<cfg::symbol *>::iterator u = t + 1;

                while (u != eol) {
                    /* If u is found to be a terminal, then add this terminal
                     * to FOLLOW(T) and break */
                    if ((*u)->is_terminal()) {
                        updated |= (*t)->add_follow((*u)->get_key());
                        break;
                    }

                    /* If u is found to be a non-terminal,
                     * add FIRST(U) - except for eps - to FOLLOW(T) */
                    for (std::string f : (*u)->get_first())
                        if (f != EPS)
                            updated |= (*t)->add_follow(f);

                    /* Advance if eps is in FIRST(u) */
                    if ((*u)->contains_first(EPS))
                        u++;
                    else
                        break;
                }

                /* If all of u contains eps, FOLLOW(S) C FOLLOW(T) */
                if (u == eol)
                    for (std::string f : s->get_follow())
                        updated |= (*t)->add_follow(f);
                t++;
            }
        }
    }

    return updated;
}

void build_follow(cfg *grmr) {
    /* Add EOF to starting symbol */
    cfg::symbol *s = grmr->get_starting_symbol();
    if (s == nullptr)
        throw std::invalid_argument("Can't find a starting symbol!");

    s->add_follow(EOI);

    while (build_follow_util(grmr));
}

int find_prefix(cfg::symbol::production &a, cfg::symbol::production &b) {

    int len = std::min(a.get_symbols().size(), b.get_symbols().size());
    int i = 0;

    for (i = 0 ; i < len ; i++) {
        if (a.get_symbols()[i] != b.get_symbols()[i])
            return i;
    }

    return i;
}

std::string add_aux_sym(cfg *grmr, std::string _sym_str) {

    if (grmr == nullptr)
        return "error";

    std::string new_sym_str = _sym_str;

    do {
        new_sym_str += "'";
    } while (grmr->get_symbol(new_sym_str) != nullptr);

    grmr->add_symbol(new_sym_str, false);

    return new_sym_str;
}

int left_factor(cfg *grmr, std::string _sym_str, std::vector<std::string> &symbol_names) {
    
    int new_symbols_cnt = 0;
    cfg::symbol *sym = grmr->get_symbol(_sym_str);

    if (grmr == nullptr || sym == nullptr || sym->get_productions().empty()) {
        return 0;
    }

    std::unordered_set<int> removed;
    std::vector<cfg::symbol::production> prods = sym->get_productions();
    int len = prods.size();

    for (int i = 0 ; i < len ; i++) {

        if (removed.find(i) != removed.end())
            continue;

        std::vector<int> common_prods;
        int prefix = 0;

        for (int j = i + 1 ; j < len ; j++) {

            if (removed.find(j) != removed.end())
                continue;

            int new_prefix = find_prefix(prods[i], prods[j]);

            if (new_prefix > 0) {
                common_prods.push_back(j);
                if (prefix == 0)
                    prefix = new_prefix;
                else
                    prefix = std::min(prefix, new_prefix);
            }
        }
        if (prefix > 0) {

            common_prods.push_back(i);
            removed.insert(common_prods.begin(), common_prods.end());

            // Add new symbol.
            new_symbols_cnt++;
            std::string new_sym_str = add_aux_sym(grmr, _sym_str);
            symbol_names.push_back(new_sym_str);

            bool eps_added = false;

            for (int prod : common_prods) {

                std::vector<cfg::symbol *> production_symbols = prods[prod].get_symbols();
                std::vector<cfg::symbol *> rhs(production_symbols.begin() + prefix, production_symbols.end());

                if (rhs.empty()) {
                    /*  Add eps once in case of repeated productions.
                    *   e.g. A -> Bc | Bc | Bcd
                    *   which becomes
                    *   A -> BcA'
                    *   A'-> eps | d
                    */
                    if (eps_added)
                        continue;
                    eps_added = true;
                    rhs.push_back(grmr->get_symbol(EPS));
                }
                grmr->add_production(new_sym_str, rhs);
            }
            // Add new production refering to symbol.
            std::vector<cfg::symbol *> prod_syms = prods[i].get_symbols();
            std::vector<cfg::symbol *> rhs(prod_syms.begin(),
                                           prod_syms.begin() + prefix);
            rhs.push_back(grmr->get_symbol(new_sym_str));

            // TODO: this should not be on the stack.
            cfg::symbol::production new_production(_sym_str, rhs);
            prods.push_back(new_production);
        }
    }
    if (new_symbols_cnt > 0) {

        sym->clear_productions();

        for (int i = 0 ; i < prods.size() ; i++) {
            if (removed.find(i) == removed.end()) {
                sym->add_production(prods[i]);
            }
        }
    }

    return new_symbols_cnt;
}

bool left_factor(cfg *grmr) {

    std::vector<std::string> symbols = grmr->get_symbols();
    int len = symbols.size();
    bool modified = false;

    for (int i = 0 ; i < len ; i++) {
        if (!grmr->get_symbol(symbols[i])->is_terminal()) {
            
            int new_syms_cnt = left_factor(grmr, symbols[i], symbols);
            len += new_syms_cnt;
            
            if (new_syms_cnt > 0)
                modified = true;
        }
    }

    return modified;
}

bool remove_immediate_left_recursion(cfg *grmr, std::string _sym_str) {

    cfg::symbol *sym = grmr->get_symbol(_sym_str);
    std::unordered_set<int> recursive_prods;
    std::vector<cfg::symbol::production> prods = sym->get_productions();

    for (int i = 0 ; i < prods.size() ; i++) {
        std::vector<cfg::symbol *> prod_symbols = prods[i].get_symbols();

        if (!prod_symbols.empty() && prod_symbols[0] == sym)
            recursive_prods.insert(i);
    }

    if (recursive_prods.empty())
        return false;
    // Handles cases like (A -> A | Ab | Ac) where no symbols have
    if (recursive_prods.size() == prods.size()) {

        std::string err_msg = "Error: Invalid Grammar: Symbol("
                                 + _sym_str + ") is infinitely left recursive";
        std::cerr << err_msg << std::endl;

        throw std::invalid_argument(err_msg);
    }

    std::string new_sym_str = add_aux_sym(grmr, _sym_str);
    sym->clear_productions();

    for (int i  = 0 ; i < prods.size() ; i++) {

        std::vector<cfg::symbol *> prod_symbols = prods[i].get_symbols();

        if (recursive_prods.find(i) == recursive_prods.end()) {
            if (prod_symbols.size() == 1 && prod_symbols[0]->get_key() == EPS)
                prod_symbols.clear();

            prod_symbols.push_back(grmr->get_symbol(new_sym_str));
            grmr->add_production(_sym_str, prod_symbols);

        } else {
            std::vector<cfg::symbol *> new_prod_rhs(prod_symbols.begin() + 1, prod_symbols.end());
            new_prod_rhs.push_back(grmr->get_symbol(new_sym_str));
            grmr->add_production(new_sym_str, new_prod_rhs);
        }
    }

    std::vector<cfg::symbol *> eps_rhs;
    eps_rhs.push_back(grmr->get_symbol(EPS));
    grmr->add_production(new_sym_str, eps_rhs);

    return true;
}
bool remove_left_recursion(cfg *grmr, std::string _sym_a_str, std::string _sym_b_str) {

    bool modified = false;
    std::vector<cfg::symbol::production> new_prods;

    cfg::symbol *sym_a = grmr->get_symbol(_sym_a_str);
    cfg::symbol *sym_b = grmr->get_symbol(_sym_b_str);

    std::vector<cfg::symbol::production> prods_a = sym_a->get_productions();
    std::vector<cfg::symbol::production> prods_b = sym_b->get_productions();

    for (int i = 0 ; i < prods_a.size() ; i++) {

        std::vector<cfg::symbol *> syms_a = prods_a[i].get_symbols();

        if (!syms_a.empty() && syms_a[0] == sym_b) {

            modified = true;

            for (int j = 0 ; j < prods_b.size() ; j++) {

                std::vector<cfg::symbol *> syms_b = prods_b[j].get_symbols();

                std::vector<cfg::symbol *> new_prod_rhs(syms_b.begin(),
                                                        syms_b.end());
                new_prod_rhs.insert(new_prod_rhs.end(), syms_a.begin() + 1, syms_a.end());
                // TODO: remove this from the stack.
                cfg::symbol::production new_production(_sym_a_str, new_prod_rhs);
                new_prods.push_back(new_production);
            }

        } else {
            new_prods.push_back(prods_a[i]);
        }
    }
    if (!modified)
        return false;

    sym_a->clear_productions();

    for (int i = 0 ; i < new_prods.size() ; i++) {
        sym_a->add_production(new_prods[i]);
    }

    return true;
}

bool remove_left_recursion(cfg *grmr) {

    bool modified = false;

    if (grmr == nullptr)
        return false;

    std::vector<std::string> symbols = grmr->get_symbols();
    int len = symbols.size();

    for (int i = 0 ; i < len ; i++) {

        if (grmr->get_symbol(symbols[i])->is_terminal())
            continue;

        for (int j = 0 ; j < i - 1 ; j++) {

            if (grmr->get_symbol(symbols[j])->is_terminal())
                continue;

            modified = remove_left_recursion(grmr, symbols[i], symbols[j]) || modified;

        }

        bool contains_immediate_recursion = false;
        // Remove immediate left recursion of any length.
        do {
            contains_immediate_recursion = remove_immediate_left_recursion(grmr, symbols[i]);
            modified = modified || contains_immediate_recursion;
        } while (contains_immediate_recursion);

    }

    return modified;
}

bool cfg::to_ll1() {

    bool factored = left_factor(this);
    // return factored;
    bool removed_recursion = remove_left_recursion(this);

    return factored || removed_recursion;
}

void print_range(std::unordered_set<std::string> set) {
    std::cout << "{";
    for (std::string key : set) {
        std::cout << (key == EPS ? "ε" : key) << " ";
    }
    std::cout << "}" << std::endl;
}


void cfg::build() {
    build_first(this);
    build_follow(this);
     for (std::string sym_str : get_symbols()) {
         auto sym = get_symbol(sym_str);
         if (!sym->is_terminal()) {
             auto first = sym->get_first();
             auto follow = sym->get_follow();
             std::cout << "Symbol:" << sym_str << "\nFirst: ";
             print_range(first);
             std::cout << "Follow: ";
             print_range(follow);
             std::cout << std::endl;
         }
     }
}

std::ostream &operator<<(std::ostream& stream, const cfg::symbol::production &prod) {

    stream << prod.lhs;
    stream << " -> ";
    for (cfg::symbol *sym : prod.symbols) {
        stream << (sym->is_eps() ? "ε" : sym->get_key()) << ' ';
    }
    stream << std::endl;

    return stream;
}

std::ostream &operator<<(std::ostream& stream, cfg &grmr) {

    for (std::string sym_str : grmr.get_symbols()) {
        for (cfg::symbol::production prod : grmr.get_symbol(sym_str)->get_productions()) {
            stream << prod;
        }
    }

    return stream;
}


cfg::symbol::production::production() : lhs() {

}

cfg::symbol::production::production(std::string _lhs, std::vector<cfg::symbol *> _symbols) : lhs(std::move(_lhs)),
                                                                                             symbols(_symbols) {

}

void cfg::symbol::production::add_symbol(symbol *sym){
    symbols.push_back(sym);
}

std::vector<cfg::symbol *> cfg::symbol::production::get_symbols() {
    return symbols;
}

std::unordered_set<std::string> cfg::symbol::production::get_first() const {
    return first;
}

bool cfg::symbol::production::add_first(std::string _key){
    return first.insert(_key).second;
}

bool cfg::symbol::production::contains_first(std::string _key) const {
    return first.find(_key) != first.end();
}

cfg::symbol::symbol(){
    // TODO
}

cfg::symbol::symbol(std::string _key, bool _terminal) {
    key = _key;
    terminal = _terminal;

}

bool cfg::symbol::is_terminal() const {
    return terminal;
}

bool cfg::symbol::is_eps() const {
    return key == EPS;
}

std::string cfg::symbol::get_key() const {
    return key;
}

void cfg::symbol::add_production(production _production){
    // TODO
    productions.push_back(_production);
}

void cfg::symbol::add_production(std::vector<symbol *> &rhs) {
    production p(key, rhs);
    add_production(p);
}

void cfg::symbol::clear_productions(){
    productions.clear();
}

bool cfg::symbol::add_follow(std::string _key) {
    return follow.insert(_key).second;
}

bool cfg::symbol::contains_first(std::string _key) const {
    for (cfg::symbol::production prod : productions)
        if (prod.contains_first(_key))
            return true;
    return false;
}

bool cfg::symbol::contains_follow(std::string _key) const{
    return (bool) follow.count(_key);
}

std::unordered_set<std::string> cfg::symbol::get_first() const{
    std::unordered_set<std::string> res;
    for (cfg::symbol::production prod : productions)
        res.insert(prod.get_first().begin(), prod.get_first().end());
    return res;
}

std::unordered_set<std::string> cfg::symbol::get_follow() const{
    return follow;
}

std::vector<cfg::symbol::production> cfg::symbol::get_productions(){
    return productions;
}

std::size_t cfg::symbol::get_production_count() const {
    return productions.size();
}

cfg::symbol::production &cfg::symbol::operator[](std::size_t idx) {
    return productions[idx];
}

cfg::cfg(std::string _grammar) : grammar(std::move(_grammar)) {
    add_symbol(EPS, true);
}

cfg::symbol *cfg::add_symbol(std::string _key, bool _terminal){
    if (symbols.count(_key))
        return &symbols[_key];

    cfg::symbol sym(_key, _terminal);
    symbols[_key] = sym;
    return &symbols[_key];
}

bool cfg::add_production(std::string lhs, std::vector<std::string> rhs) {
    std::vector<symbol *> syms;
    for (std::string sym_str : rhs) {
        symbol *sym = get_symbol(sym_str);
        if (sym == nullptr)
            return false;
        syms.push_back(sym);
    }
    return add_production(lhs, syms);
}

bool cfg::add_production(std::string lhs, std::vector<cfg::symbol *> rhs) {
    symbol *lhs_sym = get_symbol(lhs);
    if (lhs_sym == nullptr)
        return false;
    symbol::production prod(lhs, rhs);
    lhs_sym->add_production(prod);
    return true;
}

cfg::symbol *cfg::get_symbol(std::string _key){
    // TODO
    if (!symbols.count(_key)){
        return nullptr;
        //throw std::invalid_argument("No symbol exists for provided key: " + _key);
    }
    return &symbols[_key];
}

std::vector<std::string> cfg::get_symbols(){
    std::vector<std::string> syms;
    for (auto& entry : symbols){
        syms.push_back(entry.first);
    }
    return syms;
}

cfg::symbol *cfg::get_starting_symbol(void) {
    return starting_symbol;
}

void cfg::set_starting_symbol(cfg::symbol *s) {
    starting_symbol = s;
}

