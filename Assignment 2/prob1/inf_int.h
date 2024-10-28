#ifndef _INF_INT_H_
#define _INF_INT_H_

#include <iostream>
#include <string>

class inf_int
{
private :
    string digits;
    unsigned int length;
    bool sign;   // true if positive , false if negative
public :
    inf_int();               // assign 0 as a default value
    inf_int(int);
    inf_int(const string);
    inf_int(const inf_int&); // copy constructor
    ~inf_int(); // destructor

    inf_int& operator=(const inf_int&); // assignment operator

    friend bool operator==(const inf_int& , const inf_int&);
    friend bool operator!=(const inf_int& , const inf_int&);
    friend bool operator>(const inf_int& , const inf_int&);
    friend bool operator<(const inf_int& , const inf_int&);

    friend inf_int operator+(const inf_int& , const inf_int&);
    friend inf_int operator-(const inf_int& , const inf_int&);
    friend inf_int operator*(const inf_int& , const inf_int&);
    // friend inf_int operator/(const inf_int& , const inf_int&); // not required

    friend ostream& operator<<(ostream& , const inf_int&);
    // friend istream& operator>>(istream& , inf_int&);    // not required
};

#endif