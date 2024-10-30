// calculator.cpp 

#include "inf_int.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    cout << "type 'q' to exit" << endl;
    
    char quit = q;
    string cmd, temp;
    
    while((q = getchar()) != 'q') {
        vector<string> op;
        vector<int> order;
        inf_int answer;
        
        getline(cin, cmd);
        sstream stream(cmd);
        
        while(stream >> temp) {
            // add calculation order
            if(temp == '*' || temp == '/')
                order.push_back(op.size());
            op.push_back(temp);
        }

        for(int i = 0; i < order.size(); i++) {
            
            op[order[i]-1) = ;
        }
        
        // 1 3 5 7 9 ...
        // do things in the order vector first.
        
    }
}
