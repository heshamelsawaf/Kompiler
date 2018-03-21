//
// Created by Marwan Tammam on 3/21/18.
//

#include <iostream>
#include <string>
#include "machine.h"
#include "rexplib.h"

using namespace std;

int main(){
    machine m = machine_ops::single_char('a');
    machine p = machine_ops::single_char('b');
//    machine z = machine_ops::oring(m, p);
    m.print_machine();
    p.print_machine();


    return 0;
}