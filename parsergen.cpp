#include <iostream>
#include "cfg.h"
using namespace std;

int main() {
    /* E → E + T | T
     * T → T * F | F
     * F → (E) | id
     * ----should be converted to----
     *  E → TE'
     *  E' → +TE' | ε
     *  T → FT'
     *  T' → *FT' | ε
     *  F → (E) | id
     */

    cfg g("java");
    
    cfg::symbol *E = g.add_symbol("E", false);
    cfg::symbol *T = g.add_symbol("T", false);
    cfg::symbol *F = g.add_symbol("F", false);
    
    cfg::symbol *plus = g.add_symbol("+", true);
    cfg::symbol *mul  = g.add_symbol("*", true);
    cfg::symbol *lp   = g.add_symbol("(", true);
    cfg::symbol *rp   = g.add_symbol(")", true);
    cfg::symbol *id   = g.add_symbol("id", true);
    cfg::symbol *eps = g.add_symbol(EPS, true);

    cfg::symbol::production pE1("E", vector<cfg::symbol*>() = {E, E, plus, T});
    cfg::symbol::production pE2("E", vector<cfg::symbol*>() = {E});
    cfg::symbol::production pE3("E", vector<cfg::symbol*>() = {T});
    E->add_production(pE1);
    E->add_production(pE2);
    E->add_production(pE3);

    cfg::symbol::production pT1("T", vector<cfg::symbol*>() = {T, E, T, mul, F});
    cfg::symbol::production pT2("T", vector<cfg::symbol*>() = {E, F});
    T->add_production(pT1);
    T->add_production(pT2);

    cfg::symbol::production pF1("F", vector<cfg::symbol*>() = {lp, E, rp});
    cfg::symbol::production pF2("F", vector<cfg::symbol*>() = {id});
    F->add_production(pF1);
    F->add_production(pF2);
    cout << "Before LL1: " << endl;
    cout << g << endl;
    g.to_ll1();
    cout << "-----------------------" << endl;
    cout << "After LL1: " << endl;
    cout << g << endl;
}
