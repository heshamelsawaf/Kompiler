#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../cfgparser.h"
#include "../cfg.h"
#include "../ll1_parser.h"

using namespace std;

TEST(FIRST, ALL1) {
    ifstream grammar_ifs;

    grammar_ifs.open("lan1.cfg");

    auto s = [&grammar_ifs] {
        std::ostringstream ss;
        ss << grammar_ifs.rdbuf();
        return ss.str();
    }();

    if (s.empty())
        return;

    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    cfgparser _cfgparser;

    cfg _cfg = _cfgparser.rules2cfg(s);
    std::cout << "Before:\n\n" << _cfg << std::endl;
    _cfg.to_ll1();
    std::cout << "=======================\nAfter:\n\n" << _cfg << std::endl;
    _cfg.build();
    
    ifstream ttab("m.out");
    machine m("");
    ttab >> m;
    
    parsetable ptab(_cfg);
    // std::cout << ptab << std::endl;

    std::ifstream input_stream("in.c");
    leftmost_derivation d = parse::parse_ll1(ptab, m, input_stream);
    std::cout << d << std::endl;


    grammar_ifs.close();

}

TEST(FIRST, ALL2) {
    ifstream grammar_ifs;
    grammar_ifs.open("lan2.cfg");

    auto s = [&grammar_ifs] {
        std::ostringstream ss;
        ss << grammar_ifs.rdbuf();
        return ss.str();
    }();

    if (s.empty())
        return;

    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    cfgparser _cfgparser;
    cfg _cfg = _cfgparser.rules2cfg(s);
    std::cout << _cfg << "\n-----------------------------------\n";
    
    _cfg.build();

    for (string k : _cfg.get_symbols()) {
        cfg::symbol *s = _cfg.get_symbol(k);

        if (s->is_terminal())
            continue;
        
        cout << "FIRST(" << k << ") = { ";
        for (string f : s->get_first())
            cout << f << " ";
        cout << "}" << endl;
        cout << "FOLLOW(" << k << ") = { ";
        for (string f : s->get_follow())
            cout << f << " ";
        cout << "}" << endl;
    }

    grammar_ifs.close();    
}

TEST(FIRST, ALL3) {
    cfg g("java");

    cfg::symbol *E  = g.add_symbol("E", false);
    cfg::symbol *E_ = g.add_symbol("E`", false);
    cfg::symbol *T  = g.add_symbol("T", false);
    cfg::symbol *T_ = g.add_symbol("T`", false);
    cfg::symbol *F  = g.add_symbol("F", false);
    cfg::symbol *F_ = g.add_symbol("F`", false);
    cfg::symbol *P  = g.add_symbol("P", false);

    cfg::symbol *plus = g.add_symbol("+", true);
    cfg::symbol *mul  = g.add_symbol("*", true);
    cfg::symbol *lp   = g.add_symbol("(", true);
    cfg::symbol *rp   = g.add_symbol(")", true);
    cfg::symbol *a    = g.add_symbol("a", true);
    cfg::symbol *b    = g.add_symbol("b", true);
    cfg::symbol *Em   = g.add_symbol("Em", true);
    cfg::symbol *eps  = g.add_symbol(EPS, true);

    g.set_starting_symbol(E);

    cfg::symbol::production pE("E", vector<cfg::symbol*>() = {T, E_});
    E->add_production(pE);

    cfg::symbol::production pE_1("E`", vector<cfg::symbol*>() = {plus, E});
    cfg::symbol::production pE_2("E`", vector<cfg::symbol*>() = {eps});
    E_->add_production(pE_1);
    E_->add_production(pE_2);

    cfg::symbol::production pT("T", vector<cfg::symbol*>() = {F, T_});
    T->add_production(pT);

    cfg::symbol::production pT_1("T`", vector<cfg::symbol*>() = {T});
    cfg::symbol::production pT_2("T`", vector<cfg::symbol*>() = {eps});
    T_->add_production(pT_1);
    T_->add_production(pT_2);

    cfg::symbol::production pF("F", vector<cfg::symbol*>() = {P, F_});
    F->add_production(pF);

    cfg::symbol::production pF_1("F`", vector<cfg::symbol*>() = {mul, F});
    cfg::symbol::production pF_2("F`", vector<cfg::symbol*>() = {eps});
    F_->add_production(pF_1);
    F_->add_production(pF_2);

    cfg::symbol::production pP1("P", vector<cfg::symbol*>() = {lp, E, rp});
    cfg::symbol::production pP2("P", vector<cfg::symbol*>() = {a});
    cfg::symbol::production pP3("P", vector<cfg::symbol*>() = {b});
    cfg::symbol::production pP4("P", vector<cfg::symbol*>() = {Em});
    P->add_production(pP1);
    P->add_production(pP2);
    P->add_production(pP3);
    P->add_production(pP4);

    g.build();

    cout << g << endl;
    cout << "\n-----------------------------\n";
    for (string k : g.get_symbols()) {
        cfg::symbol *s = g.get_symbol(k);

        if (s->is_terminal())
            continue;
        
        cout << "FIRST(" << k << ") = { ";
        for (string f : s->get_first())
            cout << (f == EPS ? "Ɛ" : f) << " ";
        cout << "}" << endl;
    }
    cout << "\n-----------------------------\n";
    for (string k : g.get_symbols()) {
        cfg::symbol *s = g.get_symbol(k);

        if (s->is_terminal())
            continue;
        
        cout << "FOLLOW(" << k << ") = { ";
        for (string f : s->get_follow())
            cout << (f == EPS ? "Ɛ" : f) << " ";
        cout << "}" << endl;
    }
    cout << "\n-----------------------------\n";
    parsetable t(g);
    cout << t << endl;
    cout << "\n-----------------------------\n";
    
}

TEST(ALL, ALL4) {
    cfg g("java");

    cfg::symbol *R = g.add_symbol("R", false);
    cfg::symbol *S = g.add_symbol("S", false);
    cfg::symbol *U = g.add_symbol("U", false);
    cfg::symbol *T = g.add_symbol("T", false);
    cfg::symbol *V = g.add_symbol("V", false);

    cfg::symbol *s    = g.add_symbol("s", true);
    cfg::symbol *b    = g.add_symbol("b", true);
    cfg::symbol *u    = g.add_symbol("u", true);
    cfg::symbol *t    = g.add_symbol("t", true);
    cfg::symbol *v    = g.add_symbol("v", true);
    cfg::symbol *eps  = g.add_symbol(EPS, true);

    g.set_starting_symbol(S);

    cfg::symbol::production pR1("R", vector<cfg::symbol*>() = {s, U, R, b});
    cfg::symbol::production pR2("R", vector<cfg::symbol*>() = {eps});
    R->add_production(pR1);
    R->add_production(pR2);

    cfg::symbol::production pS("S", vector<cfg::symbol*>() = {R, T});
    S->add_production(pS);

    cfg::symbol::production pU1("U", vector<cfg::symbol*>() = {u, U});
    cfg::symbol::production pU2("U", vector<cfg::symbol*>() = {eps});
    U->add_production(pU1);
    U->add_production(pU2);

    cfg::symbol::production pT1("T", vector<cfg::symbol*>() = {V, t, T});
    cfg::symbol::production pT2("T", vector<cfg::symbol*>() = {eps});
    T->add_production(pT1);
    T->add_production(pT2);

    cfg::symbol::production pV1("V", vector<cfg::symbol*>() = {v, V});
    cfg::symbol::production pV2("V", vector<cfg::symbol*>() = {eps});
    V->add_production(pV1);
    V->add_production(pV2);

    g.build();

    cout << g << endl;
    cout << "\n-----------------------------\n";
    for (string k : g.get_symbols()) {
        cfg::symbol *s = g.get_symbol(k);
        if (s->is_terminal())
            continue;
        
        cout << "FIRST(" << k << ") = { ";
        for (string f : s->get_first())
            cout << (f == EPS ? "Ɛ" : f) << " ";
        cout << "}" << endl;
    }
    cout << "\n-----------------------------\n";
    for (string k : g.get_symbols()) {
        cfg::symbol *s = g.get_symbol(k);

        if (s->is_terminal())
            continue;
        
        cout << "FOLLOW(" << k << ") = { ";
        for (string f : s->get_follow())
            cout << (f == EPS ? "Ɛ" : f) << " ";
        cout << "}" << endl;
    }
    cout << "\n-----------------------------\n";
}

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

    g.set_starting_symbol(E);

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

    EXPECT_EQ(2, (int) E->get_first().size());
    EXPECT_TRUE(E->get_first().find("id") != E->get_first().end());
    EXPECT_TRUE(E->get_first().find("(") != E->get_first().end());

    EXPECT_EQ(2, (int) T->get_first().size());
    EXPECT_TRUE(T->get_first().find("id") != T->get_first().end());
    EXPECT_TRUE(T->get_first().find("(") != T->get_first().end());
    
    EXPECT_EQ(2, (int) F->get_first().size());
    EXPECT_TRUE(F->get_first().find("id") != F->get_first().end());
    EXPECT_TRUE(F->get_first().find("(") != F->get_first().end());

    EXPECT_EQ(2, (int) X->get_first().size());
    EXPECT_TRUE(X->get_first().find(EPS) != X->get_first().end());
    EXPECT_TRUE(X->get_first().find("+") != X->get_first().end());

    EXPECT_EQ(2, (int) Y->get_first().size());
    EXPECT_TRUE(Y->get_first().find(EPS) != Y->get_first().end());
    EXPECT_TRUE(Y->get_first().find("*") != Y->get_first().end());
    
    cout << g << endl;

    for (std::string sym_key : g.get_symbols()) {
        cfg::symbol *s = g.get_symbol(sym_key);
        cout << s->get_key() << ": ";
        for (std::string f : s->get_follow())
            std::cout << f << " ";
        std::cout << std::endl;
    }
}