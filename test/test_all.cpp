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

    grammar_ifs.open("test.cfg");

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
    
    ifstream ttab("test.out");
    machine m("");
    ttab >> m;
    
    parsetable ptab(_cfg);
    // std::cout << ptab << std::endl;

    std::ifstream input_stream("test.c");
    leftmost_derivation d = parse::parse_ll1(ptab, m, input_stream);
    std::cout << d << std::endl;


    grammar_ifs.close();

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