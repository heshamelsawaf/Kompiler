//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "parsetable.h"
#include "cfg.h"
#include "ll1_parser.h"

using namespace std;

int main(int argc, char **argv) {
//
//    /* E → E + T | T
//     * T → T * F | F
//     * F → (E) | id
//     * ----should be converted to----
//     *  E → TE'
//     *  E' → +TE' | ε
//     *  T → FT'
//     *  T' → *FT' | ε
//     *  F → (E) | id
//     */
//
//    cfg g("java");
//
//    cfg::symbol *E = g.add_symbol("E", false);
//    cfg::symbol *T = g.add_symbol("T", false);
//    cfg::symbol *F = g.add_symbol("F", false);
//
//    cfg::symbol *plus = g.add_symbol("+", true);
//    cfg::symbol *mul  = g.add_symbol("*", true);
//    cfg::symbol *lp   = g.add_symbol("(", true);
//    cfg::symbol *rp   = g.add_symbol(")", true);
//    cfg::symbol *id   = g.add_symbol("id", true);
//    cfg::symbol *eps = g.add_symbol(EPS, true);
//
//    cfg::symbol::production pE1("E", vector<cfg::symbol*>() = {T, E, E, plus, T});
//    cfg::symbol::production pE2("E", vector<cfg::symbol*>() = {T, E});
//    cfg::symbol::production pE3("E", vector<cfg::symbol*>() = {T, id});
//    cfg::symbol::production pE4("E", vector<cfg::symbol*>() = {E, id});
//    E->add_production(pE1);
//    E->add_production(pE2);
//    E->add_production(pE3);
//    E->add_production(pE4);
//
//    cfg::symbol::production pT1("T", vector<cfg::symbol*>() = {T, E, T, mul, F});
//    cfg::symbol::production pT2("T", vector<cfg::symbol*>() = {E, F});
//    cfg::symbol::production pT3("T", vector<cfg::symbol*>() = {eps});
//    T->add_production(pT1);
//    T->add_production(pT2);
//    T->add_production(pT3);
//
//    cfg::symbol::production pF1("F", vector<cfg::symbol*>() = {lp, E, rp});
//    cfg::symbol::production pF2("F", vector<cfg::symbol*>() = {id});
//    F->add_production(pF1);
//    F->add_production(pF2);
//    cout << "Before LL1: " << endl;
//    cout << g << endl;
//    g.to_ll1();
//    cout << "-----------------------" << endl;
//    cout << "After LL1: " << endl;
//    cout << g << endl;
//
//    parsetable t(g);
//    t.print_table();

    cfg g("");
    cfg::symbol *E = g.add_symbol("E", false);
    cfg::symbol *E_ = g.add_symbol("E'", false);
    cfg::symbol *T = g.add_symbol("T", false);
    cfg::symbol *T_ = g.add_symbol("T'", false);
    cfg::symbol *F = g.add_symbol("F", false);
    cfg::symbol *plus = g.add_symbol("addop", true);
    cfg::symbol *eps = g.add_symbol(std::string(1, 0x01), true);
    cfg::symbol *mul = g.add_symbol("*", true);
    cfg::symbol *open = g.add_symbol("(", true);
    cfg::symbol *close = g.add_symbol(")", true);
    cfg::symbol *id = g.add_symbol("id", true);

    g.set_starting_symbol(E);

    cfg::symbol::production p("E", vector<cfg::symbol*>() = {T, E_});
    p.add_first("(");
    p.add_first("id");
    E->add_production(p);
    cfg::symbol::production p1("E_", vector<cfg::symbol*>() = {plus, T, E_});
    p1.add_first("addop");
    E_->add_production(p1);
    cfg::symbol::production p2("E_", vector<cfg::symbol*>() = {eps});
    p2.add_first(std::string(1, 0x01));
    E_->add_production(p2);

    cfg::symbol::production p3("T", vector<cfg::symbol*>() = {F, T_});
    p3.add_first("(");
    p3.add_first("id");
    T->add_production(p3);
    cfg::symbol::production p4("T_", vector<cfg::symbol*>() = {mul, F, T_});
    p4.add_first("*");
    T_->add_production(p4);
    cfg::symbol::production p5("T_", vector<cfg::symbol*>() = {eps});
    p5.add_first(std::string(1, 0x01));
    T_->add_production(p5);

    cfg::symbol::production p6("F", vector<cfg::symbol*>() = {open, E, close});
    p6.add_first("(");
    F->add_production(p6);
    cfg::symbol::production p7("F", vector<cfg::symbol*>() = {id});
    p7.add_first("id");
    F->add_production(p7);

    E->add_follow(EOI);
    E->add_follow(")");

    E_->add_follow(EOI);
    E_->add_follow(")");

    T->add_follow("addop");
    T->add_follow(")");
    T->add_follow(EOI);

    T_->add_follow("addop");
    T_->add_follow(")");
    T_->add_follow(EOI);

    F->add_follow("addop");
    F->add_follow(")");
    F->add_follow("*");
    F->add_follow(EOI);

    parsetable t(g);

    cout << t;
    
    t.serialize("prototest");

    ifstream ttab("m.out");

    machine m("");

    ttab >> m;
    // std::vector<lexer::token> tokens;
    // tokens.push_back(lexer::token("a", "id"));
    // tokens.push_back(lexer::token("*", "id"));
    // tokens.push_back(lexer::token("b", "id"));

    std::stringstream input_stream("a + b");
    // std::ifstream input_stream("main.cpp");
    leftmost_derivation d = parse::parse_ll1(t, m, input_stream);
    std::cout << d << std::endl;
}

