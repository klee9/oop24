#include <iostream>
#include <string>
#include "inf_int.h"

using namespace std;

int main() {
    // testing constructors
    cout << "Testing constructors...\n";
    
    inf_int a;                    
    inf_int b(123456789);
    inf_int c("-987654321");
    inf_int d("123456789");       
    inf_int e(b);                
    
    cout << "Default constructor (a): " << a << " (Expected: 0)\n";
    cout << "From integer (b = 123456789): " << b << " (Expected: 123456789)\n";
    cout << "From negative string (c = \"-987654321\"): " << c << " (Expected: -987654321)\n";
    cout << "From positive string (d = \"123456789\"): " << d << " (Expected: 123456789)\n";
    cout << "Copy constructor (e = b): " << e << " (Expected: 123456789)\n";

    // testing comparison
    cout << "\nTesting comparison operators...";
    inf_int f("12345"), g("54321"), h("12345"), i("-12345");

    cout << "\nf = " << f;
    cout << "\ng = " << g; 
    cout << "\nh = " << h;
    cout << "\ni = " << i;
    
    cout << "f == h: " << (f == h) << " (Expected: 1)\n";
    cout << "f != g: " << (f != g) << " (Expected: 1)\n";
    cout << "f < g: " << (f < g) << " (Expected: 1)\n";
    cout << "g > f: " << (g > f) << " (Expected: 1)\n";
    cout << "i < f: " << (i < f) << " (Expected: 1)\n";
    cout << "i == i: " << (i == i) << " (Expected: 1)\n";

    // testing addition
    cout << "\nTesting addition...";
    inf_int pos1("12345678901234567890"), pos2("98765432109876543210");
    inf_int neg1("-12345678901234567890"), neg2("-98765432109876543210");

    cout << "\npos1: " << pos1;
    cout << "\npos2: " << pos2;
    cout << "\nneg1: " << neg1;
    cout << "\nneg2: " << neg2;

    cout << "pos1 + pos2 = " << (pos1 + pos2) << " (Expected: 111111111011111111100)\n";
    cout << "pos1 + neg1 = " << (pos1 + neg1) << " (Expected: 0)\n";
    cout << "pos1 + neg2 = " << (pos1 + neg2) << " (Expected: -86419753208641975320)\n";
    cout << "neg1 + neg2 = " << (neg1 + neg2) << " (Expected: -111111111011111111100)\n";

    // testing subtraction
    cout << "\nTesting subtraction...\n";

    cout << "\npos1: " << pos1;
    cout << "\npos2: " << pos2;
    cout << "\nneg1: " << neg1;
    cout << "\nneg2: " << neg2;
    
    cout << "pos1 - pos1 = " << (pos1 - pos1) << " (Expected: 0)\n";
    cout << "pos2 - pos1 = " << (pos2 - pos1) << " (Expected: 86419753208641975320)\n";
    cout << "pos1 - neg1 = " << (pos1 - neg1) << " (Expected: 24691357802469135780)\n";
    cout << "neg1 - pos1 = " << (neg1 - pos1) << " (Expected: -24691357802469135780)\n";
    cout << "neg1 - neg1 = " << (neg1 - neg1) << " (Expected: 0)\n";
    cout << "neg2 - neg1 = " << (neg2 - neg1) << " (Expected: -86419753208641975320)\n";

    // testing multiplication
    cout << "\nTesting multiplication...\n";
    inf_int pos("123456"), neg("-654321");

    cout << "\npos: " << pos;
    cout << "\nneg: " << neg;
    
    cout << "pos * pos = " << (mul_pos * mul_pos) << " (Expected: 15241383936)\n";
    cout << "pos * neg = " << (mul_pos * mul_neg) << " (Expected: -80779853376)\n";
    cout << "neg * pos = " << (mul_neg * mul_pos) << " (Expected: -80779853376)\n";
    cout << "neg * neg = " << (mul_neg * mul_neg) << " (Expected: 428107856641)\n";

    // testing division
    cout << "\nTesting division...\n";
    inf_int pos("121932631112635269"), neg("-121932631112635269");

    cout << "\npos: " << pos;
    cout << "\nneg: " << neg;
    
    cout << "pos / pos = " << (div_pos / mul_pos) << " (Expected: 987660633040.397137...)\n";
    cout << "pos / neg = " << (div_pos / mul_neg) << " (Expected: -186349866674.973398...)\n";
    cout << "neg / pos = " << (div_neg / mul_pos) << " (Expected: -987660633040.397137...)\n";
    cout << "neg / neg = " << (div_neg / mul_neg) << " (Expected: 186349866674.973398...)\n";

    // testing edge cases
    cout << "\nTesting edge cases with 0...\n";
    inf_int zero("0"), one("1"), neg_one("-1");

    cout << "0 + 0: " << (zero + zero) << " (Expected: 0)\n";
    cout << "0 - 0: " << (zero - zero) << " (Expected: 0)\n";
    cout << "0 * 0: " << (zero * zero) << " (Expected: 0)\n";
    cout << "1 / 1: " << (one / one) << " (Expected: 1)\n";

    // testing I/O operators
    cout << "\nTesting I/O operators...\n";
    inf_int input_test;
    cout << "Enter a large integer: ";
    cin >> input_test;
    cout << "You entered: " << input_test << endl;

    return 0;
}
