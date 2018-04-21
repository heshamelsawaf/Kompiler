#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../cfg.h"
#define EOI "$"

using namespace std;

TEST(FIRST, FIRST1) {
    /*  E → TX
     *  X → +TX | ε
     *  T → FY
     *  Y → *FY | ε
     *  F → (E) | id
     */

    cfg g("java");
    
    cfg::symbol *E = g.add_symbol("E", false);
    cfg::symbol *X = g.add_symbol("X", false);
    cfg::symbol *T = g.add_symbol("T", false);
    cfg::symbol *Y = g.add_symbol("Y", false);
    cfg::symbol *F = g.add_symbol("F", false);

    cfg::symbol *plus = g.add_symbol("+", true);
    cfg::symbol *mul  = g.add_symbol("*", true);
    cfg::symbol *lp   = g.add_symbol("(", true);
    cfg::symbol *rp   = g.add_symbol(")", true);
    cfg::symbol *id   = g.add_symbol("id", true);
    cfg::symbol *eps = g.add_symbol(EPS, true);

    cfg::symbol::production pE("E", vector<cfg::symbol*>() = {T, X});
    E->add_production(pE);

    cfg::symbol::production pX1("X", vector<cfg::symbol*>() = {plus, T, X});
    cfg::symbol::production pX2("X", vector<cfg::symbol*>() = {eps});
    X->add_production(pX1);
    X->add_production(pX2);

    cfg::symbol::production pT("T", vector<cfg::symbol*>() = {F, Y});
    T->add_production(pT);

    cfg::symbol::production pY1("Y", vector<cfg::symbol*>() = {mul, F, Y});
    cfg::symbol::production pY2("Y", vector<cfg::symbol*>() = {eps});
    Y->add_production(pY1);
    Y->add_production(pY2);

    cfg::symbol::production pF1("F", vector<cfg::symbol*>() = {lp, E, rp});
    cfg::symbol::production pF2("F", vector<cfg::symbol*>() = {id});
    F->add_production(pF1);
    F->add_production(pF2);

    g.build();

    cout << g << endl;
}