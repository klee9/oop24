#include <iostream>
#include <string>
#include "inf_int.h"

using namespace std;

// checks if the result matches the expected value
void check_result(const inf_int& actual, const string& expected) {
    string actual_str = (actual.sign ? "" : "-") + string(actual.digits.rbegin(), actual.digits.rend());
    cout << actual << " (Expected: " << expected << ") -- ";
    if (actual_str == expected) {
        cout << "match\n";
    } else {
        cout << "error\n";
    }
}

// checks comparison operations
void check_comparison(const inf_int& num1, const inf_int& num2, bool expected_eq, bool expected_ne, bool expected_lt, bool expected_gt) {
    cout << num1 << " == " << num2 << ": " << (num1 == num2) << " (Expected: " << expected_eq << ")\n";
    cout << num1 << " != " << num2 << ": " << (num1 != num2) << " (Expected: " << expected_ne << ")\n";
    cout << num1 << " < " << num2 << ": " << (num1 < num2) << " (Expected: " << expected_lt << ")\n";
    cout << num1 << " > " << num2 << ": " << (num1 > num2) << " (Expected: " << expected_gt << ")\n";
}

int main() {
    // testing constructors
    cout << "Testing Constructors...\n";
    
    inf_int a;                    
    inf_int b(123456789);
    inf_int c("-987654321");
    inf_int d("123456789");       
    inf_int e(b);                

    cout << "Default Constructor: "; check_result(a, "0");
    cout << "From Integer: "; check_result(b, "123456789");
    cout << "From Negative String: "; check_result(c, "-987654321");
    cout << "From Positive String: "; check_result(d, "123456789");
    cout << "Copy Constructor: "; check_result(e, "123456789");

    // testing comparison operators
    cout << "\nTesting Comparison Operators...\n";
    inf_int f("12345"), g("54321"), h("12345"), i("-12345");

    cout << "Comparing f and h (equal positive numbers):\n";
    check_comparison(f, h, true, false, false, false);

    cout << "\nComparing f and g (different positive numbers):\n";
    check_comparison(f, g, false, true, true, false);

    cout << "\nComparing g and f (reverse order):\n";
    check_comparison(g, f, false, true, false, true);

    cout << "\nComparing i and f (negative vs positive):\n";
    check_comparison(i, f, false, true, true, false);

    cout << "\nComparing i and i (equal negative numbers):\n";
    check_comparison(i, i, true, false, false, false);

    // testing addition
    cout << "\nTesting Addition...\n";
    inf_int pos1("12345678901234567890"), pos2("98765432109876543210");
    inf_int neg1("-12345678901234567890"), neg2("-98765432109876543210");

    cout << "Positive + Positive: ";
    check_result(pos1 + pos2, "111111111011111111100");

    cout << "Positive + Negative (same magnitude): ";
    check_result(pos1 + neg1, "0");

    cout << "Positive + Negative (different magnitude): ";
    check_result(pos1 + neg2, "-86419753208641975320");

    cout << "Negative + Negative: ";
    check_result(neg1 + neg2, "-111111111011111111100");

    // testing subtraction
    cout << "\nTesting Subtraction...\n";
    cout << "Positive - Positive (same value): ";
    check_result(pos1 - pos1, "0");

    cout << "Positive - Positive (different values): ";
    check_result(pos2 - pos1, "86419753208641975320");

    cout << "Positive - Negative: ";
    check_result(pos1 - neg1, "24691357802469135780");

    cout << "Negative - Positive: ";
    check_result(neg1 - pos1, "-24691357802469135780");

    cout << "Negative - Negative (same value): ";
    check_result(neg1 - neg1, "0");

    cout << "Negative - Negative (different values): ";
    check_result(neg2 - neg1, "-86419753208641975320");

    // testing multiplication with all sign combinations
    cout << "\nTesting Multiplication...\n";
    inf_int mul_pos("123456"), mul_neg("-654321");

    cout << "Positive * Positive: ";
    check_result(mul_pos * mul_pos, "15241383936");

    cout << "Positive * Negative: ";
    check_result(mul_pos * mul_neg, "-80779853376");

    cout << "Negative * Positive: ";
    check_result(mul_neg * mul_pos, "-80779853376");

    cout << "Negative * Negative: ";
    check_result(mul_neg * mul_neg, "428107856641");

    // testing division with all sign combinations
    cout << "\nTesting Division...\n";
    inf_int div_pos("121932631112635269"), div_neg("-121932631112635269");

    cout << "Positive / Positive: ";
    check_result(div_pos / mul_pos, "987654");

    cout << "Positive / Negative: ";
    check_result(div_pos / mul_neg, "-186588");

    cout << "Negative / Positive: ";
    check_result(div_neg / mul_pos, "-987654");

    cout << "Negative / Negative: ";
    check_result(div_neg / mul_neg, "186588");

    // edge cases with zero
    cout << "\nTesting Edge Cases with Zero...\n";
    inf_int zero("0"), one("1"), neg_one("-1");

    cout << "Zero + Zero: "; check_result(zero + zero, "0");
    cout << "Zero - Zero: "; check_result(zero - zero, "0");
    cout << "Zero * Zero: "; check_result(zero * zero, "0");
    cout << "One / One: "; check_result(one / one, "1");

    // testing symmetric cases
    cout << "\nTesting Symmetric Cases...\n";
    inf_int sym("123456789");

    cout << "sym + (-sym): "; check_result(sym + inf_int("-123456789"), "0");
    cout << "sym - sym: "; check_result(sym - sym, "0");
    cout << "sym * 1: "; check_result(sym * one, "123456789");
    cout << "sym * -1: "; check_result(sym * neg_one, "-123456789");

    // testing I/O operators
    cout << "\nTesting I/O Operators...\n";
    inf_int input_test;
    cout << "Enter a large integer: ";
    cin >> input_test;
    cout << "You entered: " << input_test << endl;

    return 0;
}
