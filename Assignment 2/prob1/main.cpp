#include <iostream>
#include <string>
#include "inf_int.h"

using namespace std;

int main() {
    // testing constructors
    cout << "[Constructors]\n";
    
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
    cout << "\n\n[Comparison Operators]";
    inf_int f("12345"), g("54321"), h("12345"), i("-12345");

    cout << "\nf = " << f;
    cout << "\ng = " << g; 
    cout << "\nh = " << h;
    cout << "\ni = " << i << "\n\n";
    
    cout << "f == h: " << (f == h) << " (Expected: 1)\n";
    cout << "f != g: " << (f != g) << " (Expected: 1)\n";
    cout << "f < g: " << (f < g) << " (Expected: 1)\n";
    cout << "g > f: " << (g > f) << " (Expected: 1)\n";
    cout << "i < f: " << (i < f) << " (Expected: 1)\n";
    cout << "i == i: " << (i == i) << " (Expected: 1)\n";

    // testing addition
    cout << "\n\n[Addition]";
    inf_int pos_add1("12345678901234567890"), pos_add2("98765432109876543210");
    inf_int neg_add1("-12345678901234567890"), neg_add2("-98765432109876543210");

    cout << "\npos_add1: " << pos_add1;
    cout << "\npos_add2: " << pos_add2;
    cout << "\nneg_add1: " << neg_add1;
    cout << "\nneg_add2: " << neg_add2 << "\n\n";

    cout << "pos_add1 + pos_add2 = " << (pos_add1 + pos_add2) << " (Expected: 111111111011111111100)\n";
    cout << "pos_add1 + neg_add1 = " << (pos_add1 + neg_add1) << " (Expected: 0)\n";
    cout << "pos_add1 + neg_add2 = " << (pos_add1 + neg_add2) << " (Expected: -86419753208641975320)\n";
    cout << "neg_add1 + neg_add2 = " << (neg_add1 + neg_add2) << " (Expected: -111111111011111111100)\n";

    // testing subtraction
    cout << "\n\n[Subtraction]";

    cout << "\npos_add1: " << pos_add1;
    cout << "\npos_add2: " << pos_add2;
    cout << "\nneg_add1: " << neg_add1;
    cout << "\nneg_add2: " << neg_add2 << "\n\n";
    
    cout << "pos_add1 - pos_add1 = " << (pos_add1 - pos_add1) << " (Expected: 0)\n";
    cout << "pos_add2 - pos_add1 = " << (pos_add2 - pos_add1) << " (Expected: 86419753208641975320)\n";
    cout << "pos_add1 - neg_add1 = " << (pos_add1 - neg_add1) << " (Expected: 24691357802469135780)\n";
    cout << "neg_add1 - pos_add1 = " << (neg_add1 - pos_add1) << " (Expected: -24691357802469135780)\n";
    cout << "neg_add1 - neg_add1 = " << (neg_add1 - neg_add1) << " (Expected: 0)\n";
    cout << "neg_add2 - neg_add1 = " << (neg_add2 - neg_add1) << " (Expected: -86419753208641975320)\n";

    // testing multiplication
    cout << "\n\n[Multiplication]";
    inf_int mul_pos("123456"), mul_neg("-654321");

    cout << "\nmul_pos: " << mul_pos;
    cout << "\nmul_neg: " << mul_neg << "\n\n";
    
    cout << "mul_pos * mul_pos = " << (mul_pos * mul_pos) << " (Expected: 15241383936)\n";
    cout << "mul_pos * mul_neg = " << (mul_pos * mul_neg) << " (Expected: -80779853376)\n";
    cout << "mul_neg * mul_pos = " << (mul_neg * mul_pos) << " (Expected: -80779853376)\n";
    cout << "mul_neg * mul_neg = " << (mul_neg * mul_neg) << " (Expected: 428107856641)\n";

    // testing division
    cout << "\n\n[Division]";
    inf_int div_pos("121932631112635269"), div_neg("-121932631112635269");

    cout << "\nmul_pos: " << mul_pos;
    cout << "\ndiv_pos: " << div_pos;
    cout << "\nmul_neg: " << mul_neg;
    cout << "\ndiv_neg: " << div_neg << "\n\n";
    
    cout << "div_pos / mul_pos = " << (div_pos / mul_pos) << " (Expected: 987660633040.397137...)\n";
    cout << "div_pos / mul_neg = " << (div_pos / mul_neg) << " (Expected: -186349866674.973398...)\n";
    cout << "div_neg / mul_pos = " << (div_neg / mul_pos) << " (Expected: -987660633040.397137...)\n";
    cout << "div_neg / mul_neg = " << (div_neg / mul_neg) << " (Expected: 186349866674.973398...)\n";

    // testing edge cases
    cout << "\n\n[Edge Cases with 0]\n";
    inf_int zero("0"), one("1"), neg_one("-1");

    cout << "0 + 0: " << (zero + zero) << " (Expected: 0)\n";
    cout << "0 - 0: " << (zero - zero) << " (Expected: 0)\n";
    cout << "0 * 0: " << (zero * zero) << " (Expected: 0)\n";
    cout << "1 / 1: " << (one / one) << " (Expected: 1)\n";

    // testing I/O operators
    cout << "\n\n[I/O]\n";
    inf_int input_test;
    cout << "Enter a large integer: ";
    cin >> input_test;
    cout << "You entered: " << input_test << endl;

    return 0;
}
