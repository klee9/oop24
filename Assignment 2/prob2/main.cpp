// calculator for inf_int class

#include "inf_int.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string input, left_op, right_op, op;
    
    cout << "A simple calculator for infinite integer. Enter 0 to exit." << endl;
    cout << "Usage: (integer)(space)(operator)(space)(integer)" << endl;
    
    while(true) {
        // get input
        getline(cin, input);
        if(input == "0") return 0;

        // get left operand, operator, right operand
        istringstream stream(input);
        stream >> left_op >> op >> right_op;
        
        inf_int l(left_op), r(right_op);
        
        if(op == "+") cout << l + r << endl;
        else if(op == "-") cout << l - r << endl;
        else if(op == "*") cout << l * r << endl;
        else if(op == "/") cout << l / r << endl;
        else cout << "unsupported operation." << endl;
    }
    return 0;
}
