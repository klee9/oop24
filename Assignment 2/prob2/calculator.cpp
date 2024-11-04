#include "inf_int.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string input, left_op, right_op, op;
    
    while(true) {
        // get input
        getline(cin, input);
        if(input == "q") return 0;

        // get left operand, operator, right operand
        istringstream stream(input);
        stream >> left_op >> op >> right_op;
        
        inf_int answer, l(left_op), r(right_op);
        
        if(op == "+") answer = l + r;
        else if(op == "-") answer = l - r;
        else if(op == "*") answer = l * r;
        else if(op == "/") answer = l / r;
        else {
            cout << "unsupported operation." << endl;
            return 1;
        }
        cout << answer << endl;
    }
    return 0;
}
