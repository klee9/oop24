#ifndef _INF_INT_H_
#define _INF_INT_H_

#include <iostream>
#include <string>

using namespace std;

class inf_int
{
private :
    string digits;
    unsigned int length;
    bool sign;

public :
    inf_int();             
    inf_int(int);
    inf_int(const string);
    inf_int(const inf_int&); 
    ~inf_int(); 

    inf_int& operator=(const inf_int&);

    friend bool operator==(const inf_int& , const inf_int&);
    friend bool operator!=(const inf_int& , const inf_int&);
    friend bool operator>(const inf_int& , const inf_int&);
    friend bool operator<(const inf_int& , const inf_int&);

    friend inf_int operator+(const inf_int& , const inf_int&);
    friend inf_int operator-(const inf_int& , const inf_int&);
    friend inf_int operator*(const inf_int& , const inf_int&);
    friend inf_int operator/(const inf_int& , const inf_int&);

    friend ostream& operator<<(ostream& , const inf_int&);
    friend istream& operator>>(istream& , inf_int&);
};

#endif
