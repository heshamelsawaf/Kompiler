#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../cfg.h"
#define EOI "$"

using namespace std;

TEST(LL1, LL11) {
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

    cfg::symbol::production pE1("E", vector<cfg::symbol*>() = {E, plus, T});
    cfg::symbol::production pE2("E", vector<cfg::symbol*>() = {T});
    E->add_production(pE1);
    E->add_production(pE2);

    cfg::symbol::production pT1("T", vector<cfg::symbol*>() = {T, mul, F});
    cfg::symbol::production pT2("T", vector<cfg::symbol*>() = {F});
    T->add_production(pT1);
    T->add_production(pT2);

    cfg::symbol::production pF1("F", vector<cfg::symbol*>() = {lp, E, rp});
    cfg::symbol::production pF2("F", vector<cfg::symbol*>() = {id});
    F->add_production(pF1);
    F->add_production(pF2);

    g.to_ll1();

    cout << g << endl;

    cfg::symbol *E_ = g.get_symbol("E`");
    cfg::symbol *T_ = g.get_symbol("T`");
    
    EXPECT_FALSE(E_ == nullptr);
    EXPECT_FALSE(T_ == nullptr);

    int nonterm_count = 0;
    for (string s_key : g.get_symbols())
        nonterm_count += !g.get_symbol(s_key)->is_terminal();

    EXPECT_EQ(5, nonterm_count);

    vector<string> prod;

    EXPECT_EQ(1, E->get_productions().size());
    prod = {"T", "E`"};
    for (int i = 0; i < prod.size(); i++)
        EXPECT_EQ(prod[i], E->get_productions()[0].get_symbols()[i]->get_key());

    EXPECT_EQ(2, E_->get_productions().size());
    prod = {"+", "T", "E`"};
    for (int i = 0; i < prod.size(); i++)
        EXPECT_EQ(prod[i], E_->get_productions()[0].get_symbols()[i]->get_key());
    EXPECT_EQ(EPS, E_->get_productions()[1].get_symbols()[0]->get_key());

    EXPECT_EQ(1, T->get_productions().size());
    prod = {"F", "T`"};
    for (int i = 0; i < prod.size(); i++)
        EXPECT_EQ(prod[i], T->get_productions()[0].get_symbols()[i]->get_key());
    
    EXPECT_EQ(2, T_->get_productions().size());
    prod = {"*", "F", "T`"};
    for (int i = 0; i < prod.size(); i++)
        EXPECT_EQ(prod[i], T_->get_productions()[0].get_symbols()[i]->get_key());
    EXPECT_EQ(EPS, T_->get_productions()[1].get_symbols()[0]->get_key());

    EXPECT_EQ(2, F->get_productions().size());
    prod = {"(", "E", ")"};
    for (int i = 0; i < prod.size(); i++)
        EXPECT_EQ(prod[i], F->get_productions()[0].get_symbols()[i]->get_key());
    EXPECT_EQ("id", F->get_productions()[1].get_symbols()[0]->get_key());
    
}

TEST(LL1, LL2) {

    cfg g("java");

    cfg::symbol *E = g.add_symbol("E", false);
    cfg::symbol *T = g.add_symbol("T", false);
    cfg::symbol *F = g.add_symbol("F", false);

    cfg::symbol *plus = g.add_symbol("+", true);
    cfg::symbol *mul  = g.add_symbol("*", true);
    cfg::symbol *lp   = g.add_symbol("(", true);
    cfg::symbol *rp   = g.add_symbol(")", true);
    cfg::symbol *id   = g.add_symbol("id", true);
    cfg::symbol *eps = g.add_symbol(std::string(1, 0x01), true);

    cfg::symbol::production pE1("E", vector<cfg::symbol*>() = {T, E, E, plus, T});
    cfg::symbol::production pE2("E", vector<cfg::symbol*>() = {T, E});
    cfg::symbol::production pE3("E", vector<cfg::symbol*>() = {T, id});
    cfg::symbol::production pE4("E", vector<cfg::symbol*>() = {E, id});
    E->add_production(pE1);
    E->add_production(pE2);
    E->add_production(pE3);
    E->add_production(pE4);

    cfg::symbol::production pT1("T", vector<cfg::symbol*>() = {T, E, T, mul, F});
    cfg::symbol::production pT2("T", vector<cfg::symbol*>() = {E, F});
    cfg::symbol::production pT3("T", vector<cfg::symbol*>() = {eps});
    T->add_production(pT1);
    T->add_production(pT2);
    T->add_production(pT3);

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
    
//    parsetable t(g);
//    t.print_table();
}