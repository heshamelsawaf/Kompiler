//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include "machine.h"
#include "rexplib.h"

using namespace std;

int main() {
    machine m = machine_ops::single_char('a');
//    machine p = machine_ops::single_char('b');
//    machine z = machine_ops::concat(m, p);
//    machine z = machine_ops::oring(m, p);
//    vector<machine> v = {m, p, m};
//    z = machine_ops::concat(v);
    machine z = machine_ops::star(m);

//    m.merge(p);
//    m.print_machine();
//    p.print_machine();
    z.print_machine();


    return 0;
}
