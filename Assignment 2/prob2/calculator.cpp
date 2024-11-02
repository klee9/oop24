// calculator.cpp 

#include "inf_int.h"
#include <iostream>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

int main()
{
    // input format: (int)(space)(operator)(space)(int)
    string input, left_op, right_op, op;
    
    while(true) {
        getline(cin, input);
        if(input == 'q') return 0;
        
        sstream stream(input)
        sstream >> left_op >> op >> right_op;
    
        inf_int answer, l(left_op), r(right_op);
        
        if(op == '+') answer = l + r;
        else if(op == '-') answer = l - r;
        else if(op == '*') answer = l * r;
        else if(op == '/') answer = l / r;
        else {
            cout << "unsupported operation." << endl;
            return 1;
        }
        cout << answer << endl;
    }
    
    return 0;
}
