#include "inf_int.h"
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

inf_int::inf_int() {
    this->digits = "0";
    this->length = 1;
    this->sign = true;
}

inf_int::inf_int(int n) {
    this->sign = n >= 0;
    this->digits = to_string(abs(n));
    reverse(this->digits.begin(), this->digits.end());
    this->length = this->digits.size();
}

inf_int::inf_int(const string& str) {
    if (str.empty()) {
        this->sign = true;
        this->digits = "0";
        this->length = 1;
    } else {
        this->sign = (str[0] != '-');
        this->digits = (str[0] == '-') ? str.substr(1) : str;
        reverse(this->digits.begin(), this->digits.end());
        this->length = this->digits.size();
    }
}

inf_int::inf_int(const inf_int& a) {
    this->digits = a.digits;
    this->length = a.length;
    this->sign = a.sign;
}

inf_int::~inf_int() {}

inf_int& inf_int::operator=(const inf_int& a) {
    if (this == &a) return *this;
    this->digits = a.digits;
    this->length = a.length;
    this->sign = a.sign;
    return *this;
}

bool operator==(const inf_int& a, const inf_int& b) {
    return (a.digits == b.digits && a.sign == b.sign);
}

bool operator!=(const inf_int& a, const inf_int& b) {
    return !(a == b);
}

bool operator>(const inf_int& a, const inf_int& b) {
    if (a.sign != b.sign) return a.sign;

    if (a.length != b.length) {
        return a.sign ? (a.length > b.length) : (a.length < b.length);
    }
    return a.sign ? (a.digits > b.digits) : (a.digits < b.digits);
}

bool operator<(const inf_int& a, const inf_int& b) {
    return !(a > b) && (a != b);
}

inf_int operator+(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = "";

    int carry = 0;
    int len = max(a.length, b.length);
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        for (int i = 0; i < len; ++i) {
            int sum = (digits_a[i] - '0') + (digits_b[i] - '0') + carry;
            carry = sum / 10;
            result.digits.push_back((sum % 10) + '0');
        }
        if (carry) result.digits.push_back(carry + '0');
        result.sign = a.sign;
    } else {
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        result = a - temp_b;
    }
    result.length = result.digits.size();
    return result;
}

inf_int operator-(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = "";

    int borrow = 0;
    int len = max(a.length, b.length);
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        for (int i = 0; i < len; ++i) {
            int diff = (digits_a[i] - '0') - (digits_b[i] - '0') - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits.push_back(diff + '0');
        }
        result.sign = (a > b) == a.sign;
    } else {
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        result = a + temp_b;
    }
    result.length = result.digits.size();
    return result;
}

inf_int operator*(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = karatsuba(a.digits, b.digits);
    result.sign = (a.sign == b.sign);
    result.length = result.digits.size();
    return result;
}

// Helper function to shift the digits left by `positions`
string shift_left(const string& str, int positions) {
    return str + string(positions, '0');
}

// Karatsuba multiplication for large integers
string karatsuba(const string& x, const string& y) {
    int n = max(x.size(), y.size());

    if (n <= 10) return to_string(stoll(x) * stoll(y));

    int half = n / 2;
    inf_int a(x.substr(0, x.size() - half));
    inf_int b(x.substr(x.size() - half));
    inf_int c(y.substr(0, y.size() - half));
    inf_int d(y.substr(y.size() - half));

    inf_int ac = karatsuba(a.digits, c.digits);
    inf_int bd = karatsuba(b.digits, d.digits);
    inf_int ab_cd = karatsuba((a + b).digits, (c + d).digits);
    inf_int middle = ab_cd - ac - bd;

    return shift_left(ac.digits, 2 * half) + shift_left(middle.digits, half) + bd.digits;
}

inf_int operator/(const inf_int& a, const inf_int& b) {
    if (b.digits == "0") throw runtime_error("Division by zero");

    inf_int dividend = a;
    inf_int divisor = b;
    dividend.sign = divisor.sign = true;

    inf_int quotient("0");
    inf_int temp("0");

    for (int i = dividend.length - 1; i >= 0; --i) {
        temp.digits.insert(temp.digits.begin(), dividend.digits[i]);

        while (temp >= divisor) {
            temp = temp - divisor;
            quotient.digits[i]++;
        }
    }

    quotient.sign = (a.sign == b.sign);
    reverse(quotient.digits.begin(), quotient.digits.end());

    // Remove leading zeros
    while (quotient.digits.size() > 1 && quotient.digits.back() == '0')
        quotient.digits.pop_back();

    quotient.length = quotient.digits.size();
    return quotient;
}

ostream& operator<<(ostream& out, const inf_int& a) {
    if (!a.sign) out << '-';
    string reversed_digits = a.digits;
    reverse(reversed_digits.begin(), reversed_digits.end());
    out << reversed_digits;
    return out;
}
